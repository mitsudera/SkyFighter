//=============================================================================
//
// モデル処理 [b36.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "b36.h"
#include "light.h"
#include"player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	MODEL_B36		"data/MODEL/b36.obj"			// 読み込むモデル名
//#define	MODEL_B36		"data/MODEL/cone.obj"			// 読み込むモデル名
//#define	MODEL_B36_PARTS	"data/MODEL/torus.obj"			// 読み込むモデル名


#define	MODEL_B36		"data/MODEL/b-36.obj"	// 読み込むモデル名
#define	MODEL_FUN		"data/MODEL/fun.obj"	// 読み込むモデル名
#define	MODEL_HOUDAI	"data/MODEL/houdai.obj"	// 読み込むモデル名
#define	MODEL_HOUTOU	"data/MODEL/houtou.obj"	// 読み込むモデル名



#define TEXTURE_MAX			(3)


#define	VALUE_MOVE			(2.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define B36_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define B36_OFFSET_Y		(7.0f)							// プレイヤーの足元をあわせる

#define MOTION_BLUR		(3)								//モーションブラーの数
#define FUN_SPEED		(XM_PI*0.2)					//プロペラの回転速度
#define FUN_MAX			(6)
#define CANON_MAX		(6)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static B36		g_B36;						// プレイヤー

static B36		g_Parts[B36_PARTS_MAX];		// プレイヤーのパーツ用


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
HRESULT InitB36(void)
{
	LoadModel(MODEL_B36, &g_B36.model);
	g_B36.load = TRUE;

	g_B36.pos = { 0.0f, B36_OFFSET_Y, 0.0f };
	g_B36.rot = { 0.0f, 0.0f, 0.0f };
	g_B36.scl = { 1.0f, 1.0f, 1.0f };

	g_B36.spd = 0.0f;			// 移動スピードクリア
	g_B36.size = B36_SIZE;	// 当たり判定の大きさ

	g_B36.use = FALSE;

	g_B36.axisZ = zonevec();

	g_B36.breaked = FALSE;


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



	MakeVertexCir1();
	MakeVertexCirRed1();

	ZeroMemory(&g_cirmat, sizeof(g_cirmat));
	g_cirmat.Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	ZeroMemory(&g_redmat, sizeof(g_redmat));
	g_redmat.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);



	// 階層アニメーション用の初期化処理
	g_B36.parent = NULL;			// 本体（親）なのでNULLを入れる

	// パーツの初期化
	for (int i = 0; i < B36_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Parts[i].breaked = FALSE;
		g_Parts[i].hp = 100;

		// 親子関係
		g_Parts[i].parent = &g_B36;		// ← ここに親のアドレスを入れる
	//	g_Parts[腕].parent= &g_B36;		// 腕だったら親は本体（プレイヤー）
	//	g_Parts[手].parent= &g_Paerts[腕];	// 指が腕の子供だった場合の例

		// パーツの読み込みはまだしていない
		g_Parts[i].load = 0;
	}

	g_Parts[0].use = FALSE;
	g_Parts[0].parent = &g_B36;	// 親をセット
	g_Parts[0].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[0].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Parts[0].model, &g_Parts[0].diffuse[0]);

	g_Parts[1].use = FALSE;
	g_Parts[1].parent = &g_B36;	// 親をセット
	g_Parts[1].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[1].model);
	GetModelDiffuse(&g_Parts[1].model, &g_Parts[1].diffuse[0]);


	g_Parts[2].use = FALSE;
	g_Parts[2].parent = &g_B36;	// 親をセット
	g_Parts[2].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[2].model);
	GetModelDiffuse(&g_Parts[2].model, &g_Parts[2].diffuse[0]);


	g_Parts[3].use = FALSE;
	g_Parts[3].parent = &g_B36;	// 親をセット
	g_Parts[3].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[3].model);
	GetModelDiffuse(&g_Parts[3].model, &g_Parts[3].diffuse[0]);


	g_Parts[4].use = FALSE;
	g_Parts[4].parent = &g_B36;	// 親をセット
	g_Parts[4].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[4].model);
	GetModelDiffuse(&g_Parts[4].model, &g_Parts[4].diffuse[0]);


	g_Parts[5].use = FALSE;
	g_Parts[5].parent = &g_B36;	// 親をセット
	g_Parts[5].load = 1;
	LoadModel(MODEL_FUN, &g_Parts[5].model);
	GetModelDiffuse(&g_Parts[5].model, &g_Parts[5].diffuse[0]);

	//g_Parts[6].use = FALSE;
	//g_Parts[6].parent = &g_B36;	// 親をセット
	//g_Parts[6].load = 1;
	//g_Parts[6].scl = { 0.5f,0.5f,0.5f };
	//LoadModel(MODEL_HOUDAI, &g_Parts[6].model);
	//GetModelDiffuse(&g_Parts[6].model, &g_Parts[6].diffuse[0]);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitB36(void)
{
	// モデルの解放処理
	if (g_B36.load)
	{
		UnloadModel(&g_B36.model);
		g_B36.load = FALSE;
	}


	// パーツの解放処理
	for (int i = 0; i < B36_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// パーツの解放処理
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateB36(void)
{
	CAMERA *camera = GetCamera();
	PLAYER* player = GetPlayer();

	XMFLOAT3 pl = player[0].pos;
	XMVECTOR plvec = XMLoadFloat3(&pl);
	XMVECTOR camvec = XMLoadFloat3(&camera->pos);


	for (int i = 0; i < B36_PARTS_MAX; i++)
	{

		g_Parts[i].wpos = XMFLOAT3(g_B36.pos.x+ g_Parts[i].pos.x*g_B36.scl.x, g_B36.pos.y+ g_Parts[i].pos.y*g_B36.scl.y, g_B36.pos.z+ g_Parts[i].pos.z*g_B36.scl.z);

		XMVECTOR envec = XMLoadFloat3(&g_Parts[i].wpos);
		XMVECTOR ep = envec - plvec;

		XMVECTOR eplen = XMVector3Length(ep);
		XMStoreFloat(&g_Parts[i].plegth, eplen);
		XMVECTOR ec = envec - camvec;
		ec = XMVector3Normalize(ec);
		XMStoreFloat3(&g_Parts[i].ec, ec);




	}





#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_B36.pos.z = g_B36.pos.x = 0.0f;
		g_B36.rot.y = g_B36.dir = 0.0f;
		g_B36.spd = 0.0f;
	}
#endif

	BOOL destroy = TRUE;
	for (int i = 0; i < B36_PARTS_MAX; i++)
	{
		if (g_Parts[i].breaked == FALSE)
		{
			destroy = FALSE;



			g_Parts[i].oldrot = g_Parts[i].rot.z;
			if (i < 3)
			{
				g_Parts[i].rot.z += FUN_SPEED;

			}
			else if (i >= 3 && i < 6)
			{
				g_Parts[i].rot.z -= FUN_SPEED;


			}
		}
	}

	if (destroy == TRUE)
	{
		g_B36.pos.y -= 100.0f;

		if (g_B36.pos.y<0.0f)
		{
			g_B36.use = FALSE;
		}

	}
	g_B36.pos.z += 60.0f;


#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("B36:↑ → ↓ ←　Space\n");
	PrintDebugProc("B36:X:%f Y:%f Z:%f\n", g_Parts[0].wpos.x,g_Parts[0].wpos.y,g_Parts[0].wpos.z);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawB36(void)
{
	CAMERA* camera = GetCamera();

	PLAYER* player = GetPlayer();
	if (g_B36.use == FALSE) return;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// まずは基本となるＢＯＤＹを描画する

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_B36.scl.x, g_B36.scl.y, g_B36.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_B36.rot.x, g_B36.rot.y + XM_PI, g_B36.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_B36.pos.x, g_B36.pos.y, g_B36.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	// 自分を描画したときにしようしたマトリクスを保存しておく
	XMStoreFloat4x4(&g_B36.mtxWorld, mtxWorld);


	// モデル描画
	DrawModel(&g_B36.model);



	// パーツの階層アニメーション
	for (int i = 0; i < B36_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();



		// スケールを反映
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ↑
			// g_B36.mtxWorldを指している
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
		if (g_Parts[i].use == FALSE) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		// モデル描画
		DrawModel(&g_Parts[i].model);

		for (int j = 0; j < MOTION_BLUR; j++)
		{


			if (g_Parts[i].breaked == FALSE)
			{


				// ワールドマトリックスの初期化
				mtxWorld = XMMatrixIdentity();



				mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

				float rotdif = g_Parts[i].rot.z - g_Parts[i].oldrot;
				float zrot = g_Parts[i].oldrot + (rotdif / MOTION_BLUR) * j;

				// 回転を反映
				mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, zrot);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

				mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

				if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
				{
					mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
					// ↑
					// g_B36.mtxWorldを指している
				}

				XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

				// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
				if (g_Parts[i].use == FALSE) continue;

				// ワールドマトリックスの設定
				SetWorldMatrix(&mtxWorld);

				//for (int j = 0; j < g_Parts[0].model.SubsetNum; j++)
				//{
				//	SetModelDiffuse(&g_Parts[0].model, j, XMFLOAT4(g_Parts[i].diffuse[0].x, g_Parts[i].diffuse[0].y, g_Parts[i].diffuse[0].z, 0.1f));
				//}

				//// モデル描画
				DrawModel(&g_Parts[i].model);


				//for (int j = 0; j < g_Parts[0].model.SubsetNum; j++)
				//{
				//	SetModelDiffuse(&g_Parts[0].model, j, g_Parts[i].diffuse[0]);
				//}

			}



							// αテスト設定
		// αテストを有効に
			SetAlphaTestEnable(TRUE);

			SetCullingMode(CULL_MODE_NONE);

			// ライティングを無効
			SetLightEnable(FALSE);


			if (player->target == i && player->targettype == 2)
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
			mtxTranslate = XMMatrixTranslation(camera[0].pos.x + g_Parts[i].ec.x * 50.0f, camera[0].pos.y + g_Parts[i].ec.y * 50.0f, camera[0].pos.z + g_Parts[i].ec.z * 50.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);




			// マテリアル設定
			SetMaterial(g_cirmat);
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);

			if (player->target == i && player->targettype == 2)
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



		}



	}


	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
B36 *GetB36(void)
{
	return &g_B36;
}

void SetB36(XMFLOAT3 pos)
{
	g_B36.pos = pos;
	g_B36.rot = { 0.0f, 0.0f, 0.0f };
	g_B36.scl = { 100.0f, 100.0f, 100.0f };

	g_B36.spd = 0.0f;			// 移動スピードクリア

	g_B36.use = TRUE;




	// 階層アニメーション用の初期化処理
	g_B36.parent = NULL;			// 本体（親）なのでNULLを入れる


	g_Parts[0].use = TRUE;
	g_Parts[0].pos = { 8.6f,-0.1f,3.5f };
	g_Parts[0].rot = { 0.0f,-0.05f,0.0f };
	g_Parts[0].breaked = FALSE;

	g_Parts[1].use = TRUE;
	g_Parts[1].pos = { 15.3f,-0.1f,3.5f };
	g_Parts[1].rot = { 0.0f,-0.05f,0.0f };
	g_Parts[1].breaked = FALSE;

	g_Parts[2].use = TRUE;
	g_Parts[2].pos = { 22.2f,-0.1f,3.5f };
	g_Parts[2].rot = { 0.0f,-0.05f,0.0f };
	g_Parts[2].breaked = FALSE;

	g_Parts[3].use = TRUE;
	g_Parts[3].pos = { -8.6f,-0.1f,3.5f };
	g_Parts[3].rot = { 0.0f,0.05f,0.0f };
	g_Parts[3].breaked = FALSE;

	g_Parts[4].use = TRUE;
	g_Parts[4].pos = { -15.3f,-0.1f,3.5f };
	g_Parts[4].rot = { 0.0f,0.05f,0.0f };
	g_Parts[4].breaked = FALSE;

	g_Parts[5].use = TRUE;
	g_Parts[5].pos = { -22.2f,-0.1f,3.5f };
	g_Parts[5].rot = { 0.0f,0.05f,0.0f };
	g_Parts[5].breaked = FALSE;

	//g_Parts[6].use = TRUE;
	//g_Parts[6].pos = { 0.0f,0.0f,0.0f };
	//g_Parts[6].rot = { 0.0f,0.05f,0.0f };
	//g_Parts[6].breaked = FALSE;






}

B36* GetB36Parts(void)
{
	return &g_Parts[0];
}

HRESULT MakeVertexCir1(void)
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
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight / 2, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight / 2, 0.0f);
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
HRESULT MakeVertexCirRed1(void)
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
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight / 2, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight / 2, 0.0f);
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
