//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include"player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


#define	SUN_H	(1000.0f)
#define SUN_ROT	(XM_PI/18000000)
//#define SUN_ROT	(0)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];

static FOG		g_Fog;

static BOOL		g_FogEnable = false;


static BOOL		g_ShadowEnable = false;


static SUN		g_Sun;

//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{
	PLAYER *player = GetPlayer();

	g_Sun.pos = XMFLOAT3(0.0f, SUN_H, 0.0f);
	g_Sun.Opos = player->pos;
	g_Sun.up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	g_Sun.Direction = XMLoadFloat3(&g_Sun.pos);
	g_Sun.Direction = XMLoadFloat3(&g_Sun.Opos) - g_Sun.Direction;
	g_Sun.Direction = XMVector3Normalize(g_Sun.Direction);
	g_Sun.upvec = XMLoadFloat3(&g_Sun.up);
	XMFLOAT3 vec = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_Sun.rvec = XMVector3Normalize(XMLoadFloat3(&vec));
	//g_Sun.rvec = zonevec();
	g_Sun.rot = 0.0f;
		//���C�g������
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		g_Light[i].Direction = XMFLOAT3( 0.0f, 0.0f, -1.0f );
		g_Light[i].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		g_Light[i].Ambient   = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		g_Light[i].Attenuation = 100.0f;	// ��������
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ���C�g�̃^�C�v
		g_Light[i].Enable = false;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	// ���s�����̐ݒ�i���E���Ƃ炷���j
	g_Light[0].Direction = XMFLOAT3(g_Sun.Direction.m128_f32[0], g_Sun.Direction.m128_f32[1], g_Sun.Direction.m128_f32[2]);		// ���̌���
	//g_Light[0].Direction = XMFLOAT3( player[0].pos.x-300.0f, player[0].pos.y -300.0f, player[0].pos.z+300.0f );		// ���̌���
	g_Light[0].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );	// ���̐F
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					// ���s����
	g_Light[0].Enable = true;									// ���̃��C�g��ON
	SetLight(0, &g_Light[0]);									// ����Őݒ肵�Ă���



	// �|�C���g���C�g�̐ݒ�i���d���j
	g_Light[1].Position = XMFLOAT3(0.0f, 0.0f, 100.0f);

	g_Light[1].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );	// ���̐F
	g_Light[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	g_Light[1].Attenuation = 40.0f;	// ��������
	g_Light[1].Type = LIGHT_TYPE_POINT;					// ���s����
	g_Light[1].Enable = true;									// ���̃��C�g��ON
	SetLight(1, &g_Light[1]);									// ����Őݒ肵�Ă���



	// �t�H�O�̏������i���̌��ʁj
	g_Fog.FogStart = 100.0f;									// ���_���炱�̋��������ƃt�H�O��������n�߂�
	g_Fog.FogEnd   = 2500000.0f;									// �����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
	g_Fog.FogColor = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );		// �t�H�O�̐F
	SetFog(&g_Fog);
	SetFogEnable(g_FogEnable);				// ���̏ꏊ���`�F�b�N���� shadow


	//// �e�̏�����
	//g_Shadow.ShadowColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );		// �e�̐F
	//SetShadow(&g_Shadow);
	//SetShadowEnable(g_ShadowEnable);				// ���̏ꏊ���`�F�b�N���� shadow



}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateLight(void)
{

	SetSunPos(XMFLOAT3(0.0f, 1.0f, 0.0f));



	g_Light[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// ���̐F
	g_Light[0].Enable = TRUE;									// ���̃��C�g��ON

	PLAYER *player = GetPlayer();
	
	XMVECTOR qton = XMQuaternionIdentity();
	XMMATRIX mtxrot;

	//��]���̃x�N�g����rot����]����N�H�[�^�j�I�������߂�
	qton = XMQuaternionRotationAxis(g_Sun.rvec, SUN_ROT);
	//�N�H�[�^�j�I������]�s��ɕϊ�����
	mtxrot = XMMatrixRotationQuaternion(qton);

	g_Sun.Direction = XMVector3Rotate(g_Sun.Direction, qton);

	g_Sun.upvec = XMVector3Rotate(g_Sun.upvec, qton);
	XMStoreFloat3(&g_Sun.up,g_Sun.upvec);



	g_Sun.pos.x = -g_Sun.Direction.m128_f32[0] * SUN_H;
	g_Sun.pos.y = -g_Sun.Direction.m128_f32[1] * SUN_H;
	g_Sun.pos.z = -g_Sun.Direction.m128_f32[2] * SUN_H;

	g_Sun.SunView= XMMatrixLookToLH(XMLoadFloat3(&g_Sun.pos), g_Sun.Direction, XMLoadFloat3(&g_Sun.up));




	 //���s�����̐ݒ�i���E���Ƃ炷���j
	g_Light[0].Direction = XMFLOAT3(g_Sun.Direction.m128_f32[0], g_Sun.Direction.m128_f32[1], g_Sun.Direction.m128_f32[2]);		// ���̌���



	//if (g_Sun.pos.y < 0.0f)
	//{
	//	SetShadowEnable(FALSE);
	//	g_Light[0].Enable = FALSE;

	//}
	//else
	//{
	//	SetShadowEnable(TRUE);
	//	g_Light[0].Enable = TRUE;

	//}
	SetLight(0, &g_Light[0]);									// ����Őݒ肵�Ă���


}


//=============================================================================
// ���C�g�̐ݒ�
// Type�ɂ���ăZ�b�g���郁���o�[�ϐ����ς���Ă���
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// �t�H�O�̐ݒ�
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}


BOOL	GetFogEnable(void)
{
	return(g_FogEnable);
}


//=============================================================================
// �e�̐ݒ�
//=============================================================================




SUN GetSun(void)
{
	return g_Sun;
}
XMMATRIX GetSunView(void)
{
	return g_Sun.SunView;
}






void SetSunPos(XMFLOAT3 pos)
{

	XMVECTOR sunv = XMLoadFloat3(&pos);
	XMVector3Normalize(sunv);
	
	XMFLOAT3 vec;
	XMStoreFloat3(&vec, sunv);

	g_Sun.pos.x = vec.x * SUN_H;
	g_Sun.pos.y = vec.y * SUN_H;
	g_Sun.pos.z = vec.z * SUN_H;

	g_Sun.Direction = XMLoadFloat3(&g_Sun.pos);
	g_Sun.Direction = XMLoadFloat3(&g_Sun.Opos) - g_Sun.Direction;
	g_Sun.Direction = XMVector3Normalize(g_Sun.Direction);


	return;
}