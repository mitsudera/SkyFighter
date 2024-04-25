//=============================================================================
//
// パーティクル処理 [beffect1.h]
// Author : 
//
//====================================================BEFFECT1=========================
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBeffect1(void);
void UninitBeffect1(void);
void UpdateBeffect1(void);
void DrawBeffect1(void);

int SetParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife, int type);
void SetColorBeffect1(int nIdxBeffect1, XMFLOAT4 col);

//void SetBombEffect1(XMFLOAT3 pos);



struct BEFFECT1
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
