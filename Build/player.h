//=============================================================================
//
// モデル処理 [player.h]
// Author : 
//
//=============================================================================
#pragma once
#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER		(1)					// プレイヤーの数

#define	PLAYER_SIZE		(5.0f)				// 当たり判定の大きさ

#define EFECT_MAX		(10)
#define EFECT_TIME_MAX		(10)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PLAYER
{
	XMFLOAT3		pos;		// ポリゴンの位置
	XMFLOAT3		oldpos;		
	XMFLOAT3		rot;		// ポリゴンの向き
	XMFLOAT3		scl;		// ポリゴンの大きさ(スケール)
	XMFLOAT3		dir;
	XMVECTOR		axisX;
	XMVECTOR		axisY;
	XMVECTOR		axisZ;

	XMVECTOR		qtonX;		//X軸回転のクォータニオン
	XMVECTOR		qtonY;		//X軸回転のクォータニオン
	XMVECTOR		qtonZ;		//X軸回転のクォータニオン


	XMMATRIX		mtxrot;			//回転行列
	XMMATRIX		mtxrotx;		//回転行列
	XMMATRIX		mtxroty;		//回転行列
	XMMATRIX		mtxrotz;		//回転行列


	XMFLOAT4X4		mtxWorld;	// ワールドマトリックス
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	bool			load;
	DX11_MODEL		model;		// モデル情報

	float			spd;		// 移動スピード
	float			size;		// 当たり判定の大きさ
	int				shadowIdx;	// 影のIndex
	bool			use;
	float			spdmax;		//最高速度
	float			spdup;		//加速
	float			spdupmax;		//加速
	float			spdupvalue;		//加速度
	float			brake;		//ブレーキ
	PLAYER			*parent;
	// 階層アニメーション用のメンバー変数
	float			time;		// 線形補間用
	int				tblNo;		// 行動データのテーブル番号
	int				tblMax;		// そのテーブルのデータ数
	BOOL			open;
	BOOL			misairustanby[2];
	int			misairustanbylife[2];
	int				target;
	int				targettype;
	int				hp;

};
struct ENGINE_EFECT
{
	int efecthow;
	PLAYER* parent;
	bool			load;
	DX11_MODEL		model;		// モデル情報
	// 階層アニメーション用のメンバー変数
	float			time[EFECT_TIME_MAX];		// 線形補間用
	int				tblNo;		// 行動データのテーブル番号
	int				tblMax;		// そのテーブルのデータ数
	XMFLOAT3		pos[EFECT_TIME_MAX];		// ポリゴンの位置
	XMFLOAT3		rot[EFECT_TIME_MAX];		// ポリゴンの向き
	XMFLOAT3		scl[EFECT_TIME_MAX];		// ポリゴンの大きさ(スケール)
	BOOL			use;		



};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);




XMMATRIX mtxrotX(float x);
XMMATRIX mtxrotY(float y);
XMMATRIX mtxrotZ(float z);


XMMATRIX mtxrotZYX(float x, float y, float z);
XMMATRIX mtxrotZXY(float x, float y, float z);
XMMATRIX mtxrotYXZ(float x, float y, float z);

XMMATRIX mtxrotXYZ(float x, float y, float z);

XMVECTOR xonevec(void);
XMVECTOR yonevec(void);
XMVECTOR zonevec(void);

void raunch(void);
