//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct CAMERA
{
	XMFLOAT4X4			mtxView;		// ビューマトリックス
	XMFLOAT4X4			mtxInvView;		// ビューマトリックス
	XMFLOAT4X4			mtxProjection;	// プロジェクションマトリックス
	
	XMFLOAT3			pos;			// カメラの視点(位置)
	XMFLOAT3			at;				// カメラの注視点
	XMFLOAT3			up;				// カメラの上方向ベクトル
	XMFLOAT3			rot;			// カメラの回転
	XMVECTOR		axisX;
	XMVECTOR		axisY;
	XMVECTOR		axisZ;

	XMVECTOR		qtonX;		//X軸回転のクォータニオン
	XMVECTOR		qtonY;		//Y軸回転のクォータニオン
	XMVECTOR		qtonZ;		//Z軸回転のクォータニオン
	XMMATRIX		mtxrotx;		//回転行列
	XMMATRIX		mtxroty;		//回転行列
	XMMATRIX		mtxrotz;		//回転行列
	XMMATRIX		mtxrot;
	XMVECTOR		qtonrot;
	float				len;			// カメラの視点と注視点の距離

};



enum {
	TYPE_FULL_SCREEN,
	TYPE_LEFT_HALF_SCREEN,
	TYPE_RIGHT_HALF_SCREEN,
	TYPE_UP_HALF_SCREEN,
	TYPE_DOWN_HALF_SCREEN,
	TYPE_LU_QUOTER_SCREEN,
	TYPE_LD_QUOTER_SCREEN,
	TYPE_RU_QUOTER_SCREEN,
	TYPE_RD_QUOTER_SCREEN,
	TYPE_NONE,

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

CAMERA *GetCamera(void);

void SetViewPort(int type);
int GetViewPortType(void);

void SetCameraAT();

XMFLOAT4 mul4and4x4(XMFLOAT4 f4, XMFLOAT4X4 f44);

XMINT2 WorldToScreen(XMFLOAT4X4 wpos, XMFLOAT4 inpos);
