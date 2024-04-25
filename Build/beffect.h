//=============================================================================
//
// パーティクル処理 [beffect.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBeffect(void);
void UninitBeffect(void);
void UpdateBeffect(void);
void DrawBeffect(void);

int SetBeffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife, int type);
void SetColorBeffect(int nIdxBeffect, XMFLOAT4 col);

void SetBombEffect(XMFLOAT3 pos);



struct BEFFECT
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		rot;			// 回転
	XMFLOAT3		scale;			// スケール
	XMFLOAT3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	float			fSizeX;			// 幅
	float			fSizeY;			// 高さ
	int				nLife;			// 寿命
	BOOL			use;			// 使用しているかどうか
	int				type;			// タイプ
};
