//=============================================================================
//
// モデル処理 [b36.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_B36		(1)					// プレイヤーの数

#define	B36_SIZE		(5.0f)				// 当たり判定の大きさ
#define B36_PARTS_MAX	(6)								// プレイヤーのパーツの数


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct B36
{
	XMFLOAT3		pos;		// ポリゴンの位置
	XMFLOAT3		rot;		// ポリゴンの向き(回転)
	XMFLOAT3		scl;		// ポリゴンの大きさ(スケール)
	float			plegth;
	XMFLOAT3		wpos;
	XMFLOAT3		ec;
	XMFLOAT4X4		mtxWorld;	// ワールドマトリックス

	int				hp;
	BOOL			load;
	DX11_MODEL		model;		// モデル情報

	float			spd;		// 移動スピード
	float			dir;		// 向き
	float			size;		// 当たり判定の大きさ
	BOOL			use;

	XMVECTOR		axisZ;

	float oldrot;

	BOOL			breaked;

	XMFLOAT4		diffuse[MODEL_MAX_MATERIAL];

	// 親は、NULL、子供は親のアドレスを入れる(セーブ＆ロードの時は↓ここ気をつける事)
	B36			*parent;	// 自分が親ならNULL、自分が子供なら親のb36アドレス

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitB36(void);
void UninitB36(void);
void UpdateB36(void);
void DrawB36(void);

B36 *GetB36(void);

void SetB36(XMFLOAT3 pos);
B36* GetB36Parts(void);
HRESULT MakeVertexCir1(void);
HRESULT MakeVertexCirRed1(void);
