//=============================================================================
//
// ���f������ [enemy.h]
// Author : 
//
//=============================================================================
#pragma once
#include "model.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY		(20)					// �v���C���[�̐�

#define	ENEMY_SIZE		(5.0f)				// �����蔻��̑傫��

#define EFECT_MAX		(10)
#define EFECT_TIME_MAX		(10)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ENEMY
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
	float			spdupvalue;		//�����x
	float			brake;		//�u���[�L

	BOOL			misairustanby;
	int				misairuwait;

	XMFLOAT3 ingrot;

	int hp;


	int backcnt;

	XMFLOAT3 ep;//�v���C���[�Ƃ̋���
	XMFLOAT3 ec;//�v���C���[�Ƃ̋���
	XMMATRIX eprot;

	int movemode;//0�v���C���[�Ƀz�[�~���O�A1���i�A2�A�����_���ȃ^�[�Q�b�g�Ƀz�[�~���O
	int modelife;
	int htx;
	int htz;
	float plegth;
	BOOL target;//���b�N�I������Ă��邩�H
	float hseido;//���񐫔\

};

struct RED_CORN
{
	bool			load;
	DX11_MODEL		model;		// ���f�����


};

struct TORUS
{
	bool			load;
	DX11_MODEL		model;		// ���f�����


};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);

HRESULT MakeVertexCir(void);
HRESULT MakeVertexCirRed(void);
void SetEnemy(int Lv);
