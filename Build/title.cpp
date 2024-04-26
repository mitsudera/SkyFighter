//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : 
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(10)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/sorakirei.jpg",
	"data/TEXTURE/title.png",
	"data/TEXTURE/tutorial.png",
	"data/TEXTURE/normalmode.png",
	"data/TEXTURE/infinitymode.png",
	"data/TEXTURE/end.png",
	"data/TEXTURE/tuysetumei.png",
	"data/TEXTURE/normalsetumei.png",
	"data/TEXTURE/infsetumei.png",
	"data/TEXTURE/endsetumei.png",
	
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

float	alpha;
BOOL	flag_alpha;

static BOOL						g_Load = FALSE;

static int choice;

static float	effect_dx;
static float	effect_dy;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);



	// �ϐ��̏�����
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = TRUE;

	effect_dx = 100.0f;
	effect_dy = 100.0f;

	choice = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{

	if (GetKeyboardTrigger(DIK_RETURN)|| IsButtonTriggered(0, BUTTON_M)|| IsButtonTriggered(0, BUTTON_C))
	{// Enter��������A�X�e�[�W��؂�ւ���
		switch (choice)
		{
		case 0:
			SetFade(FADE_OUT, MODE_TUTORIAL);
			break;

		case 1:
			SetInfMode(FALSE);
			SetFade(FADE_OUT, MODE_GAME);

			break;

		case 2:
			SetInfMode(TRUE);
			SetFade(FADE_OUT, MODE_GAME);

			break;

		case 3:

			SetEnd();

			break;

		default:
			break;
		}


	}


	if (IsButtonTriggered(0, BUTTON_UP)|| GetKeyboardTrigger(DIK_UP))
	{
		choice -= 1;
		if (choice<0)
		{
			choice = 3;
		}
	}

	if (IsButtonTriggered(0, BUTTON_DOWN) || GetKeyboardTrigger(DIK_DOWN))
	{
		choice += 1;
		choice %= 4;
	}




#ifdef _DEBUG	// �f�o�b�O����\������
	//PrintDebugProc("Player:�� �� �� ���@Space\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//// �^�C�g���̔w�i��`��
	//{
	//	// �e�N�X�`���ݒ�
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	//	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	//	// �|���S���`��
	//	GetDeviceContext()->Draw(4, 0);
	//}

	// �^�C�g���̕���
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X , 100.0f, 600.0f, 150.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


	// �^�C�g���̕���
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);



		if (choice==0)
		{
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X / 2, 300.0f, 200.0f, 50.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

		}
		else
		{
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X / 2, 300.0f, 200.0f, 50.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));

		}

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


	// �^�C�g���̕���
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		if (choice == 1)
		{
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X / 2, 350.0f, 200.0f, 50.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

		}
		else
		{
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X / 2, 350.0f, 200.0f, 50.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));

		}
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


	// �^�C�g���̕���
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

		if (choice == 2)
		{
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X / 2, 400.0f, 200.0f, 50.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

		}
		else
		{
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X / 2, 400.0f, 200.0f, 50.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));

		}

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �^�C�g���̕���
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);

		if (choice == 3)
		{
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X / 2, 450.0f, 200.0f, 50.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

		}
		else
		{
			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X / 2, 450.0f, 200.0f, 50.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));

		}

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


	switch (choice)
	{
	case 0:
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X+ SCREEN_CENTER_X / 2, SCREEN_CENTER_Y+100.0f, 300.0f, 100.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


		break;

	case 1:
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + SCREEN_CENTER_X / 2, SCREEN_CENTER_Y + 100.0f, 300.0f, 100.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

		break;

	case 2:


	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + SCREEN_CENTER_X / 2, SCREEN_CENTER_Y + 100.0f, 300.0f, 100.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

		break;

	case 3:


	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + SCREEN_CENTER_X / 2, SCREEN_CENTER_Y + 100.0f, 300.0f, 100.0f, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

		break;

	default:
		break;
	}




}





