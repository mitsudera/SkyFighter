//=============================================================================
//
// �J�������� [camera.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct CAMERA
{
	XMFLOAT4X4			mtxView;		// �r���[�}�g���b�N�X
	XMFLOAT4X4			mtxInvView;		// �r���[�}�g���b�N�X
	XMFLOAT4X4			mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	
	XMFLOAT3			pos;			// �J�����̎��_(�ʒu)
	XMFLOAT3			at;				// �J�����̒����_
	XMFLOAT3			up;				// �J�����̏�����x�N�g��
	XMFLOAT3			rot;			// �J�����̉�]
	XMVECTOR		axisX;
	XMVECTOR		axisY;
	XMVECTOR		axisZ;

	XMVECTOR		qtonX;		//X����]�̃N�H�[�^�j�I��
	XMVECTOR		qtonY;		//Y����]�̃N�H�[�^�j�I��
	XMVECTOR		qtonZ;		//Z����]�̃N�H�[�^�j�I��
	XMMATRIX		mtxrotx;		//��]�s��
	XMMATRIX		mtxroty;		//��]�s��
	XMMATRIX		mtxrotz;		//��]�s��
	XMMATRIX		mtxrot;
	XMVECTOR		qtonrot;
	float				len;			// �J�����̎��_�ƒ����_�̋���

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
// �v���g�^�C�v�錾
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
