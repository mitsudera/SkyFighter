#pragma once
#include "CoreMinimal.h"
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

XMFLOAT3 FbxDouble3ConvertToXMFLOAT3(FbxDouble3 fbxd3);

XMMATRIX FbxMatrixConvertToXMMATRIX(FbxMatrix fbxmat);

XMFLOAT3 XMFLOAT3Normalize(XMFLOAT3 p);
