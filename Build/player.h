//=============================================================================
//
// ���f������ [player.h]
// Author : 
//
//=============================================================================
#pragma once
#include "model.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER		(1)					// �v���C���[�̐�

#define	PLAYER_SIZE		(5.0f)				// �����蔻��̑傫��

#define EFECT_MAX		(10)
#define EFECT_TIME_MAX		(10)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
	XMFLOAT3		pos;		// �|���S���̈ʒu
	XMFLOAT3		oldpos;		
	XMFLOAT3		rot;		// �|���S���̌���
	XMFLOAT3		scl;		// �|���S���̑傫��(�X�P�[��)
	XMFLOAT3		dir;
	XMVECTOR		axisX;
	XMVECTOR		axisY;
	XMVECTOR		axisZ;

	XMVECTOR		qtonX;		//X����]�̃N�H�[�^�j�I��
	XMVECTOR		qtonY;		//X����]�̃N�H�[�^�j�I��
	XMVECTOR		qtonZ;		//X����]�̃N�H�[�^�j�I��


	XMMATRIX		mtxrot;			//��]�s��
	XMMATRIX		mtxrotx;		//��]�s��
	XMMATRIX		mtxroty;		//��]�s��
	XMMATRIX		mtxrotz;		//��]�s��


	XMFLOAT4X4		mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	bool			load;
	DX11_MODEL		model;		// ���f�����

	float			spd;		// �ړ��X�s�[�h
	float			size;		// �����蔻��̑傫��
	int				shadowIdx;	// �e��Index
	bool			use;
	float			spdmax;		//�ō����x
	float			spdup;		//����
	float			spdupmax;		//����
	float			spdupvalue;		//�����x
	float			brake;		//�u���[�L
	PLAYER			*parent;
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	float			time;		// ���`��ԗp
	int				tblNo;		// �s���f�[�^�̃e�[�u���ԍ�
	int				tblMax;		// ���̃e�[�u���̃f�[�^��
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
	DX11_MODEL		model;		// ���f�����
	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	float			time[EFECT_TIME_MAX];		// ���`��ԗp
	int				tblNo;		// �s���f�[�^�̃e�[�u���ԍ�
	int				tblMax;		// ���̃e�[�u���̃f�[�^��
	XMFLOAT3		pos[EFECT_TIME_MAX];		// �|���S���̈ʒu
	XMFLOAT3		rot[EFECT_TIME_MAX];		// �|���S���̌���
	XMFLOAT3		scl[EFECT_TIME_MAX];		// �|���S���̑傫��(�X�P�[��)
	BOOL			use;		



};

//*****************************************************************************
// �v���g�^�C�v�錾
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
