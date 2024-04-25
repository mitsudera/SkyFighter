//=============================================================================
//
// ���C�g���� [light.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************

struct SUN
{
	XMFLOAT3	pos;
	XMFLOAT3	Opos;
	XMVECTOR	rvec;
	XMFLOAT3	up;
	XMVECTOR	upvec;
	XMVECTOR	Direction;
	XMMATRIX	SunView;
	XMMATRIX	SunProj;
	float		rot;


};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitLight(void);
void UpdateLight(void);

void SetLightData(int index, LIGHT *light);
void SetFogData(FOG *fog);
//void SetShadowData(FOG *fog);
BOOL GetFogEnable(void);
//BOOL GetShadowEnable(void);

LIGHT *GetLightData(int index);

SUN GetSun(void);
XMMATRIX GetSunView(void);

void SetSunPos(XMFLOAT3 vec);
