//=============================================================================
//
// ミサイルモデル処理 [misairu.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MISAIRU		(100)					// ミサイルの数

#define	MISAIRU_SIZE		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct MISAIRU
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	XMVECTOR			vec;
	bool				use;
	bool				load;
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	float				spd;				// 移動スピード
	float				size;				// 当たり判定の大きさ
	int muki;
	int move;
	int obj;//目標
	int targettype;
	int pore;//プレイヤーが目標かエネミーが目標か？p=0,e=1,none=-1
	int life;

	float hseido;

	XMFLOAT3 oldpos;
	XMMATRIX			mtxrot;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMisairu(void);
void UninitMisairu(void);
void UpdateMisairu(void);
void DrawMisairu(void);

MISAIRU* GetMisairu(void);

void SetMisairu(XMFLOAT3 pos, XMVECTOR vec, XMMATRIX rot, float spd, int obj, int pore);
