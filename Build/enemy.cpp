//=============================================================================
//
// モデル処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "enemy.h"
#include"light.h"
#include"misairu.h"
#include"player.h"
#include"beffect1.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY		"data/MODEL/sentouki3.obj"			// 読み込むモデル名
#define	MODEL_ENEMY_ENEF				"data/MODEL/enef.obj"			// 読み込むモデル名
#define MODEL_REDC			"data/MODEL/redcorn.obj"
#define MODEL_TORUS			"data/MODEL/torus.obj"
#define	VALUE_MOVE			(1.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量
#define VALUE_ROTATE_PAD_ROLL	(0.00003f)
#define VALUE_ROTATE_PAD_PITCH	(0.00002f)
#define VALUE_ROTATE_PAD_YAW	(0.01f)
#define ENEMY_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define ENEMY_OFFSET_Y		(7.0f)							// プレイヤーの足元をあわせる
#define ENEMY_PARTS_MAX	(8)
#define ONE_ANGLE			(XM_PIDIV4/45.0f)
#define MAIN_WING_MOVERANGE	(ONE_ANGLE * 105.0f)
#define BACK_WING_MOVERANGE_Z	(ONE_ANGLE * 45.0f)
#define CANARD_WING_MOVERANGE_Z	(ONE_ANGLE * 52.0f)
#define SMOKE_POP				(10)
#define PART_LIFE				(15)
#define SMOKE_SIZE				(1.0f)
#define SMOKE_RAND				(3)
#define SMOKE_MOVE				(0.1f)
#define TEXTURE_MAX			(3)				// テクスチャの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY				g_Enemy[MAX_ENEMY];						
static XMFLOAT3 move;
static LONG lastlsx;
static LONG lastlsy;
static XMFLOAT3 lastrot;
//static float hseido= XM_PI / 360;
static RED_CORN redc;
static TORUS torus;

//static ENGINE_EFECT	g_Efect[EFECT_MAX];

XMFLOAT3 g_hompoint[10][10];
static ID3D11Buffer* g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11Buffer* g_VertexBufferRed = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TextureName[] =
{
	"data/TEXTURE/siromaru.png",
	"data/TEXTURE/sirowaku.png",
	"data/TEXTURE/sirohisi.png",
};
static MATERIAL g_cirmat;
static MATERIAL g_redmat;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}



	MakeVertexCir();
	MakeVertexCirRed();

	ZeroMemory(&g_cirmat, sizeof(g_cirmat));
	g_cirmat.Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	ZeroMemory(&g_redmat, sizeof(g_redmat));
	g_redmat.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			g_hompoint[x][y] = XMFLOAT3(1000.0f*x,60000.0f, 1000.0f * y + 50000.0f);

		}
	}

	for (int i = 0; i < MAX_ENEMY; i++)
	{

		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = true;

		g_Enemy[i].pos = { 1000.0f*i,10000.0f, 1000.0f };
		g_Enemy[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Enemy[i].scl = { 0.5f, 0.5f, 0.5f };

		g_Enemy[i].axisX = xonevec();
		g_Enemy[i].axisY = yonevec();
		g_Enemy[i].axisZ = zonevec();


		g_Enemy[i].qtonX = XMQuaternionRotationAxis(g_Enemy[i].axisX, 0.0f);
		g_Enemy[i].qtonY = XMQuaternionRotationAxis(g_Enemy[i].axisY, XM_PI);
		g_Enemy[i].qtonZ = XMQuaternionRotationAxis(g_Enemy[i].axisZ, XM_PI);

		g_Enemy[i].mtxrotx = XMMatrixRotationQuaternion(g_Enemy[i].qtonX);
		g_Enemy[i].mtxroty = XMMatrixRotationQuaternion(g_Enemy[i].qtonY);
		g_Enemy[i].mtxrotz = XMMatrixRotationQuaternion(g_Enemy[i].qtonZ);


		g_Enemy[i].spdmax = 120.0f;
		g_Enemy[i].spdup = 1.0f;
		g_Enemy[i].spdupvalue = 0.001f;
		g_Enemy[i].brake = 0.5f;
		g_Enemy[i].hseido= XM_PI / 360;


		g_Enemy[i].spd = 40.0f;			// 移動スピードクリア
		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ
				// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[0]);

		g_Enemy[i].mtxrot = {
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f,

		};


		//g_Enemy[i].axisX = XMFLOAT3(1.0f, 0.0f, 0.0f);
		//g_Enemy[i].axisY = XMFLOAT3(0.0f, 1.0f, 0.0f);
		//g_Enemy[i].axisZ = XMFLOAT3(0.0f, 0.0f, 1.0f);
		lastlsx = 0;
		lastlsy = 0;
		lastrot = { 0.0f,0.0f,0.0f };

		g_Enemy[i].use = FALSE;

		g_Enemy[i].misairustanby = TRUE;

		g_Enemy[i].movemode = 0;
		g_Enemy[i].modelife = 180+(rand()%180);

		g_Enemy[i].hp = 100;

	}

	LoadModel(MODEL_REDC, &redc.model);
	redc.load = true;


	LoadModel(MODEL_TORUS, &torus.model);
	torus.load = true;


	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{


		// モデルの解放処理
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = false;
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

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	if (g_VertexBufferRed)
	{
		g_VertexBufferRed->Release();
		g_VertexBufferRed = NULL;
	}


		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			if (g_Texture[i])
			{
				g_Texture[i]->Release();
				g_Texture[i] = NULL;
			}
		}



	if (redc.load==true)
	{
		UnloadModel(&redc.model);
		redc.load = false;
	}
	if (torus.load==true)
	{
		UnloadModel(&torus.model);
		torus.load = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{

	PLAYER* player = GetPlayer();
	CAMERA* camera = GetCamera();
	XMVECTOR qton;

	XMVECTOR pposv = XMLoadFloat3(&player[0].pos);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use==TRUE)
		{

			if (g_Enemy[i].pos.y<3000.0f)
			{
				g_Enemy[i].movemode = 2;
				g_Enemy[i].modelife = 180.0f;
			}


			XMVECTOR temp;
			XMFLOAT3 pl = player[0].pos;
			XMVECTOR envec = XMLoadFloat3(&g_Enemy[i].pos);
			XMVECTOR plvec = XMLoadFloat3(&pl);
			XMVECTOR camvec = XMLoadFloat3(&camera->pos);
			XMVECTOR ep = envec - plvec;
			XMVECTOR pe = plvec - envec;
			XMVECTOR ec = envec - camvec;

			XMVECTOR eplen = XMVector3Length(ep);
			XMStoreFloat(&g_Enemy[i].plegth, eplen);
			ep = XMVector3Normalize(ep);
			ec = XMVector3Normalize(ec);
			XMStoreFloat3(&g_Enemy[i].ep, ep);
			XMStoreFloat3(&g_Enemy[i].ec, ec);
			XMVECTOR anglev = XMVector3AngleBetweenNormals(g_Enemy[i].axisZ, ep);
			float angle;
			XMStoreFloat(&angle, anglev);
			XMVECTOR ziku = XMVector3Cross(g_Enemy[i].axisZ, ep);
			ziku = XMVector3Normalize(ziku);

			switch (g_Enemy[i].movemode)
			{
			case 0:

				if (angle < g_Enemy[i].hseido)
				{
					qton = XMQuaternionRotationNormal(ziku, angle);
					temp = XMVector3Rotate(g_Enemy[i].axisZ, qton);

					g_Enemy[i].axisZ = temp;
				}
				else
				{

					qton = XMQuaternionRotationNormal(ziku, g_Enemy[i].hseido);
					temp = XMVector3Rotate(g_Enemy[i].axisZ, qton);

					g_Enemy[i].axisZ = temp;


				}



				g_Enemy[i].mtxrot = XMMatrixMultiply(g_Enemy[i].mtxrot, XMMatrixRotationQuaternion(qton));



				//g_Enemy[i]

				break;
			case 1:
				break;





			case 2:


				temp;
				pl = g_hompoint[g_Enemy[i].htx][g_Enemy[i].htz];
				envec = XMLoadFloat3(&g_Enemy[i].pos);
				plvec = XMLoadFloat3(&pl);
				ep = envec - plvec;
				pe = plvec - envec;
				ep = XMVector3Normalize(ep);
				anglev = XMVector3AngleBetweenNormals(g_Enemy[i].axisZ, ep);
				float angle;
				XMStoreFloat(&angle, anglev);
				ziku = XMVector3Cross(g_Enemy[i].axisZ, ep);
				ziku = XMVector3Normalize(ziku);

				if (angle < g_Enemy[i].hseido)
				{
					qton = XMQuaternionRotationNormal(ziku, angle);
					temp = XMVector3Rotate(g_Enemy[i].axisZ, qton);

					g_Enemy[i].axisZ = temp;
				}
				else
				{

					qton = XMQuaternionRotationNormal(ziku, g_Enemy[i].hseido);
					temp = XMVector3Rotate(g_Enemy[i].axisZ, qton);

					g_Enemy[i].axisZ = temp;


				}



				g_Enemy[i].mtxrot = XMMatrixMultiply(g_Enemy[i].mtxrot, XMMatrixRotationQuaternion(qton));





				break;

			default:
				break;
			}

			g_Enemy[i].pos.x -= g_Enemy[i].axisZ.m128_f32[0] * g_Enemy[i].spd;
			g_Enemy[i].pos.y -= g_Enemy[i].axisZ.m128_f32[1] * g_Enemy[i].spd;
			g_Enemy[i].pos.z -= g_Enemy[i].axisZ.m128_f32[2] * g_Enemy[i].spd;


			anglev = XMVector3AngleBetweenNormals(zonevec(), pe);
			XMStoreFloat(&angle, anglev);
			ziku = XMVector3Cross(zonevec(), pe);
			ziku = XMVector3Normalize(ziku);
			qton = XMQuaternionRotationNormal(ziku, angle);
			g_Enemy[i].eprot = XMMatrixRotationQuaternion(qton);

			if (g_Enemy[i].plegth > 30000.0f)
			{
				g_Enemy[i].movemode = 0;
				g_Enemy[i].modelife = 600 + (rand() % 600);
			}
			else if (g_Enemy[i].pos.y < 10000.0f)
			{
				g_Enemy[i].movemode = 2;
				g_Enemy[i].htx = rand() % 10;
				g_Enemy[i].htz = rand() % 10;


			}
			//else if (g_Enemy[i].plegth < 3000.0f)
			//{
			//	g_Enemy[i].movemode = 1;
			//	g_Enemy[i].modelife = 600 + (rand() % 600);
			//}

			g_Enemy[i].modelife -= 1;
			if (g_Enemy[i].modelife <= 0)
			{
				if (g_Enemy[i].movemode == 0)
				{
					g_Enemy[i].movemode = 1;
				}
				else if (g_Enemy[i].movemode == 2)
				{
					g_Enemy[i].movemode = rand() % 3;

				}
				else
				{
					g_Enemy[i].movemode = rand() % 2;

				}


				if (g_Enemy[i].movemode == 0)
				{
					g_Enemy[i].modelife = 600 + (rand() % 600);

				}
				if (g_Enemy[i].movemode == 1)
				{
					g_Enemy[i].modelife = 600 + (rand() % 600);

				}
				if (g_Enemy[i].movemode == 2)
				{
					g_Enemy[i].modelife = 600 + (rand() % 600);

				}


				if (g_Enemy[i].movemode == 2)
				{
					g_Enemy[i].htx = rand() % 10;
					g_Enemy[i].htz = rand() % 10;

				}


			}


			XMFLOAT3 diff;

			XMFLOAT3 pos;
			diff.x = g_Enemy[i].pos.x - g_Enemy[i].oldpos.x;
			diff.y = g_Enemy[i].pos.y - g_Enemy[i].oldpos.y;
			diff.z = g_Enemy[i].pos.z - g_Enemy[i].oldpos.z;

			XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;


			for (int j = 0; j < SMOKE_POP; j++)
			{

				XMFLOAT3 enginepos = XMFLOAT3(0.0f, 0.1f, -28.0f);
				XMVECTOR envec = XMLoadFloat3(&enginepos);
				XMVECTOR enqton = XMQuaternionRotationMatrix(g_Enemy[i].mtxrot);
				envec = XMVector3Rotate(envec, enqton);
				XMStoreFloat3(&enginepos, envec);



				XMFLOAT2 moveangle;

				moveangle.x = (rand() % 618) * 0.01f;
				moveangle.y = (rand() % 618) * 0.01f;

				XMFLOAT3 move;

				move.x = sinf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.z = cosf(moveangle.y) * cosf(moveangle.x) * SMOKE_MOVE;
				move.y = sinf(moveangle.x) * SMOKE_MOVE;



				pos.x = g_Enemy[i].oldpos.x + j * (diff.x / SMOKE_POP) + enginepos.x;
				pos.y = g_Enemy[i].oldpos.y + j * (diff.y / SMOKE_POP) + enginepos.y;
				pos.z = g_Enemy[i].oldpos.z + j * (diff.z / SMOKE_POP) + enginepos.z;
				//float fSize = (float)((rand() % 30 + 20) * SMOKE_SIZE);
				float fSize = SMOKE_SIZE;
				SetParticle(pos, move, XMFLOAT4(0.9f, 0.9f, 0.9f, 0.85f), fSize, fSize, PART_LIFE + (rand() % PART_LIFE), 1);
			}

			g_Enemy[i].oldpos = g_Enemy[i].pos;


			XMVECTOR eposv = XMLoadFloat3(&g_Enemy[i].pos);
			XMVECTOR ep2 = eposv-pposv;
			ep2 = XMVector3Normalize(ep2);
			XMVECTOR anglev2 = XMVector3AngleBetweenNormals(g_Enemy[i].axisZ, ep2);
			float angle2;
			XMStoreFloat(&angle2, anglev2);
			if (angle2 < XM_PIDIV4 * 0.33f  &&10000.0f<g_Enemy[i].plegth < 20000.0f&&g_Enemy[i].misairustanby==TRUE)
			{
				SetMisairu(g_Enemy[i].pos, g_Enemy[i].axisZ, g_Enemy[i].mtxrot, g_Enemy[i].spd,-1,0);
				g_Enemy[i].misairustanby = FALSE;
				g_Enemy[i].misairuwait = 600;


			}


			if (g_Enemy[i].misairustanby == FALSE)
			{
				g_Enemy[i].misairuwait -= 1;
				if (g_Enemy[i].misairuwait <=0 )
				{
					g_Enemy[i].misairustanby = TRUE;

				}


			}



			XMVECTOR epos = XMLoadFloat3(&player[0].pos);
			XMVECTOR vec = XMLoadFloat3(&g_Enemy[i].pos) - epos;			//自分とプレイヤーの差分を求める
			g_Enemy[i].ingrot.y = atan2f(vec.m128_f32[0], vec.m128_f32[2]);		//その差分を使って角度を求める
			if (vec.m128_f32[2] < 0)
			{
				vec.m128_f32[2] *= -1;
			}

			g_Enemy[i].ingrot.x = -atan2f(vec.m128_f32[2], vec.m128_f32[1]);		//その差分を使って角度を求める
			g_Enemy[i].ingrot.x -= XM_PIDIV2;
			g_Enemy[i].ingrot.z = 0.0f - XM_PI;


		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	PLAYER* player = GetPlayer();
	CAMERA* camera = GetCamera();
	XMVECTOR camposv = XMLoadFloat3( &camera->pos);
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;


	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)
		{



			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, -g_Enemy[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			//mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);

			//mtxRot = mtxrotYXZ(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);

			mtxWorld = XMMatrixMultiply(mtxWorld, g_Enemy[i].mtxrot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);



			XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);






			//SetCameraAT();
			//SetCamera();
			// モデル描画
			DrawModel(&g_Enemy[i].model);


			
				// αテスト設定
		// αテストを有効に
		SetAlphaTestEnable(TRUE);

		SetCullingMode(CULL_MODE_NONE);

		// ライティングを無効
		SetLightEnable(FALSE);


			if (player->target==i&&player->targettype==1)
			{
				// 頂点バッファ設定
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBufferRed, &stride, &offset);

			}
			else
			{
				// 頂点バッファ設定
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);


			}


			// プリミティブトポロジ設定
			GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			XMMATRIX mtxView = XMLoadFloat4x4(&camera->mtxView);




			// 正方行列（直交行列）を転置行列させて逆行列を作ってる版(速い)
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];




			//// スケールを反映
			//float sclk = (1 / g_Enemy[i].plegth);

			mtxScl = XMMatrixScaling(0.1f, 0.1f, 0.1f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			//mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);

			//mtxRot = mtxrotYXZ(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);

			//mtxWorld = XMMatrixMultiply(mtxWorld, g_Enemy[i].eprot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(camera[0].pos.x + g_Enemy[i].ec.x * 50.0f, camera[0].pos.y + g_Enemy[i].ec.y * 50.0f, camera[0].pos.z + g_Enemy[i].ec.z * 50.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			SetModelDiffuse(&torus.model, 0, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));


			
			// マテリアル設定
			SetMaterial(g_cirmat);
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);

			if (player->target == i && player->targettype == 1)
			{
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

				// ポリゴンの描画
				GetDeviceContext()->Draw(4, 0);


			}


			//DrawModel(&torus.model);



			// ライティングを有効に
			SetLightEnable(TRUE);


			SetCullingMode(CULL_MODE_BACK);


			// αテストを無効に
			SetAlphaTestEnable(FALSE);


			










			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();


			// スケールを反映
			float sclk = (1 / g_Enemy[i].plegth);

			mtxScl = XMMatrixScaling(0.2f + 20.0f * sclk, 0.2f + 20.0f * sclk, -0.2f - 20.0f * sclk);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].ingrot.x, g_Enemy[i].ingrot.y + XM_PI, g_Enemy[i].ingrot.z);

			//mtxRot = mtxrotYXZ(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);

			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(player[0].pos.x + g_Enemy[i].ep.x * 50.0f, player[0].pos.y + g_Enemy[i].ep.y * 50.0f, player[0].pos.z + g_Enemy[i].ep.z * 50.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);
			//// 移動を反映
			//mtxTranslate = XMMatrixTranslation(player[0].pos.x, player[0].pos.y, player[0].pos.z+50.0f);
			//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			SetModelDiffuse(&redc.model, 0, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.3f));

			DrawModel(&redc.model);



		}

	}

}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_Enemy[0];
}


HRESULT MakeVertexCir(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = 20.0f;
	float fHeight = 20.0f;

	// 頂点座標の設定
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight/2, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight/2, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, -fHeight / 2, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, -fHeight / 2, 0.0f);

	// 法線の設定
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 拡散光の設定
	vertex[0].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// テクスチャ座標の設定
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}
HRESULT MakeVertexCirRed(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBufferRed);

	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBufferRed, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = 20.0f;
	float fHeight = 20.0f;

	// 頂点座標の設定
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight/2, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight/2, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, -fHeight / 2, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, -fHeight / 2, 0.0f);

	// 法線の設定
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 拡散光の設定
	vertex[0].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	// テクスチャ座標の設定
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBufferRed, 0);

	return S_OK;
}

void SetEnemy(int Lv)
{
	int	x, y;
	x = rand() % 10;
	y = rand() % 10;
	XMFLOAT3 pos = g_hompoint[x][y];


	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE)
		{

			switch (Lv)
			{
			case 0:
				
				g_Enemy[i].pos = pos;
				g_Enemy[i].spd = 60.0f;			// 移動スピードクリア
				g_Enemy[i].misairustanby = TRUE;
				g_Enemy[i].movemode = 0;
				g_Enemy[i].modelife = 180 + (rand() % 180);
				g_Enemy[i].hp = 100;
				g_Enemy[i].axisZ = zonevec();
				g_Enemy[i].hseido = XM_PI / 3000;
				g_Enemy[i].use = TRUE;
				return;
				break;

			case 1:
				
				g_Enemy[i].pos = pos;
				g_Enemy[i].spd = 60.0f;			// 移動スピードクリア
				g_Enemy[i].misairustanby = TRUE;
				g_Enemy[i].movemode = 0;
				g_Enemy[i].modelife = 180 + (rand() % 180);
				g_Enemy[i].hp = 100;
				g_Enemy[i].axisZ = zonevec();
				g_Enemy[i].hseido = XM_PI / 2500;
				g_Enemy[i].use = TRUE;
				return;
				break;


			case 2:
				
				g_Enemy[i].pos = pos;
				g_Enemy[i].spd = 60.0f;			// 移動スピードクリア
				g_Enemy[i].misairustanby = TRUE;
				g_Enemy[i].movemode = 0;
				g_Enemy[i].modelife = 180 + (rand() % 180);
				g_Enemy[i].hp = 100;
				g_Enemy[i].axisZ = zonevec();
				g_Enemy[i].hseido = XM_PI / 2000;
				g_Enemy[i].use = TRUE;
				return;
				break;




			default:
				break;
			}



			
		}
	}


}