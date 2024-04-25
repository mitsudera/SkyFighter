//=============================================================================
//
// �p�[�e�B�N������ [cloud.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCloud(void);
void UninitCloud(void);
void UpdateCloud(void);
void DrawCloud(void);

void SetCloud(XMFLOAT3 pos, float fSizeX, float fSizeY,float fSizeZ);
void SetColorCloud(int nIdxCloud, XMFLOAT4 col);
//void SetCloudHightX(int p1, int p2, int retu,float h);
//void SetCloudHightZ(int p1, int p2, int retu, float h,int pz,int hi);

void SetCloudHight1(int p1, int p2,int p3, int p4, int n, float h1, float h2, float h3, float h4);
void SetCloudHight2(int p1, int p2,int p3, int p4, int n, float h1, float h2, float h3, float h4);
float GetRandomHightP(int n);
float GetRandomHightPM(int n);


struct CLOUD
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scale;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	float			fSizeX;			// ��
	float			fSizeY;			// ����
	int				nLife;			// ����
	BOOL			use;			// �g�p���Ă��邩�ǂ���
	int				type;			// �^�C�v
};

