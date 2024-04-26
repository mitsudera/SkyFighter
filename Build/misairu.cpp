//=============================================================================
//
// ミサイルモデル処理 [misairu.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "misairu.h"
#include"player.h"
#include"sound.h"
#include"enemy.h"
#include"beffect1.h"
#include"b36.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_MISAIRU			"data/MODEL/misairu.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(50.0f)						// 移動量
#define	VALUE_ROTATEX		(XM_PI * 0.02f)				// 1fの回転量X
#define	VALUE_ROTATEY		(XM_PI * 0.02f)				// 1fの回転量Y

#define MISAIRU_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define MISAIRU_OFFSET_Y		(7.0f)						// ミサイルの足元をあわせる
#define MAX_SPEED				(200.0f)
#define SMOKE_POP				(10)
#define PART_LIFE				(15)
#define SMOKE_SIZE				(1.0f)
#define SMOKE_RAND				(3)
#define SMOKE_MOVE				(0.1f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MISAIRU			g_Misairu[MAX_MISAIRU];				// ミサイル
int g_Misairu_load = 0;
static float hseido = XM_PI / 180;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMisairu(void)
{
	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		LoadModel(MODEL_MISAIRU, &g_Misairu[i].model);
		g_Misairu[i].load = true;

		g_Misairu[i].pos = XMFLOAT3(-50.0f + i * 30.0f, 7.0f, 20.0f);
		g_Misairu[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Misairu[i].scl = XMFLOAT3(2.0f, 2.0f, 2.0f);

		g_Misairu[i].spd  = 0.0f;			// 移動スピードクリア
		g_Misairu[i].size = MISAIRU_SIZE;	// 当たり判定の大きさ

		int muki = 0;
		int move = 1;


		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Misairu[0].model, &g_Misairu[0].diffuse[0]);




		g_Misairu[i].use = false;		//最初は未使用

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMisairu(void)
{

	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		if (g_Misairu[i].load)
		{
			UnloadModel(&g_Misairu[i].model);
			g_Misairu[i].load = false;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMisairu(void)
{
	PLAYER* g_Player = GetPlayer();
	ENEMY* g_Enemy = GetEnemy();
	B36* b36parts = GetB36Parts();
	XMVECTOR qton;
	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		if (g_Misairu[i].use == true)		// このミサイルが使われている？
		{								// Yes
			if (g_Misairu[i].spd < MAX_SPEED)
			{
				g_Misairu[i].spd += 1.0f;
			}


			XMVECTOR temp;
			XMFLOAT3 en ;
			XMVECTOR misvec ;
			XMVECTOR envec ;
			XMVECTOR em ;
			XMVECTOR anglev;
			float angle;
			XMVECTOR ziku;


			switch (g_Misairu[i].pore)
			{
			case -1:


				break;

			case 0:
				en = g_Player[0].pos;
				misvec = XMLoadFloat3(&g_Misairu[i].pos);
				envec = XMLoadFloat3(&en);
				em = misvec - envec;
				em = XMVector3Normalize(em);
				anglev = XMVector3AngleBetweenNormals(g_Misairu[i].vec, em);
				angle;
				XMStoreFloat(&angle, anglev);
				ziku = XMVector3Cross(g_Misairu[i].vec, em);
				ziku = XMVector3Normalize(ziku);
				if (angle < hseido)
				{
					qton = XMQuaternionRotationNormal(ziku, angle);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;
				}
				else
				{
					qton = XMQuaternionRotationNormal(ziku, hseido);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;
				}

				g_Misairu[i].mtxrot = XMMatrixMultiply(g_Misairu[i].mtxrot, XMMatrixRotationQuaternion(qton));




				break;

			case 1:

				en = g_Enemy[g_Misairu[i].obj].pos;
				misvec = XMLoadFloat3(&g_Misairu[i].pos);
				envec = XMLoadFloat3(&en);
				em =  misvec-envec;
				em = XMVector3Normalize(em);
				anglev=XMVector3AngleBetweenNormals(g_Misairu[i].vec, em);
				angle;
				XMStoreFloat(&angle, anglev);
				ziku = XMVector3Cross(g_Misairu[i].vec, em);

				if (angle< hseido)
				{
					qton = XMQuaternionRotationAxis(ziku, angle);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;
				}
				else
				{

					qton = XMQuaternionRotationAxis(ziku, hseido);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;


				}

				g_Misairu[i].mtxrot=XMMatrixMultiply(g_Misairu[i].mtxrot, XMMatrixRotationQuaternion(qton));
				//g_Misairu[i].mtxrot = XMMatrixRotationQuaternion(qton);
				

				break;

			case 2:

				en = b36parts[g_Misairu[i].obj].wpos;
				misvec = XMLoadFloat3(&g_Misairu[i].pos);
				envec = XMLoadFloat3(&en);
				em =  misvec-envec;
				em = XMVector3Normalize(em);
				anglev=XMVector3AngleBetweenNormals(g_Misairu[i].vec, em);
				angle;
				XMStoreFloat(&angle, anglev);
				ziku = XMVector3Cross(g_Misairu[i].vec, em);

				if (angle< hseido)
				{
					qton = XMQuaternionRotationAxis(ziku, angle);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;
				}
				else
				{

					qton = XMQuaternionRotationAxis(ziku, hseido);
					temp = XMVector3Rotate(g_Misairu[i].vec, qton);

					g_Misairu[i].vec = temp;


				}

				g_Misairu[i].mtxrot=XMMatrixMultiply(g_Misairu[i].mtxrot, XMMatrixRotationQuaternion(qton));
				//g_Misairu[i].mtxrot = XMMatrixRotationQuaternion(qton);
				

				break;





			default:
				break;
			}





			g_Misairu[i].pos.x -= g_Misairu[i].vec.m128_f32[0] * g_Misairu[i].spd;
			g_Misairu[i].pos.y -= g_Misairu[i].vec.m128_f32[1] * g_Misairu[i].spd;
			g_Misairu[i].pos.z -= g_Misairu[i].vec.m128_f32[2] * g_Misairu[i].spd;

			g_Misairu[i].life -= 1;
			if (g_Misairu[i].life<=0)
			{
				g_Misairu[i].use = FALSE;
			}


			XMFLOAT3 diff;

			XMFLOAT3 pos;
			diff.x = g_Misairu[i].pos.x - g_Misairu[i].oldpos.x;
			diff.y = g_Misairu[i].pos.y - g_Misairu[i].oldpos.y;
			diff.z = g_Misairu[i].pos.z - g_Misairu[i].oldpos.z;



			for (int j = 0; j < 10; j++)
			{

				XMFLOAT3 enginepos = XMFLOAT3(0.0f, 0.1f, -28.0f);
				XMVECTOR envec = XMLoadFloat3(&enginepos);
				XMVECTOR enqton = XMQuaternionRotationMatrix(g_Misairu[i].mtxrot);
				envec = XMVector3Rotate(envec, enqton);
				XMStoreFloat3(&enginepos, envec);



				XMFLOAT2 moveangle;

				moveangle.x = (rand() % 618) * 0.01f;
				moveangle.y = (rand() % 618) * 0.01f;

				XMFLOAT3 move;

				move.x = sinf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.z = cosf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.y = sinf(moveangle.x) * SMOKE_MOVE;


				pos.x = g_Misairu[i].oldpos.x + j * (diff.x / SMOKE_POP) + enginepos.x;
				pos.y = g_Misairu[i].oldpos.y + j * (diff.y / SMOKE_POP) + enginepos.y;
				pos.z = g_Misairu[i].oldpos.z + j * (diff.z / SMOKE_POP) + enginepos.z;
				//float fSize = (float)((rand() % 30 + 20) * SMOKE_SIZE);
				float fSize = SMOKE_SIZE;
				SetParticle(pos, move, XMFLOAT4(0.9f, 0.9f, 0.9f, 0.85f), fSize, fSize, PART_LIFE + (rand() % PART_LIFE), 1);
			}

			g_Misairu[i].oldpos = g_Misairu[i].pos;


		}

	}


	//cnt += 1;


#ifdef _DEBUG

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	// モデルの色を変更できるよ！半透明にもできるよ。
	//	for (int j = 0; j < g_Misairu[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Misairu[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// モデルの色を元に戻している
	//	for (int j = 0; j < g_Misairu[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Misairu[0].model, j, g_Misairu[0].diffuse[j]);
	//	}
	//}

#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMisairu(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		if (g_Misairu[i].use == false) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Misairu[i].scl.x, g_Misairu[i].scl.y, g_Misairu[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxWorld = XMMatrixMultiply(mtxWorld, g_Misairu[i].mtxrot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Misairu[i].pos.x, g_Misairu[i].pos.y, g_Misairu[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Misairu[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Misairu[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// ミサイルの取得
//=============================================================================
MISAIRU *GetMisairu()
{
	return &g_Misairu[0];
}

//=============================================================================
// ミサイルのセット
//=============================================================================
void SetMisairu(XMFLOAT3 pos, XMVECTOR vec,XMMATRIX rot,float spd,int obj,int pore)
{
	for (int i = 0; i < MAX_MISAIRU; i++)
	{

		if (g_Misairu[i].use == FALSE)
		{
		PlaySound(SOUND_LABEL_SE_misairu001);

			g_Misairu[i].use = TRUE;
			g_Misairu[i].pos = pos;
			g_Misairu[i].vec = vec;
			g_Misairu[i].mtxrot = rot;
			g_Misairu[i].spd = spd+50.0f;
			g_Misairu[i].obj = obj;
			g_Misairu[i].pore = pore;
			g_Misairu[i].life = 180;
			return;
		}
	}


}



