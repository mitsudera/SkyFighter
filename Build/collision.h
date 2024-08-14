//=============================================================================
//
// 当たり判定処理 [collision.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

float dotProduct(XMVECTOR* v1, XMVECTOR* v2);
void crossProduct(XMVECTOR* ret, XMVECTOR* v1, XMVECTOR* v2);
BOOL RayCast(XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 pos0, XMFLOAT3 pos1, XMFLOAT3* hit, XMFLOAT3* normal);

BOOL CollisionPointSphere(XMFLOAT3 point, XMFLOAT3 center, float r);
BOOL CollisionPointCapsule(XMFLOAT3 point, XMFLOAT3 pos1, XMFLOAT3 pos2,float r);

BOOL CollisionLineSphere(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 center, float r);
BOOL CollisionLineCapsule(XMFLOAT3 lp1, XMFLOAT3 lp2, XMFLOAT3 cp1, XMFLOAT3 cp2,float r);

BOOL CollisionSphereSphere(XMFLOAT3 center1, float r1, XMFLOAT3 center2, float r2);
BOOL CollisionSphereCapsule(XMFLOAT3 center1, float r1, XMFLOAT3 cp1, XMFLOAT3 cp2,float r2);

BOOL CollisionCapsuleCapsule(XMFLOAT3 cp1_1, XMFLOAT3 cp1_2,float r1, XMFLOAT3 cp2_1, XMFLOAT3 cp2_2 ,float r2);

void ClosestPtSegmentSegment(XMVECTOR p1, XMVECTOR d1, XMVECTOR p2, XMVECTOR d2, float& t1, float& t2, XMVECTOR& c1, XMVECTOR& c2);



