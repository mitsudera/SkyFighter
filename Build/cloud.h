//=============================================================================
//
// パーティクル処理 [cloud.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCloud(void);
void UninitCloud(void);
void UpdateCloud(void);
void DrawCloud(void);

void SetCloud(XMFLOAT3 pos, float fSizeX, float fSizeY,float fSizeZ);
void SetColorCloud(int nIdxCloud, XMFLOAT4 col);
//void SetCloudHightX(int p1, int p2, int retu,float h);
//void SetCloudHightZ(int p1, int p2, int retu, float h,int pz,int hi);

void SetCloudHight1(int p1, int p2,int p3, int p4, int n, float h1, float h2, float h3, float h4);
void SetCloudHight2(int p1, int p2,int p3, int p4, int n, float h1, float h2, float h3, float h4);
float GetRandomHightP(int n);
float GetRandomHightPM(int n);


struct CLOUD
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

