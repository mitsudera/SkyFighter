//=============================================================================
//
// モデル処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once
#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY		(20)					// プレイヤーの数

#define	ENEMY_SIZE		(5.0f)				// 当たり判定の大きさ

#define EFECT_MAX		(10)
#define EFECT_TIME_MAX		(10)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY
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
	float			spdupvalue;		//加速度
	float			brake;		//ブレーキ

	BOOL			misairustanby;
	int				misairuwait;

	XMFLOAT3 ingrot;

	int hp;


	int backcnt;

	XMFLOAT3 ep;//プレイヤーとの距離
	XMFLOAT3 ec;//プレイヤーとの距離
	XMMATRIX eprot;

	int movemode;//0プレイヤーにホーミング、1直進、2、ランダムなターゲットにホーミング
	int modelife;
	int htx;
	int htz;
	float plegth;
	BOOL target;//ロックオンされているか？
	float hseido;//旋回性能

};

struct RED_CORN
{
	bool			load;
	DX11_MODEL		model;		// モデル情報


};

struct TORUS
{
	bool			load;
	DX11_MODEL		model;		// モデル情報


};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);

HRESULT MakeVertexCir(void);
HRESULT MakeVertexCirRed(void);
void SetEnemy(int Lv);
