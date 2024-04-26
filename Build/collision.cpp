//=============================================================================
//
// 当たり判定処理 [collision.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************


//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
BOOL CollisionBB(XMFLOAT3 mpos, float mw, float mh,
	XMFLOAT3 ypos, float yw, float yh)
{
	BOOL ans = FALSE;	// 外れをセットしておく

	// 座標が中心点なので計算しやすく半分にしている
	mw /= 2;
	mh /= 2;
	yw /= 2;
	yh /= 2;

	// バウンディングボックス(BB)の処理
	if ((mpos.x + mw > ypos.x - yw) &&
		(mpos.x - mw < ypos.x + yw) &&
		(mpos.y + mh > ypos.y - yh) &&
		(mpos.y - mh < ypos.y + yh))
	{
		// 当たった時の処理
		ans = TRUE;
	}

	return ans;
}

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらTRUE
//=============================================================================
BOOL CollisionBC(XMFLOAT3 pos1, XMFLOAT3 pos2, float r1, float r2)
{
	BOOL ans = FALSE;						// 外れをセットしておく

	float len = (r1 + r2) * (r1 + r2);		// 半径を2乗した物
	XMVECTOR temp = XMLoadFloat3(&pos1) - XMLoadFloat3(&pos2);
	temp = XMVector3LengthSq(temp);			// 2点間の距離（2乗した物）
	float lenSq = 0.0f;
	XMStoreFloat(&lenSq, temp);

	// 半径を2乗した物より距離が短い？
	if (len > lenSq)
	{
		ans = TRUE;	// 当たっている
	}

	return ans;
}
















//ひし形と一点の当たり判定
//引数はひし形の中心点と幅と高さと点の座標
BOOL CollisionDiaDot(XMFLOAT3 mpos, float mw, float mh,XMFLOAT3 ypos)
{
	BOOL ans = FALSE;	// 外れをセットしておく
		// 座標が中心点なので計算しやすく半分にしている
	mw /= 2;
	mh /= 2;
	


	



	XMFLOAT3 A;
	XMFLOAT3 B;
	XMFLOAT3 C;
	XMFLOAT3 D;
	
	XMFLOAT3 AB;
	XMFLOAT3 BC;
	XMFLOAT3 CD;
	XMFLOAT3 DA;



	
	A.x = (mpos.x);
	A.y = (mpos.y - mh);
	
	B.x = (mpos.x-mw);
	B.y = (mpos.y);
	
	C.x = (mpos.x);
	C.y = (mpos.y + mh);
	
	D.x = (mpos.x+mw);
	D.y = (mpos.y);
	

	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	BC.x = C.x - B.x;
	BC.y = C.y - B.y;
	CD.x = D.x - C.x;
	CD.y = D.y - C.y;
	DA.x = A.x - D.x;
	DA.y = A.y - D.y;

	XMFLOAT3 AO;
	XMFLOAT3 BO;
	XMFLOAT3 CO;
	XMFLOAT3 DO;

	AO.x = ypos.x - A.x;
	AO.y = ypos.y - A.y;
	BO.x = ypos.x - B.x;
	BO.y = ypos.y - B.y;
	CO.x = ypos.x - C.x;
	CO.y = ypos.y - C.y;
	DO.x = ypos.x - D.x;
	DO.y = ypos.y - D.y;

	//z軸成分を求める
	float Az, Bz, Cz, Dz;
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;




	//全てプラスか全てマイナスなら中にいる
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}

	return ans;
}


//四角形(全ての頂点の座標が180度以内)と点の当たり判定
//引数は四角形の四つの頂点と点の座標
BOOL CollisionFreeBox(XMFLOAT3 A,XMFLOAT3 B,XMFLOAT3 C,XMFLOAT3 D, XMFLOAT3 ypos)
{
	BOOL ans = FALSE;	// 外れをセットしておく
	


	



	
	XMFLOAT3 AB;
	XMFLOAT3 BC;
	XMFLOAT3 CD;
	XMFLOAT3 DA;



	
	

	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	BC.x = C.x - B.x;
	BC.y = C.y - B.y;
	CD.x = D.x - C.x;
	CD.y = D.y - C.y;
	DA.x = A.x - D.x;
	DA.y = A.y - D.y;

	XMFLOAT3 AO;
	XMFLOAT3 BO;
	XMFLOAT3 CO;
	XMFLOAT3 DO;

	AO.x = ypos.x - A.x;
	AO.y = ypos.y - A.y;
	BO.x = ypos.x - B.x;
	BO.y = ypos.y - B.y;
	CO.x = ypos.x - C.x;
	CO.y = ypos.y - C.y;
	DO.x = ypos.x - D.x;
	DO.y = ypos.y - D.y;

	//z軸成分を求める
	float Az, Bz, Cz, Dz;
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;




	//全てプラスか全てマイナスなら中にいる
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}

	return ans;
}
//四角形(全ての頂点の座標が180度以内)同士の当たり判定
//引数は四角形1と2の四つの頂点座標
BOOL CollisionFreeBox(XMFLOAT3 A1,XMFLOAT3 B1,XMFLOAT3 C1,XMFLOAT3 D1,XMFLOAT3 A2,XMFLOAT3 B2,XMFLOAT3 C2,XMFLOAT3 D2)
{
	BOOL ans = FALSE;	// 外れをセットしておく
	
	//頂点のどれかが当たっていれば当たり

	XMFLOAT3 AB;
	XMFLOAT3 BC;
	XMFLOAT3 CD;
	XMFLOAT3 DA;
	float Az, Bz, Cz, Dz;

	XMFLOAT3 AO;
	XMFLOAT3 BO;
	XMFLOAT3 CO;
	XMFLOAT3 DO;

	AB.x = B1.x - A1.x;
	AB.y = B1.y - A1.y;
	BC.x = C1.x - B1.x;
	BC.y = C1.y - B1.y;
	CD.x = D1.x - C1.x;
	CD.y = D1.y - C1.y;
	DA.x = A1.x - D1.x;
	DA.y = A1.y - D1.y;


	//頂点A

	AO.x = A2.x - A1.x;
	AO.y = A2.y - A1.y;
	BO.x = A2.x - B1.x;
	BO.y = A2.y - B1.y;
	CO.x = A2.x - C1.x;
	CO.y = A2.y - C1.y;
	DO.x = A2.x - D1.x;
	DO.y = A2.y - D1.y;

	//z軸成分を求める
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;
	//全てプラスか全てマイナスなら中にいる
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}


	//頂点B
	AB.x = B1.x - A1.x;
	AB.y = B1.y - A1.y;
	BC.x = C1.x - B1.x;
	BC.y = C1.y - B1.y;
	CD.x = D1.x - C1.x;
	CD.y = D1.y - C1.y;
	DA.x = A1.x - D1.x;
	DA.y = A1.y - D1.y;

	AO.x = B2.x - A1.x;
	AO.y = B2.y - A1.y;
	BO.x = B2.x - B1.x;
	BO.y = B2.y - B1.y;
	CO.x = B2.x - C1.x;
	CO.y = B2.y - C1.y;
	DO.x = B2.x - D1.x;
	DO.y = B2.y - D1.y;

	//z軸成分を求める
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;

	//全てプラスか全てマイナスなら中にいる
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}
	//頂点C
	AB.x = B1.x - A1.x;
	AB.y = B1.y - A1.y;
	BC.x = C1.x - B1.x;
	BC.y = C1.y - B1.y;
	CD.x = D1.x - C1.x;
	CD.y = D1.y - C1.y;
	DA.x = A1.x - D1.x;
	DA.y = A1.y - D1.y;


	AO.x = C2.x - A1.x;
	AO.y = C2.y - A1.y;
	BO.x = C2.x - B1.x;
	BO.y = C2.y - B1.y;
	CO.x = C2.x - C1.x;
	CO.y = C2.y - C1.y;
	DO.x = C2.x - D1.x;
	DO.y = C2.y - D1.y;

	//z軸成分を求める
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;

	//全てプラスか全てマイナスなら中にいる
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}
	//頂点D
	AB.x = B1.x - A1.x;
	AB.y = B1.y - A1.y;
	BC.x = C1.x - B1.x;
	BC.y = C1.y - B1.y;
	CD.x = D1.x - C1.x;
	CD.y = D1.y - C1.y;
	DA.x = A1.x - D1.x;
	DA.y = A1.y - D1.y;


	AO.x = D2.x - A1.x;
	AO.y = D2.y - A1.y;
	BO.x = D2.x - B1.x;
	BO.y = D2.y - B1.y;
	CO.x = D2.x - C1.x;
	CO.y = D2.y - C1.y;
	DO.x = D2.x - D1.x;
	DO.y = D2.y - D1.y;

	//z軸成分を求める
	Az = AB.x * AO.y - AO.x * AB.y;
	Bz = BC.x * BO.y - BO.x * BC.y;
	Cz = CD.x * CO.y - CO.x * CD.y;
	Dz = DA.x * DO.y - DO.x * DA.y;

	//全てプラスか全てマイナスなら中にいる
	if (Az<0&&Bz<0&&Cz<0&&Dz<0)
	{
		ans = TRUE;

	}
	if (Az>0&&Bz>0&&Cz>0&&Dz>0)
	{
		ans = TRUE;

	}

	return ans;
}

//=============================================================================
// 内積(dot)
//=============================================================================
float dotProduct(XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	float ans = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
#else
	// ダイレクトＸでは、、、
	XMVECTOR temp = XMVector3Dot(*v1, *v2);
	float ans = 0.0f;
	XMStoreFloat(&ans, temp);
#endif

	return(ans);
}


//=============================================================================
// 外積(cross)
//=============================================================================
void crossProduct(XMVECTOR* ret, XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	ret->x = v1->y * v2->z - v1->z * v2->y;
	ret->y = v1->z * v2->x - v1->x * v2->z;
	ret->z = v1->x * v2->y - v1->y * v2->x;
#else
	// ダイレクトＸでは、、、
	* ret = XMVector3Cross(*v1, *v2);
#endif

}


//=============================================================================
// レイキャスト
// p0, p1, p2　ポリゴンの３頂点
// pos0 始点
// pos1 終点
// hit　交点の返却用
// normal 法線ベクトルの返却用
// 当たっている場合、TRUEを返す
//=============================================================================
BOOL RayCast(XMFLOAT3 xp0, XMFLOAT3 xp1, XMFLOAT3 xp2, XMFLOAT3 xpos0, XMFLOAT3 xpos1, XMFLOAT3* hit, XMFLOAT3* normal)
{
	XMVECTOR	p0 = XMLoadFloat3(&xp0);
	XMVECTOR	p1 = XMLoadFloat3(&xp1);
	XMVECTOR	p2 = XMLoadFloat3(&xp2);
	XMVECTOR	pos0 = XMLoadFloat3(&xpos0);
	XMVECTOR	pos1 = XMLoadFloat3(&xpos1);

	XMVECTOR	nor;	// ポリゴンの法線
	XMVECTOR	vec1;
	XMVECTOR	vec2;
	float		d1, d2;

	{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
		vec1 = p1 - p0;
		vec2 = p2 - p0;
		crossProduct(&nor, &vec2, &vec1);
		nor = XMVector3Normalize(nor);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを１にしている)
		XMStoreFloat3(normal, nor);			// 求めた法線を入れておく
	}

	// ポリゴン平面と線分の内積とって衝突している可能性を調べる（鋭角なら＋、鈍角ならー、直角なら０）
	vec1 = pos0 - p0;
	vec2 = pos1 - p0;
	{	// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
		d1 = dotProduct(&vec1, &nor);
		d2 = dotProduct(&vec2, &nor);
		if (((d1 * d2) > 0.0f) || (d1 == 0 && d2 == 0))
		{
			// 当たっている可能性は無い
			return(FALSE);
		}
	}


	{	// ポリゴンと線分の交点を求める
		d1 = (float)fabs(d1);	// 絶対値を求めている
		d2 = (float)fabs(d2);	// 絶対値を求めている
		float a = d1 / (d1 + d2);							// 内分比

		XMVECTOR	vec3 = (1 - a) * vec1 + a * vec2;		// p0から交点へのベクトル
		XMVECTOR	p3 = p0 + vec3;							// 交点
		XMStoreFloat3(hit, p3);								// 求めた交点を入れておく

		{	// 求めた交点がポリゴンの中にあるか調べる

			// ポリゴンの各辺のベクトル
			XMVECTOR	v1 = p1 - p0;
			XMVECTOR	v2 = p2 - p1;
			XMVECTOR	v3 = p0 - p2;

			// 各頂点と交点とのベクトル
			XMVECTOR	v4 = p3 - p1;
			XMVECTOR	v5 = p3 - p2;
			XMVECTOR	v6 = p3 - p0;

			// 外積で各辺の法線を求めて、ポリゴンの法線との内積をとって符号をチェックする
			XMVECTOR	n1, n2, n3;

			crossProduct(&n1, &v4, &v1);
			if (dotProduct(&n1, &nor) < 0.0f) return(FALSE);	// 当たっていない

			crossProduct(&n2, &v5, &v2);
			if (dotProduct(&n2, &nor) < 0.0f) return(FALSE);	// 当たっていない

			crossProduct(&n3, &v6, &v3);
			if (dotProduct(&n3, &nor) < 0.0f) return(FALSE);	// 当たっていない
		}
	}

	return(TRUE);	// 当たっている！(hitには当たっている交点が入っている。normalには法線が入っている)
}

BOOL CollisionRaySphire(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 center ,float r)
{




	BOOL ans = FALSE;
	XMVECTOR p1 = XMLoadFloat3(&pos1);
	XMVECTOR p2 = XMLoadFloat3(&pos2);
	XMVECTOR cp = XMLoadFloat3(&center);

	XMVECTOR p1c = cp - p1;
	XMVECTOR p2c = cp - p2;
	float p1l;
	float p2l;
	XMStoreFloat(&p1l, XMVector3Length(p1c));
	XMStoreFloat(&p2l, XMVector3Length(p2c));

	if (p1l < r || p2l < r)
	{
		return TRUE;
	}

	XMVECTOR lv = XMVector3LinePointDistance(p1,p2,cp);
	float length;
	XMStoreFloat(&length, lv);
	if (length<r)
	{
		return TRUE;
	}

	return FALSE;

}




