#include"math.h"

XMMATRIX mtxrotX(float x)
{
	float sx = sinf(x);
	float cx = cosf(x);

	XMMATRIX mtxrotx = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cx,-sx,0.0f,
		0.0f,sx,cx,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};
	return mtxrotx;
}
XMMATRIX mtxrotY(float y)
{
	float sy = sinf(y);
	float cy = cosf(y);

	XMMATRIX mtxroty = {
		cy,0.0f,sy,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		-sy,0.0f,cy,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};
	return mtxroty;

}
XMMATRIX mtxrotZ(float z)
{
	float sz = sinf(z);
	float cz = cosf(z);

	XMMATRIX mtxrotz = {
		cz,-sz,0.0f,0.0f,
		sz,cz,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};
	return mtxrotz;

}

XMMATRIX mtxrotZYX(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(cy * cz),(sx * sy * cz - cx * sz),(cx * sy * cz + sx * sz),0.0f,
		(cy * sz),(sx * sy * sz + cx * cz),(cx * sy * sz - sx * cz),0.0f,
		(-sy),(sx * cy),(cx * cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}

XMMATRIX mtxrotZXY(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(-sx * sy * sz + cy * cz),(-cx * sz),(sx * cy * sz + sy * cz),0.0f,
		(sx * sy * cz + cy * sz),(cx * cz),(-sx * cy * cz + sy * sz),0.0f,
		(-cx * sy),(sx),(cx * cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}
XMMATRIX mtxrotYXZ(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(sx * sy * sz + cy * cz),(sx * sy * cz - cy * sz),(cx * sy),0.0f,
		(cx * sz),(cx * cz),(-sx),0.0f,
		(sx * cy * sz - sy * cz),(sx * cy * cz + sy * sz),(cx * cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}

XMMATRIX mtxrotXYZ(float x, float y, float z)
{
	XMMATRIX mtxrotzyx;
	float sx = sinf(x);
	float cx = cosf(x);
	float sy = sinf(y);
	float cy = cosf(y);
	float sz = sinf(z);
	float cz = cosf(z);



	mtxrotzyx = {
		(cy * cz),(-cy * sz),(sy),0.0f,
		(sx * sy * cz + cx * sz),(-sx * sy * sz + cx * cz),(-sx * cy),0.0f,
		(-cx * sy * cz + sx * sz),(cx * sy * sz + sx * cz),(cx * cy),0.0f,
		0.0f,0.0f,0.0f,1.0f,

	};

	return mtxrotzyx;
}






XMVECTOR xonevec(void)
{
	XMFLOAT3 x = { 1.0f,0.0f,0.0f };

	XMVECTOR vec = XMLoadFloat3(&x);
	return vec;
}

XMVECTOR yonevec(void)
{
	XMFLOAT3 y = { 0.0f,1.0f,0.0f };

	XMVECTOR vec = XMLoadFloat3(&y);
	return vec;
}

XMVECTOR zonevec(void)
{
	XMFLOAT3 z = { 0.0f,0.0f,1.0f };

	XMVECTOR vec = XMLoadFloat3(&z);
	return vec;
}

XMFLOAT3 FbxDouble3ConvertToXMFLOAT3(FbxDouble3 fbxd3)
{
	XMFLOAT3 xmfl3;

	xmfl3.x = (float)fbxd3[0];
	xmfl3.y = (float)fbxd3[1];
	xmfl3.z = (float)fbxd3[2];

	return xmfl3;
}

XMMATRIX FbxMatrixConvertToXMMATRIX(FbxMatrix fbxmat)
{
	XMMATRIX xmat;
	XMFLOAT4X4 matrix;
	matrix._11 = (float)fbxmat[0][0];
	matrix._12 = (float)fbxmat[0][1];
	matrix._13 = (float)fbxmat[0][2];
	matrix._14 = (float)fbxmat[0][3];

	matrix._21 = (float)fbxmat[1][0];
	matrix._22 = (float)fbxmat[1][1];
	matrix._23 = (float)fbxmat[1][2];
	matrix._24 = (float)fbxmat[1][3];

	matrix._31 = (float)fbxmat[2][0];
	matrix._32 = (float)fbxmat[2][1];
	matrix._33 = (float)fbxmat[2][2];
	matrix._34 = (float)fbxmat[2][3];

	matrix._41 = (float)fbxmat[3][0];
	matrix._42 = (float)fbxmat[3][1];
	matrix._43 = (float)fbxmat[3][2];
	matrix._44 = (float)fbxmat[3][3];

	xmat = XMLoadFloat4x4(&matrix);
	return xmat;

}

XMFLOAT3 XMFLOAT3Normalize(XMFLOAT3 p)
{
	XMVECTOR v = XMLoadFloat3(&p);

	v = XMVector3Normalize(v);

	XMFLOAT3 ans;

	XMStoreFloat3(&ans, v);

	return ans;

}
