//=============================================================================
//
// エネミーモデル処理 [biru.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "biru.h"
#include"player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BIRU			"data/MODEL/aoibiru.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define BIRU_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define BIRU_OFFSET_Y		(7.0f)						// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BIRU			g_Biru[MAX_BIRU];				// エネミー
int g_Biru_load = 0;
static DX11_MODEL model;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBiru(void)
{
	LoadModel(MODEL_BIRU, &model);

	for (int i = 0; i < MAX_BIRU; i++)
	{
		g_Biru[i].load = true;

		g_Biru[i].pos = XMFLOAT3(-100.0f + i * 30.0f, 0.0f, 100.0f*i);
		g_Biru[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Biru[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Biru[i].spd  = 0.0f;			// 移動スピードクリア
		g_Biru[i].size = BIRU_SIZE;	// 当たり判定の大きさ

		int muki = 0;
		int move = 1;





		g_Biru[i].use = true;		// true:生きてる

	}
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Biru[0].model, &g_Biru[0].diffuse[0]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBiru(void)
{

	for (int i = 0; i < MAX_BIRU; i++)
	{
		if (g_Biru[i].load)
		{
			UnloadModel(&g_Biru[i].model);
			g_Biru[i].load = false;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBiru(void)
{
	PLAYER* g_Player = GetPlayer();

	// エネミーを動かす場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_BIRU; i++)
	{
		if (g_Biru[i].use == true)		// このエネミーが使われている？
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
	//	for (int j = 0; j < g_Biru[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Biru[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// モデルの色を元に戻している
	//	for (int j = 0; j < g_Biru[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Biru[0].model, j, g_Biru[0].diffuse[j]);
	//	}
	//}
#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBiru(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BIRU; i++)
	{
		if (g_Biru[i].use == true)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Biru[i].scl.x, g_Biru[i].scl.y, g_Biru[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Biru[i].rot.x, g_Biru[i].rot.y + XM_PI, g_Biru[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Biru[i].pos.x, g_Biru[i].pos.y, g_Biru[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Biru[i].mtxWorld, mtxWorld);


			//// モデル描画
			//DrawModel(&model);
		}
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
BIRU *GetBiru()
{
	return &g_Biru[0];
}
