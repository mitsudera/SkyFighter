//=============================================================================
//
// エネミーモデル処理 [sky.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "sky.h"
#include"player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_SKY			"data/MODEL/sky1.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define SKY_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define SKY_OFFSET_Y		(7.0f)						// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static SKY			g_Sky[MAX_SKY];				// エネミー
int g_Sky_load = 0;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSky(void)
{
	for (int i = 0; i < MAX_SKY; i++)
	{
		LoadModel(MODEL_SKY, &g_Sky[i].model);
		g_Sky[i].load = true;

		g_Sky[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Sky[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Sky[i].scl = XMFLOAT3(10000000.0f, 10000000.0f, 10000000.0f);

		g_Sky[i].spd  = 0.0f;			// 移動スピードクリア
		g_Sky[i].size = SKY_SIZE;	// 当たり判定の大きさ

		int muki = 0;
		int move = 1;


		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Sky[0].model, &g_Sky[0].diffuse[0]);




		g_Sky[i].use = true;		// true:生きてる

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSky(void)
{

	for (int i = 0; i < MAX_SKY; i++)
	{
		if (g_Sky[i].load)
		{
			UnloadModel(&g_Sky[i].model);
			g_Sky[i].load = false;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSky(void)
{
	PLAYER* g_Player = GetPlayer();

	// エネミーを動かす場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_SKY; i++)
	{
		if (g_Sky[i].use == true)		// このエネミーが使われている？
		{								// Yes
			
			switch (i)
			{
			case 0:	
				break;

			case 1:
				break;

			case 2:
				break;

			default:
				break;
			}

		}
	}

	//cnt += 1;


#ifdef _DEBUG

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	// モデルの色を変更できるよ！半透明にもできるよ。
	//	for (int j = 0; j < g_Sky[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Sky[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// モデルの色を元に戻している
	//	for (int j = 0; j < g_Sky[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Sky[0].model, j, g_Sky[0].diffuse[j]);
	//	}
	//}
#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSky(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_SKY; i++)
	{
		if (g_Sky[i].use == false) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Sky[i].scl.x, g_Sky[i].scl.y, g_Sky[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Sky[i].rot.x, g_Sky[i].rot.y+XM_PI , g_Sky[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Sky[i].pos.x, g_Sky[i].pos.y, g_Sky[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Sky[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Sky[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
SKY *GetSky()
{
	return &g_Sky[0];
}
