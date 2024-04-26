//=============================================================================
//
// �G�l�~�[���f������ [biru.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "biru.h"
#include"player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BIRU			"data/MODEL/aoibiru.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define BIRU_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define BIRU_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BIRU			g_Biru[MAX_BIRU];				// �G�l�~�[
int g_Biru_load = 0;
static DX11_MODEL model;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBiru(void)
{
	LoadModel(MODEL_BIRU, &model);

	for (int i = 0; i < MAX_BIRU; i++)
	{
		g_Biru[i].load = true;

		g_Biru[i].pos = XMFLOAT3(-100.0f + i * 30.0f, 0.0f, 100.0f*i);
		g_Biru[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Biru[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Biru[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Biru[i].size = BIRU_SIZE;	// �����蔻��̑傫��

		int muki = 0;
		int move = 1;





		g_Biru[i].use = true;		// true:�����Ă�

	}
		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Biru[0].model, &g_Biru[0].diffuse[0]);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBiru(void)
{

	for (int i = 0; i < MAX_BIRU; i++)
	{
		if (g_Biru[i].load)
		{
			UnloadModel(&g_Biru[i].model);
			g_Biru[i].load = false;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBiru(void)
{
	PLAYER* g_Player = GetPlayer();

	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_BIRU; i++)
	{
		if (g_Biru[i].use == true)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes
			
			switch (i)
			{
			case 0:	
				break;

			case 1:
				break;

			case 2:
				break;

			default:
				break;
			}

		}
	}

	//cnt += 1;


#ifdef _DEBUG

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	// ���f���̐F��ύX�ł����I�������ɂ��ł����B
	//	for (int j = 0; j < g_Biru[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Biru[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// ���f���̐F�����ɖ߂��Ă���
	//	for (int j = 0; j < g_Biru[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Biru[0].model, j, g_Biru[0].diffuse[j]);
	//	}
	//}
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBiru(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BIRU; i++)
	{
		if (g_Biru[i].use == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Biru[i].scl.x, g_Biru[i].scl.y, g_Biru[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Biru[i].rot.x, g_Biru[i].rot.y + XM_PI, g_Biru[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Biru[i].pos.x, g_Biru[i].pos.y, g_Biru[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Biru[i].mtxWorld, mtxWorld);


			//// ���f���`��
			//DrawModel(&model);
		}
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
BIRU *GetBiru()
{
	return &g_Biru[0];
}
