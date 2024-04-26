//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include"light.h"
#include"misairu.h"
#include"enemy.h"
#include"beffect.h"
#include"beffect1.h"
#include"b36.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	MODEL_PLAYER		"data/MODEL/sentouki3.obj"			// 読み込むモデル名
#define	MODEL_PLAYER			"data/MODEL/x00_Body.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_RWING		"data/MODEL/x00_MoveRWing.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_LWING		"data/MODEL/x00_MoveLWing.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_BACK_RWING		"data/MODEL/x00_MoveBackRWing.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_BACK_LWING		"data/MODEL/x00_MoveBackLWing.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_CANARD_RWING		"data/MODEL/x00_MoveRCanardWing.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_CANARD_LWING		"data/MODEL/x00_MoveLCanardWing.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_ENEF				"data/MODEL/enef.obj"			// 読み込むモデル名
#define	VALUE_MOVE			(1.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量
#define VALUE_ROTATE_PAD_ROLL	(0.00002f)
#define VALUE_ROTATE_PAD_PITCH	(0.00001f)
#define VALUE_ROTATE_PAD_YAW	(0.002f)
#define PLAYER_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(7.0f)							// プレイヤーの足元をあわせる
#define PLAYER_PARTS_MAX	(8)
#define ONE_ANGLE			(XM_PIDIV4/45.0f)
#define MAIN_WING_MOVERANGE	(ONE_ANGLE * 105.0f)
#define BACK_WING_MOVERANGE_Z	(ONE_ANGLE * 45.0f)
#define CANARD_WING_MOVERANGE_Z	(ONE_ANGLE * 52.0f)
#define SMOKE_POP				(50)
#define PART_LIFE				(15)
#define SMOKE_SIZE				(1.0f)
#define SMOKE_RAND				(3)
#define SMOKE_MOVE				(0.1f)
#define MAX_HP					(200)
#define STALL_SPEED				(10.0f)
#define DOWNSPEED_MAX			(50.0f)
#define DOWNSPEED_MIN			(1.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER				g_Player;						// プレイヤー
XMFLOAT3 move;
LONG lastlsx;
LONG lastlsy;
XMFLOAT3 lastrot;

static float downspeed;

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// プレイヤーのパーツ用

static ENGINE_EFECT	g_Efect[EFECT_MAX];
// プレイヤーの階層アニメーションデータ
// プレイヤーの左右パーツを動かしているアニメデータ
//0
static INTERPOLATION_DATA move_tbl_enefr[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(10.0f, -0.0f, -50.0f), XMFLOAT3(0.0f,  0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(10.0f, -0.0f, -100.0f), XMFLOAT3(0.0f,  0.0f, 0.0f),    XMFLOAT3(2.0f, 2.0f, 1.0f), 1 },

};



static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	
	move_tbl_enefr,
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER, &g_Player.model);
	g_Player.load = true;

	g_Player.misairustanby[0] = TRUE;
	g_Player.misairustanbylife[0] = 0;
	g_Player.misairustanby[1] = TRUE;
	g_Player.misairustanbylife[1] = 0;
	g_Player.target = -1;
	g_Player.targettype = 1;
	g_Player.pos = { 0.0f,60000.0f, 0.0f };
	g_Player.rot = { 0.0f, 0.0f, 0.0f };
	g_Player.scl = { 0.5f, 0.5f, 0.5f };

	g_Player.axisX = xonevec();
	g_Player.axisY = yonevec();
	g_Player.axisZ = zonevec();


	g_Player.qtonX = XMQuaternionRotationAxis(g_Player.axisX, 0.0f);
	g_Player.qtonY = XMQuaternionRotationAxis(g_Player.axisY, XM_PI);
	g_Player.qtonZ = XMQuaternionRotationAxis(g_Player.axisZ, XM_PI);

	g_Player.mtxrotx = XMMatrixRotationQuaternion(g_Player.qtonX);
	g_Player.mtxroty = XMMatrixRotationQuaternion(g_Player.qtonY);
	g_Player.mtxrotz = XMMatrixRotationQuaternion(g_Player.qtonZ);


	g_Player.spdmax=160.0f;
	g_Player.spdup = 0.0f;
	g_Player.spdupvalue=0.002f;
	g_Player.brake=0.5f;
	g_Player.spdupmax = 1.0f;
	g_Player.hp = MAX_HP;


	g_Player.spd  = 20.0f;			// 移動スピードクリア
	g_Player.size = PLAYER_SIZE;	// 当たり判定の大きさ
			// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Player.model, &g_Player.diffuse[0]);

	g_Player.mtxrot = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	
	//g_Player.axisX = XMFLOAT3(1.0f, 0.0f, 0.0f);
	//g_Player.axisY = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//g_Player.axisZ = XMFLOAT3(0.0f, 0.0f, 1.0f);
	lastlsx = 0;
	lastlsy = 0;
	lastrot = { 0.0f,0.0f,0.0f };

	g_Player.use = true;
	g_Player.open = TRUE;

	// 階層アニメーション用の初期化処理
	g_Player.parent = NULL;			// 本体（親）なのでNULLを入れる

	// パーツの初期化
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// 親子関係
		g_Parts[i].parent = &g_Player;		// ← ここに親のアドレスを入れる
	//	g_Parts[腕].parent= &g_Player;		// 腕だったら親は本体（プレイヤー）
	//	g_Parts[手].parent= &g_Paerts[腕];	// 指が腕の子供だった場合の例

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Parts[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Parts[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット
		// パーツの読み込みはまだしていない
		g_Parts[i].load = 0;
	}

	g_Parts[0].use = TRUE;
	g_Parts[0].parent = &g_Player;	// 親をセット
	//g_Parts[0].tblNo = 0;			// 再生するアニメデータの先頭アドレスをセット
	//g_Parts[0].tblMax = sizeof(move_tbl_mrwingopen) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[0].load = 1;
	g_Parts[0].pos = XMFLOAT3(33.0f, -0.0f, -24.0f);
	LoadModel(MODEL_PLAYER_RWING, &g_Parts[0].model);

	g_Parts[1].use = TRUE;
	g_Parts[1].parent = &g_Player;	// 親をセット
	//g_Parts[1].tblNo = 1;			// 再生するアニメデータの先頭アドレスをセット
	//g_Parts[1].tblMax = sizeof(move_tbl_mlwingopen) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[1].load = 1;
	g_Parts[1].pos = XMFLOAT3(-33.0f, -0.0f, -24.0f);

	LoadModel(MODEL_PLAYER_LWING, &g_Parts[1].model);

	g_Parts[2].use = TRUE;
	g_Parts[2].parent = &g_Player;	// 親をセット
	//g_Parts[2].tblNo = 4;			// 再生するアニメデータの先頭アドレスをセット
	//g_Parts[2].tblMax = sizeof(move_tbl_mrbwing2) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
	g_Parts[2].load = 1;
	
	g_Parts[2].pos = XMFLOAT3(16.0f, 1.0f, -46.0f);
	LoadModel(MODEL_PLAYER_BACK_RWING, &g_Parts[2].model);

	g_Parts[3].use = TRUE;
	g_Parts[3].parent = &g_Player;	// 親をセット
	g_Parts[3].load = 1;
	g_Parts[3].pos = XMFLOAT3(-16.0f, 1.0f, -46.0f);
	LoadModel(MODEL_PLAYER_BACK_LWING, &g_Parts[3].model);

	g_Parts[4].use = TRUE;
	g_Parts[4].parent = &g_Player;	// 親をセット
	g_Parts[4].load = 1;
	g_Parts[4].pos = XMFLOAT3(8.0f, 0.0f,10.0f);
	g_Parts[4].scl = XMFLOAT3(-1.0f, 1.0f,1.0f);
	XMFLOAT3 axz = XMFLOAT3(-0.223f, 0.0f, 0.933f);
	g_Parts[4].axisZ = XMLoadFloat3(&axz);
	g_Parts[4].axisZ = XMVector3Normalize(g_Parts[4].axisZ);

	LoadModel(MODEL_PLAYER_CANARD_RWING, &g_Parts[4].model);

	g_Parts[5].use = TRUE;
	g_Parts[5].parent = &g_Player;	// 親をセット
	g_Parts[5].load = 1;
	g_Parts[5].pos = XMFLOAT3(-8.0f, 0.0f, 10.0f);
	g_Parts[5].scl = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	axz = XMFLOAT3(0.223f, 0.0f, 0.933f);
	g_Parts[5].axisZ = XMLoadFloat3(&axz);
	g_Parts[5].axisZ = XMVector3Normalize(g_Parts[5].axisZ);

	LoadModel(MODEL_PLAYER_CANARD_LWING, &g_Parts[5].model);

	//g_Parts[6].use = TRUE;
	//g_Parts[6].parent = &g_Player;	// 親をセット
	//g_Parts[6].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	//g_Parts[6].tblMax = sizeof(move_tbl_enefr) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	//g_Parts[6].load = 1;
	//g_Parts[6].pos = XMFLOAT3(0.0f, -0.0f, -26.0f);
	//g_Parts[6].scl = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	//g_Parts[6].time =0 ;

	//LoadModel(MODEL_PLAYER_ENEF, &g_Parts[6].model);

	//g_Parts[7].use = TRUE;
	//g_Parts[7].parent = &g_Player;	// 親をセット
	//g_Parts[7].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	//g_Parts[7].tblMax = sizeof(move_tbl_enefr) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	//g_Parts[7].load = 1;
	//g_Parts[7].pos = XMFLOAT3(0.0f, -0.0f, -26.0f);
	//g_Parts[7].scl = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	//g_Parts[7].time = 5;

	//LoadModel(MODEL_PLAYER_ENEF, &g_Parts[6].model);

	//g_Efect[0].load=1;
	//g_Efect[0].use=FALSE;
	//g_Efect[0].parent=& g_Player;;
	//g_Efect[0].tblMax=sizeof(move_tbl_enefr) / sizeof(INTERPOLATION_DATA);
	//g_Efect[0].tblNo=0;

	//for (int i = 0; i < EFECT_TIME_MAX; i++)
	//{
	//	g_Efect[0].time[i] = i;

	//}
	//LoadModel(MODEL_PLAYER_ENEF, &g_Efect[0].model);

	downspeed = DOWNSPEED_MIN;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// モデルの解放処理
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = false;
	}

	// パーツの解放処理
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// パーツの解放処理
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}
	//for (int i = 0; i < EFECT_MAX; i++)
	//{
	//	if (g_Parts[i].load == TRUE)
	//	{
	//		// パーツの解放処理
	//		UnloadModel(&g_Efect[i].model);
	//		g_Parts[i].load = FALSE;
	//	}
	//}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA* cam = GetCamera();



	if (GetMode() == 2)
	{

		//各軸の回転を求める

		if (g_Player.spd > STALL_SPEED)
		{



			//Z
			XMMATRIX lzrotmtx = XMMatrixMultiply(g_Player.mtxrotx, g_Player.mtxroty);
			XMVECTOR lzrotqton = XMQuaternionRotationMatrix(lzrotmtx);
			//X
			XMMATRIX lxrotmtx = XMMatrixMultiply(g_Player.mtxroty, g_Player.mtxrotz);
			XMVECTOR lxrotqton = XMQuaternionRotationMatrix(lxrotmtx);
			//Y
			XMMATRIX lyrotmtx = XMMatrixMultiply(g_Player.mtxrotz, g_Player.mtxrotx);
			XMVECTOR lyrotqton = XMQuaternionRotationMatrix(lyrotmtx);



			g_Player.axisZ = XMVector3Rotate(g_Player.axisZ, lzrotqton);
			g_Player.axisX = XMVector3Rotate(g_Player.axisX, lxrotqton);
			g_Player.axisY = XMVector3Rotate(g_Player.axisY, lyrotqton);

			float rot = 0.0f;

			if (GetKeyboardPress(DIK_LEFT))
			{
				//ロールZ軸回転
				g_Player.rot.z -= 1000 * VALUE_ROTATE_PAD_ROLL;	//入力

			}

			if (GetKeyboardPress(DIK_RIGHT))
			{
				//ロールZ軸回転
				g_Player.rot.z -= -1000 * VALUE_ROTATE_PAD_ROLL;	//入力

			}
			else
			{
				//ロールZ軸回転
				g_Player.rot.z -= -GetLeftStickX(0) * VALUE_ROTATE_PAD_ROLL;	//入力

			}

			rot = g_Player.rot.z - lastrot.z;								//前回との差分を求める
			lastrot.z = g_Player.rot.z;
			//回転軸のベクトルでrot分回転するクォータニオンを求める
			g_Player.qtonZ = XMQuaternionRotationAxis(g_Player.axisZ, rot);
			//Z軸クォータニオンをZ軸回転行列に変換する
			g_Player.mtxrotz = XMMatrixRotationQuaternion(g_Player.qtonZ);
			//Z軸回転行列を回転行列にかける
			g_Player.mtxrot = XMMatrixMultiply(g_Player.mtxrot, g_Player.mtxrotz);

			g_Parts[2].rot.x = rot * 10.0f;
			g_Parts[3].rot.x = -rot * 10.0f;




			//ピッチX軸回転

			if (IsButtonPressed(0, BUTTON_R) && IsButtonPressed(0, BUTTON_L))
			{
				g_Player.rot.x -= GetLeftStickY(0) * VALUE_ROTATE_PAD_PITCH * 1.5f;

			}
			else
			{
				g_Player.rot.x -= GetLeftStickY(0) * VALUE_ROTATE_PAD_PITCH;

			}

			if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_S))
			{
				if (GetKeyboardPress(DIK_UP))
				{
					g_Player.rot.x -= 1000 * VALUE_ROTATE_PAD_PITCH * 1.5f;

				}
				if (GetKeyboardPress(DIK_DOWN))
				{
					g_Player.rot.x -= -1000 * VALUE_ROTATE_PAD_PITCH * 1.5f;

				}

			}
			else
			{
				if (GetKeyboardPress(DIK_UP))
				{
					g_Player.rot.x -= 1000 * VALUE_ROTATE_PAD_PITCH;

				}
				if (GetKeyboardPress(DIK_DOWN))
				{
					g_Player.rot.x -= -1000 * VALUE_ROTATE_PAD_PITCH;

				}

			}


			rot = g_Player.rot.x - lastrot.x;						//前回との差分を求める
			lastrot.x = g_Player.rot.x;
			//回転軸のベクトルでrot分回転するクォータニオンを求める
			g_Player.qtonX = XMQuaternionRotationAxis(g_Player.axisX, rot);
			//Y軸クォータニオンをY軸回転行列に変換する
			g_Player.mtxrotx = XMMatrixRotationQuaternion(g_Player.qtonX);
			//Y軸回転行列を回転行列にかける
			g_Player.mtxrot = XMMatrixMultiply(g_Player.mtxrot, g_Player.mtxrotx);



			g_Parts[2].rot.x -= rot * 30.0f;
			g_Parts[3].rot.x -= rot * 30.0f;
			g_Parts[4].rot.x += rot * 30.0f;
			g_Parts[5].rot.x += rot * 30.0f;


			//ヨー
			if (IsButtonPressed(0, BUTTON_Y) || GetKeyboardPress(DIK_A))
			{
				g_Player.rot.y += VALUE_ROTATE_PAD_YAW;
			}
			if (IsButtonPressed(0, BUTTON_Z) || GetKeyboardPress(DIK_D))
			{
				g_Player.rot.y -= VALUE_ROTATE_PAD_YAW;
			}

			rot = g_Player.rot.y - lastrot.y;						//前回との差分を求める
			lastrot.y = g_Player.rot.y;

			//回転軸のベクトルでrot分回転するクォータニオンを求める
			g_Player.qtonY = XMQuaternionRotationAxis(g_Player.axisY, rot);
			//Y軸クォータニオンをY軸回転行列に変換する
			g_Player.mtxroty = XMMatrixRotationQuaternion(g_Player.qtonY);
			//Y軸回転行列を回転行列にかける
			g_Player.mtxrot = XMMatrixMultiply(g_Player.mtxrot, g_Player.mtxroty);


			if (g_Player.rot.y < -XM_PI)
			{
				g_Player.rot.y += XM_PI * 2.0f;
			}
			if (g_Player.rot.y > XM_PI)
			{
				g_Player.rot.y -= XM_PI * 2.0f;
			}
			if (g_Player.rot.x < -XM_PI)
			{
				g_Player.rot.x += XM_PI * 2.0f;
			}
			if (g_Player.rot.x > XM_PI)
			{
				g_Player.rot.x -= XM_PI * 2.0f;
			}
			if (g_Player.rot.z < -XM_PI)
			{
				g_Player.rot.z += XM_PI * 2.0f;
			}
			if (g_Player.rot.z > XM_PI)
			{
				g_Player.rot.z -= XM_PI * 2.0f;
			}
			downspeed = DOWNSPEED_MIN;
		}
		else
		{
			g_Player.pos.y -= downspeed;
			downspeed *= 1.01f;
			if (downspeed > DOWNSPEED_MAX)
			{
				downspeed = DOWNSPEED_MAX;
			}
		}


		if ((IsButtonPressed(0, BUTTON_R) && IsButtonPressed(0, BUTTON_L)) || (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_S)))
		{
			g_Player.spd *= 0.99f;
		}
		else if (IsButtonPressed(0, BUTTON_R) || GetKeyboardPress(DIK_W))
		{	// 前へ移動


			g_Player.spdup += g_Player.spdupvalue;
			if (g_Player.spdup > g_Player.spdupmax)
			{
				g_Player.spdup = g_Player.spdupmax;
			}
			g_Player.spd += g_Player.spdup;



		}
		else if (IsButtonPressed(0, BUTTON_L) || GetKeyboardPress(DIK_S))
		{
			g_Player.spd *= 0.98f;

			g_Player.spdup *= 0.98f;

		}
		else
		{

			g_Player.spdup -= g_Player.spdupvalue;
			if (g_Player.spd < 60.0f)
			{

				g_Player.spd += 0.5f;

			}
			else
			{
				g_Player.spd -= 0.5f;
			}

		}

		if (g_Player.spd > g_Player.spdmax)
		{
			g_Player.spd = g_Player.spdmax;
		}
		if (g_Player.spdup <= 0)
		{
			g_Player.spdup = 0;
		}


		//Z軸の向きに動く
		move.x = g_Player.axisZ.m128_f32[0] * g_Player.spd;
		move.y = g_Player.axisZ.m128_f32[1] * g_Player.spd;
		move.z = g_Player.axisZ.m128_f32[2] * g_Player.spd;


		// Key入力があったら移動処理する
		if (g_Player.spd > 0.0f)
		{
			g_Player.pos.x -= move.x;
			g_Player.pos.y -= move.y;
			g_Player.pos.z -= move.z;

		}
		float allmove = sqrtf((pow(move.x, 2) + pow(move.z, 2)) + pow(move.y, 2));


#ifdef _DEBUG
		if (GetKeyboardPress(DIK_R))
		{
			g_Player.pos.z = g_Player.pos.x = g_Player.pos.y = 0.0f;
			g_Player.spd = 0.0f;
		}


		//if(IsButtonPressed(0, BUTTON_UP)|| GetKeyboardPress(DIK_W))
		//{
		//	g_Player.pos.z += VALUE_MOVE;
		//}
		//if(IsButtonPressed(0, BUTTON_DOWN) || GetKeyboardPress(DIK_S))
		//{
		//	g_Player.pos.z -= VALUE_MOVE;
		//}
		//if(IsButtonPressed(0, BUTTON_LEFT) || GetKeyboardPress(DIK_LEFT))
		//{
		//	g_Player.pos.x -= VALUE_MOVE;
		//}
		//if(IsButtonPressed(0, BUTTON_RIGHT)||GetKeyboardPress(DIK_RIGHT))
		//{
		//	g_Player.pos.x += VALUE_MOVE;
		//}
		//if(GetKeyboardPress(DIK_UP))
		//{
		//	g_Player.pos.y += VALUE_MOVE;
		//}
		//if(GetKeyboardPress(DIK_DOWN))
		//{
		//	g_Player.pos.y -= VALUE_MOVE;
		//}

#endif
		ENEMY* enemy = GetEnemy();
		B36* b36parts = GetB36Parts();

		XMVECTOR pposv = XMLoadFloat3(&g_Player.pos);
		float lastangle = XM_PI;
		g_Player.target = -1;
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (enemy[i].use == TRUE)
			{
				XMVECTOR eposv = XMLoadFloat3(&enemy[i].pos);
				XMVECTOR pe = pposv - eposv;
				pe = XMVector3Normalize(pe);
				XMVECTOR anglev = XMVector3AngleBetweenNormals(g_Player.axisZ, pe);
				float angle;
				XMStoreFloat(&angle, anglev);
				if (angle < XM_PIDIV4 * 0.33 && angle < lastangle && enemy[i].plegth < 20000.0f)
				{
					g_Player.targettype = 1;
					g_Player.target = i;
					lastangle = angle;
				}
			}
		}

		for (int i = 0; i <B36_PARTS_MAX; i++)
		{
			if (b36parts[i].breaked == FALSE)
			{
				XMVECTOR eposv = XMLoadFloat3(&b36parts[i].wpos);
				XMVECTOR pe = pposv - eposv;
				pe = XMVector3Normalize(pe);
				XMVECTOR anglev = XMVector3AngleBetweenNormals(g_Player.axisZ, pe);
				float angle;
				XMStoreFloat(&angle, anglev);
				if (angle < XM_PIDIV4 * 0.33 && angle < lastangle && b36parts[i].plegth < 20000.0f)
				{
					g_Player.targettype = 2;

					g_Player.target = i;
					lastangle = angle;
				}
			}
		}



		if (IsButtonTriggered(0, BUTTON_C) || GetKeyboardTrigger(DIK_SPACE))
		{
			if (g_Player.target == -1)
			{
				for (int i = 0; i < 2; i++)
				{
					if (g_Player.misairustanby[i] == TRUE)
					{
						g_Player.misairustanby[i] = FALSE;
						g_Player.misairustanbylife[i] = 180;
						SetMisairu(g_Player.pos, g_Player.axisZ, g_Player.mtxrot, g_Player.spd, 0, -1);
						break;
					}
				}


			}
			else
			{
				for (int i = 0; i < 2; i++)
				{

					if (g_Player.misairustanby[i] == TRUE)
					{
						g_Player.misairustanby[i] = FALSE;
						g_Player.misairustanbylife[i] = 180;
						SetMisairu(g_Player.pos, g_Player.axisZ, g_Player.mtxrot, g_Player.spd, g_Player.target, g_Player.targettype);
						i = 2;
						break;
					}
				}
			}
		}
		{
			for (int i = 0; i < 2; i++)
			{

				if (g_Player.misairustanby[i] == FALSE)
				{
					g_Player.misairustanbylife[i] -= 1;
					if (g_Player.misairustanbylife[i] <= 0)
					{
						g_Player.misairustanby[i] = TRUE;
					}

				}
			}

		}

		//if(g_Player.spd>100.0f&&g_Player.open==TRUE)
		//{
		//	g_Player.open = FALSE;
		//	g_Parts[0].tblNo = 6;			// 再生するアニメデータの先頭アドレスをセット
		//	g_Parts[0].tblMax = sizeof(move_tbl_mrwingopen) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		//	g_Parts[1].tblNo = 7;			// 再生するアニメデータの先頭アドレスをセット
		//	g_Parts[1].tblMax = sizeof(move_tbl_mlwingopen) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

		//	
		//}
		if (g_Player.spd < 60.0f)
		{
			g_Parts[0].rot.y = 0.0f;
			g_Parts[1].rot.y = 0.0f;
			g_Parts[2].rot.z = BACK_WING_MOVERANGE_Z;
			g_Parts[3].rot.z = -BACK_WING_MOVERANGE_Z;
			g_Parts[4].rot.z = 0.0f;
			g_Parts[4].qtonZ = XMQuaternionRotationAxis(g_Parts[4].axisZ, g_Parts[4].rot.z);
			g_Parts[5].rot.z = 0.0f;
			g_Parts[5].qtonZ = XMQuaternionRotationAxis(g_Parts[5].axisZ, g_Parts[5].rot.z);
		}
		else if (g_Player.spd > 100.0f)
		{
			g_Parts[0].rot.y = -MAIN_WING_MOVERANGE;
			g_Parts[1].rot.y = MAIN_WING_MOVERANGE;
			g_Parts[2].rot.z = 0.0f;
			g_Parts[3].rot.z = 0.0f;
			g_Parts[4].rot.z = -CANARD_WING_MOVERANGE_Z;
			g_Parts[4].qtonZ = XMQuaternionRotationAxis(g_Parts[4].axisZ, g_Parts[4].rot.z);
			g_Parts[5].rot.z = CANARD_WING_MOVERANGE_Z;
			g_Parts[5].qtonZ = XMQuaternionRotationAxis(g_Parts[5].axisZ, g_Parts[5].rot.z);

		}
		else
		{
			g_Parts[0].rot.y = (-MAIN_WING_MOVERANGE / 40.0f) * (g_Player.spd - 60.0f);
			g_Parts[1].rot.y = (MAIN_WING_MOVERANGE / 40.0f) * (g_Player.spd - 60.0f);
			g_Parts[2].rot.z = BACK_WING_MOVERANGE_Z - (BACK_WING_MOVERANGE_Z / 40.0f) * (g_Player.spd - 60.0f);
			g_Parts[3].rot.z = -BACK_WING_MOVERANGE_Z + (BACK_WING_MOVERANGE_Z / 40.0f) * (g_Player.spd - 60.0f);
			g_Parts[4].rot.z = -(CANARD_WING_MOVERANGE_Z / 40.0f) * (g_Player.spd - 60.0f);
			g_Parts[4].qtonZ = XMQuaternionRotationAxis(g_Parts[4].axisZ, g_Parts[4].rot.z);
			g_Parts[5].rot.z = (CANARD_WING_MOVERANGE_Z / 40.0f) * (g_Player.spd - 60.0f);
			g_Parts[5].qtonZ = XMQuaternionRotationAxis(g_Parts[5].axisZ, g_Parts[5].rot.z);


		}


		// 階層アニメーション
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{

			// 使われているなら処理する
			if ((g_Parts[i].use == TRUE) && (g_Parts[i].tblMax > 0))
			{	// 線形補間の処理
				int nowNo = (int)g_Parts[i].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Parts[i].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

				float nowTime = g_Parts[i].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;								// 現在の移動量を計算している
				Rot *= nowTime;								// 現在の回転量を計算している
				Scl *= nowTime;								// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

				// frameを使て時間経過処理をする
				g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
				if ((int)g_Parts[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Parts[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
				}

			}

		}
		{	// ポイントライト
			LIGHT* light = GetLightData(1);
			XMFLOAT3 pos = g_Player.pos;
			pos.y += 20.0f;

			light->Position = pos;
			light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			light->Type = LIGHT_TYPE_POINT;
			light->Enable = TRUE;
			SetLightData(1, light);
		}

		if (IsButtonPressed(0, BUTTON_R) || GetKeyboardPress(DIK_W))
		{

			XMFLOAT3 diff;

			XMFLOAT3 pos;
			diff.x = g_Player.pos.x - g_Player.oldpos.x;
			diff.y = g_Player.pos.y - g_Player.oldpos.y;
			diff.z = g_Player.pos.z - g_Player.oldpos.z;



			for (int i = 0; i < SMOKE_POP; i++)
			{

				XMFLOAT3 enginepos = XMFLOAT3(5.0f, 0.1f, -28.0f);
				XMVECTOR envec = XMLoadFloat3(&enginepos);
				XMVECTOR enqton = XMQuaternionRotationMatrix(g_Player.mtxrot);
				envec = XMVector3Rotate(envec, enqton);
				XMStoreFloat3(&enginepos, envec);



				XMFLOAT2 moveangle;

				moveangle.x = (rand() % 618) * 0.01f;
				moveangle.y = (rand() % 618) * 0.01f;

				XMFLOAT3 move;

				move.x = sinf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.z = cosf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.y = sinf(moveangle.x) * SMOKE_MOVE;


				//move.x = ((rand() % SMOKE_RAND) - (SMOKE_RAND / 2)) * SMOKE_MOVE;
				//move.y = ((rand() % SMOKE_RAND) - (SMOKE_RAND / 2)) * SMOKE_MOVE;
				//move.z = ((rand() % SMOKE_RAND) - (SMOKE_RAND / 2)) * SMOKE_MOVE;

				pos.x = g_Player.oldpos.x + i * (diff.x / SMOKE_POP) + enginepos.x;
				pos.y = g_Player.oldpos.y + i * (diff.y / SMOKE_POP) + enginepos.y;
				pos.z = g_Player.oldpos.z + i * (diff.z / SMOKE_POP) + enginepos.z;
				//float fSize = (float)((rand() % 30 + 20) * SMOKE_SIZE);
				float fSize = SMOKE_SIZE;
				SetParticle(pos, move, XMFLOAT4(0.9f, 0.9f, 0.9f, 0.85f), fSize, fSize, PART_LIFE + (rand() % PART_LIFE), 1);
			}
			for (int i = 0; i < SMOKE_POP; i++)
			{




				XMFLOAT2 moveangle;

				moveangle.x = (rand() % 618) * 0.01f;
				moveangle.y = (rand() % 618) * 0.01f;

				XMFLOAT3 move;

				move.x = sinf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.z = cosf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.y = sinf(moveangle.x) * SMOKE_MOVE;


				//move.x = ((rand() % SMOKE_RAND) - (SMOKE_RAND / 2)) * SMOKE_MOVE;
				//move.y = ((rand() % SMOKE_RAND) - (SMOKE_RAND / 2)) * SMOKE_MOVE;
				//move.z = ((rand() % SMOKE_RAND) - (SMOKE_RAND / 2)) * SMOKE_MOVE;



				XMFLOAT3 enginepos = XMFLOAT3(-5.0f, 0.1f, -28.0f);
				XMVECTOR envec = XMLoadFloat3(&enginepos);
				XMVECTOR enqton = XMQuaternionRotationMatrix(g_Player.mtxrot);
				envec = XMVector3Rotate(envec, enqton);
				XMStoreFloat3(&enginepos, envec);

				pos.x = g_Player.oldpos.x + i * (diff.x / SMOKE_POP) + enginepos.x;
				pos.y = g_Player.oldpos.y + i * (diff.y / SMOKE_POP) + enginepos.y;
				pos.z = g_Player.oldpos.z + i * (diff.z / SMOKE_POP) + enginepos.z;
				//float fSize = (float)((rand() % 30 + 20) * SMOKE_SIZE);
				float fSize = SMOKE_SIZE;
				SetParticle(pos, move, XMFLOAT4(0.9f, 0.9f, 0.9f, 0.85f), fSize, fSize, PART_LIFE + (rand() % PART_LIFE), 1);
			}
		}

		g_Player.oldpos = g_Player.pos;





#ifdef _DEBUG	// デバッグ情報を表示する
		PrintDebugProc("Player:↑ → ↓ ←　Space\n");
		PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		PrintDebugProc("Playerrot:X:%f Y:%f Z:%f\n", g_Player.rot.x, g_Player.rot.y, g_Player.rot.z);
		PrintDebugProc("camrot:X:%f Y:%f Z:%f\n", cam[0].rot.x, cam[0].rot.y, cam[0].rot.z);
		PrintDebugProc("Playermove:X:%f Y:%f Z:%f:move%f\n", move.x, move.y, move.z, allmove);
		PrintDebugProc("Playerspd:%d\n", g_Player.targettype);
#endif

	}
	else
	{


	//各軸の回転を求める

	if (g_Player.spd > STALL_SPEED)
	{



		//Z
		XMMATRIX lzrotmtx = XMMatrixMultiply(g_Player.mtxrotx, g_Player.mtxroty);
		XMVECTOR lzrotqton = XMQuaternionRotationMatrix(lzrotmtx);
		//X
		XMMATRIX lxrotmtx = XMMatrixMultiply(g_Player.mtxroty, g_Player.mtxrotz);
		XMVECTOR lxrotqton = XMQuaternionRotationMatrix(lxrotmtx);
		//Y
		XMMATRIX lyrotmtx = XMMatrixMultiply(g_Player.mtxrotz, g_Player.mtxrotx);
		XMVECTOR lyrotqton = XMQuaternionRotationMatrix(lyrotmtx);



		g_Player.axisZ = XMVector3Rotate(g_Player.axisZ, lzrotqton);
		g_Player.axisX = XMVector3Rotate(g_Player.axisX, lxrotqton);
		g_Player.axisY = XMVector3Rotate(g_Player.axisY, lyrotqton);

		float rot = 0.0f;


		rot = g_Player.rot.z - lastrot.z;								//前回との差分を求める
		lastrot.z = g_Player.rot.z;
		//回転軸のベクトルでrot分回転するクォータニオンを求める
		g_Player.qtonZ = XMQuaternionRotationAxis(g_Player.axisZ, rot);
		//Z軸クォータニオンをZ軸回転行列に変換する
		g_Player.mtxrotz = XMMatrixRotationQuaternion(g_Player.qtonZ);
		//Z軸回転行列を回転行列にかける
		g_Player.mtxrot = XMMatrixMultiply(g_Player.mtxrot, g_Player.mtxrotz);

		g_Parts[2].rot.x = rot * 10.0f;
		g_Parts[3].rot.x = -rot * 10.0f;





		rot = g_Player.rot.x - lastrot.x;						//前回との差分を求める
		lastrot.x = g_Player.rot.x;
		//回転軸のベクトルでrot分回転するクォータニオンを求める
		g_Player.qtonX = XMQuaternionRotationAxis(g_Player.axisX, rot);
		//Y軸クォータニオンをY軸回転行列に変換する
		g_Player.mtxrotx = XMMatrixRotationQuaternion(g_Player.qtonX);
		//Y軸回転行列を回転行列にかける
		g_Player.mtxrot = XMMatrixMultiply(g_Player.mtxrot, g_Player.mtxrotx);


		g_Parts[2].rot.x -= rot * 30.0f;
		g_Parts[3].rot.x -= rot * 30.0f;
		g_Parts[4].rot.x += rot * 30.0f;
		g_Parts[5].rot.x += rot * 30.0f;


		//ヨー
		if (IsButtonPressed(0, BUTTON_Y) || GetKeyboardPress(DIK_A))
		{
			g_Player.rot.y += VALUE_ROTATE_PAD_YAW;
		}
		if (IsButtonPressed(0, BUTTON_Z) || GetKeyboardPress(DIK_D))
		{
			g_Player.rot.y -= VALUE_ROTATE_PAD_YAW;
		}

		rot = g_Player.rot.y - lastrot.y;						//前回との差分を求める
		lastrot.y = g_Player.rot.y;

		//回転軸のベクトルでrot分回転するクォータニオンを求める
		g_Player.qtonY = XMQuaternionRotationAxis(g_Player.axisY, rot);
		//Y軸クォータニオンをY軸回転行列に変換する
		g_Player.mtxroty = XMMatrixRotationQuaternion(g_Player.qtonY);
		//Y軸回転行列を回転行列にかける
		g_Player.mtxrot = XMMatrixMultiply(g_Player.mtxrot, g_Player.mtxroty);


		if (g_Player.rot.y < -XM_PI)
		{
			g_Player.rot.y += XM_PI * 2.0f;
		}
		if (g_Player.rot.y > XM_PI)
		{
			g_Player.rot.y -= XM_PI * 2.0f;
		}
		if (g_Player.rot.x < -XM_PI)
		{
			g_Player.rot.x += XM_PI * 2.0f;
		}
		if (g_Player.rot.x > XM_PI)
		{
			g_Player.rot.x -= XM_PI * 2.0f;
		}
		if (g_Player.rot.z < -XM_PI)
		{
			g_Player.rot.z += XM_PI * 2.0f;
		}
		if (g_Player.rot.z > XM_PI)
		{
			g_Player.rot.z -= XM_PI * 2.0f;
		}
		downspeed = DOWNSPEED_MIN;
	}


	{	// 前へ移動


		g_Player.spdup += g_Player.spdupvalue;
		if (g_Player.spdup > g_Player.spdupmax)
		{
			g_Player.spdup = g_Player.spdupmax;
		}
		g_Player.spd += g_Player.spdup;



	}
	if (g_Player.spd > g_Player.spdmax)
	{
		g_Player.spd = g_Player.spdmax;
	}
	if (g_Player.spdup <= 0)
	{
		g_Player.spdup = 0;
	}


	//Z軸の向きに動く
	move.x = g_Player.axisZ.m128_f32[0] * g_Player.spd;
	move.y = g_Player.axisZ.m128_f32[1] * g_Player.spd;
	move.z = g_Player.axisZ.m128_f32[2] * g_Player.spd;


	// Key入力があったら移動処理する
	if (g_Player.spd > 0.0f)
	{
		g_Player.pos.x -= move.x;
		g_Player.pos.y -= move.y;
		g_Player.pos.z -= move.z;

	}
	float allmove = sqrtf((pow(move.x, 2) + pow(move.z, 2)) + pow(move.y, 2));


#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = g_Player.pos.y = 0.0f;
		g_Player.spd = 0.0f;
	}


	//if(IsButtonPressed(0, BUTTON_UP)|| GetKeyboardPress(DIK_W))
	//{
	//	g_Player.pos.z += VALUE_MOVE;
	//}
	//if(IsButtonPressed(0, BUTTON_DOWN) || GetKeyboardPress(DIK_S))
	//{
	//	g_Player.pos.z -= VALUE_MOVE;
	//}
	//if(IsButtonPressed(0, BUTTON_LEFT) || GetKeyboardPress(DIK_LEFT))
	//{
	//	g_Player.pos.x -= VALUE_MOVE;
	//}
	//if(IsButtonPressed(0, BUTTON_RIGHT)||GetKeyboardPress(DIK_RIGHT))
	//{
	//	g_Player.pos.x += VALUE_MOVE;
	//}
	//if(GetKeyboardPress(DIK_UP))
	//{
	//	g_Player.pos.y += VALUE_MOVE;
	//}
	//if(GetKeyboardPress(DIK_DOWN))
	//{
	//	g_Player.pos.y -= VALUE_MOVE;
	//}

#endif

	//if(g_Player.spd>100.0f&&g_Player.open==TRUE)
	//{
	//	g_Player.open = FALSE;
	//	g_Parts[0].tblNo = 6;			// 再生するアニメデータの先頭アドレスをセット
	//	g_Parts[0].tblMax = sizeof(move_tbl_mrwingopen) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	//	g_Parts[1].tblNo = 7;			// 再生するアニメデータの先頭アドレスをセット
	//	g_Parts[1].tblMax = sizeof(move_tbl_mlwingopen) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	//	
	//}
	if (g_Player.spd < 60.0f)
	{
		g_Parts[0].rot.y = 0.0f;
		g_Parts[1].rot.y = 0.0f;
		g_Parts[2].rot.z = BACK_WING_MOVERANGE_Z;
		g_Parts[3].rot.z = -BACK_WING_MOVERANGE_Z;
		g_Parts[4].rot.z = 0.0f;
		g_Parts[4].qtonZ = XMQuaternionRotationAxis(g_Parts[4].axisZ, g_Parts[4].rot.z);
		g_Parts[5].rot.z = 0.0f;
		g_Parts[5].qtonZ = XMQuaternionRotationAxis(g_Parts[5].axisZ, g_Parts[5].rot.z);
	}
	else if (g_Player.spd > 100.0f)
	{
		g_Parts[0].rot.y = -MAIN_WING_MOVERANGE;
		g_Parts[1].rot.y = MAIN_WING_MOVERANGE;
		g_Parts[2].rot.z = 0.0f;
		g_Parts[3].rot.z = 0.0f;
		g_Parts[4].rot.z = -CANARD_WING_MOVERANGE_Z;
		g_Parts[4].qtonZ = XMQuaternionRotationAxis(g_Parts[4].axisZ, g_Parts[4].rot.z);
		g_Parts[5].rot.z = CANARD_WING_MOVERANGE_Z;
		g_Parts[5].qtonZ = XMQuaternionRotationAxis(g_Parts[5].axisZ, g_Parts[5].rot.z);

	}
	else
	{
		g_Parts[0].rot.y = (-MAIN_WING_MOVERANGE / 40.0f) * (g_Player.spd - 60.0f);
		g_Parts[1].rot.y = (MAIN_WING_MOVERANGE / 40.0f) * (g_Player.spd - 60.0f);
		g_Parts[2].rot.z = BACK_WING_MOVERANGE_Z - (BACK_WING_MOVERANGE_Z / 40.0f) * (g_Player.spd - 60.0f);
		g_Parts[3].rot.z = -BACK_WING_MOVERANGE_Z + (BACK_WING_MOVERANGE_Z / 40.0f) * (g_Player.spd - 60.0f);
		g_Parts[4].rot.z = -(CANARD_WING_MOVERANGE_Z / 40.0f) * (g_Player.spd - 60.0f);
		g_Parts[4].qtonZ = XMQuaternionRotationAxis(g_Parts[4].axisZ, g_Parts[4].rot.z);
		g_Parts[5].rot.z = (CANARD_WING_MOVERANGE_Z / 40.0f) * (g_Player.spd - 60.0f);
		g_Parts[5].qtonZ = XMQuaternionRotationAxis(g_Parts[5].axisZ, g_Parts[5].rot.z);


	}


	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{

		// 使われているなら処理する
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tblMax > 0))
		{	// 線形補間の処理
			int nowNo = (int)g_Parts[i].time;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Parts[i].tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
			INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// 行動テーブルのアドレスを取得

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

			float nowTime = g_Parts[i].time - nowNo;	// 時間部分である少数を取り出している

			Pos *= nowTime;								// 現在の移動量を計算している
			Rot *= nowTime;								// 現在の回転量を計算している
			Scl *= nowTime;								// 現在の拡大率を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
			XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

			// 計算して求めた回転量を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

			// 計算して求めた拡大率を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

			// frameを使て時間経過処理をする
			g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
			if ((int)g_Parts[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
			{
				g_Parts[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
			}

		}

	}
	{	// ポイントライト
		LIGHT* light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}


	g_Player.oldpos = g_Player.pos;

	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	//mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	
	//mtxRot = mtxrotYXZ(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);

	mtxWorld = XMMatrixMultiply(mtxWorld, g_Player.mtxrot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);



	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);






	//SetCameraAT();
	//SetCamera();
	// モデル描画
	DrawModel(&g_Player.model);


	// パーツの階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{



		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		if ((i==4)||(i==5))
		{
			mtxRot = XMMatrixRotationQuaternion(g_Parts[i].qtonZ);

		}
		else
		{
			mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		}
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ↑
			// g_Player.mtxWorldを指している
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
		if (g_Parts[i].use == FALSE) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		// モデル描画
		DrawModel(&g_Parts[i].model);

	}
	//// ワールドマトリックスの初期化
	//mtxWorld = XMMatrixIdentity();

	//// スケールを反映
	//mtxScl = XMMatrixScaling(g_Player.scl.x, 0.01, g_Player.scl.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//// 回転を反映
	//mtxRot = XMMatrixRotationRollPitchYaw(0.0f, g_Player.rot.y + XM_PI, 0.0f);
	//

	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//// 移動を反映
	//mtxTranslate = XMMatrixTranslation(g_Player.pos.x, 0.1f, g_Player.pos.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);



	//// ワールドマトリックスの設定
	//SetWorldMatrix(&mtxWorld);

	//XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

	//for (int j = 0; j < g_Player.model.SubsetNum; j++)
	//{
	//	SetModelDiffuse(&g_Player.model, j, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	//}

	//// モデル描画
	//DrawModel(&g_Player.model);

	//// モデルの色を元に戻している
	//for (int j = 0; j < g_Player.model.SubsetNum; j++)
	//{
	//	SetModelDiffuse(&g_Player.model, j, g_Player.diffuse[j]);
	//}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}








XMMATRIX mtxrotX(float x)
{
	float sx = sinf(x);
	float cx = cosf(x);

	XMMATRIX mtxrotx = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cx,-sx,0.0f,
		0.0f,sx,cx,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};
	return mtxrotx;
}
XMMATRIX mtxrotY(float y)
{
	float sy = sinf(y);
	float cy = cosf(y);

	XMMATRIX mtxroty = {
		cy,0.0f,sy,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		-sy,0.0f,cy,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};
	return mtxroty;

}
XMMATRIX mtxrotZ(float z)
{
	float sz = sinf(z);
	float cz = cosf(z);

	XMMATRIX mtxrotz = {
		cz,-sz,0.0f,0.0f,
		sz,cz,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};
	return mtxrotz;

}

XMMATRIX mtxrotZYX(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(cy * cz),(sx * sy * cz - cx * sz),(cx * sy * cz + sx * sz),0.0f,
		(cy * sz),(sx * sy * sz + cx * cz),(cx * sy * sz - sx * cz),0.0f,
		(-sy),(sx*cy),(cx*cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}

XMMATRIX mtxrotZXY(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);


	
	mtxrotzyx = {
		(-sx*sy*sz+cy*cz),(-cx*sz),(sx*cy*sz+sy*cz),0.0f,
		(sx*sy*cz+cy*sz),(cx*cz),(-sx*cy*cz+sy*sz),0.0f,
		(-cx*sy),(sx),(cx*cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}
XMMATRIX mtxrotYXZ(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);


	
	mtxrotzyx = {
		(sx*sy*sz+cy*cz),(sx*sy*cz-cy*sz),(cx*sy),0.0f,
		(cx*sz),(cx*cz),(-sx),0.0f,
		(sx*cy*sz-sy*cz),(sx*cy*cz+sy*sz),(cx*cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}

XMMATRIX mtxrotXYZ(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(cy * cz),(-cy*sz),(sy),0.0f,
		(sx*sy*cz+cx*sz),(-sx*sy*sz+ cx*cz),(-sx*cy),0.0f,
		(-cx*sy*cz+sx*sz),(cx*sy*sz+sx*cz),(cx*cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}






XMVECTOR xonevec(void)
{
	XMFLOAT3 x = { 1.0f,0.0f,0.0f };

	XMVECTOR vec = XMLoadFloat3(&x);
	return vec;
}

XMVECTOR yonevec(void)
{
	XMFLOAT3 y = { 0.0f,1.0f,0.0f };

	XMVECTOR vec = XMLoadFloat3(&y);
	return vec;
}

XMVECTOR zonevec(void)
{
	XMFLOAT3 z = { 0.0f,0.0f,1.0f };

	XMVECTOR vec = XMLoadFloat3(&z);
	return vec;
}


void raunch(void)
{
	if (g_Player.target == -1)
	{
		for (int i = 0; i < 2; i++)
		{
			if (g_Player.misairustanby[i] == TRUE)
			{
				g_Player.misairustanby[i] = FALSE;
				g_Player.misairustanbylife[i] = 240;
				SetMisairu(g_Player.pos, g_Player.axisZ, g_Player.mtxrot, g_Player.spd, 0, -1);
				return;
			}
		}


	}
	else
	{
		for (int i = 0; i < 2; i++)
		{

			if (g_Player.misairustanby[i] == TRUE)
			{
				g_Player.misairustanby[i] = FALSE;
				g_Player.misairustanbylife[i] = 240;
				SetMisairu(g_Player.pos, g_Player.axisZ, g_Player.mtxrot, g_Player.spd, g_Player.target, g_Player.targettype);
				return;
			}
		}
	}

}