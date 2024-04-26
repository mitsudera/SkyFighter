//=============================================================================
//
// �p�[�e�B�N������ [cloud.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "light.h"
#include "cloud.h"
#include "player.h"
#include"renderer.h"
#include"debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)			// �e�N�X�`���̐�

#define	CLOUD_SIZE_X		(20000.0f)		// ���_�T�C�Y
#define	CLOUD_SIZE_Y		(20000.0f)		// ���_�T�C�Y
#define	VALUE_MOVE_CLOUD	(5.0f)		// �ړ����x

#define	MAX_CLOUD_PART		(16348)		// �p�[�e�B�N���ő吔
#define MAX_CLOUD			(100)
#define MAX_POLYGON			(MAX_CLOUD_PART*2)
#define MAX_VERTEX			(MAX_POLYGON*6)
#define CLOUD_SIZE			(10.0f)
#define POLYGON_VERTEX		(6)
#define CLOUD_ALPHA			(0.2f)
#define MAX_MPOINT			(32)//2�̔{��
#define MPOINT_SPLIT		(5)
#define CLOUD_HIGHT			(3500)
#define CLOUD_PART_DIST		(20000.0f)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexCloud(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;		// ���_�o�b�t�@
// ���_�z��
static VERTEX_3D g_VertexArray[MAX_CLOUD_PART][POLYGON_VERTEX];



static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;					// �e�N�X�`���ԍ�

static CLOUD					g_aCloud[MAX_CLOUD_PART];		// �p�[�e�B�N�����[�N
static XMFLOAT3					g_posBase;						// �r���{�[�h�����ʒu
static float					g_fWidthBase = 5.0f;			// ��̕�
static float					g_fHeightBase = 10.0f;			// ��̍���
static float					g_roty = 0.0f;					// �ړ�����
static float					g_spd = 0.0f;					// �ړ��X�s�[�h
static int						g_partcnt = 0;

static XMFLOAT3 g_vpos[4];

//x		//z
static float hight[MAX_MPOINT + 1][MAX_MPOINT + 1][2];

static char *g_TextureName[] =
{
	//"data/TEXTURE/effect000.jpg",
	//"data/TEXTURE/siro.png",
	"data/TEXTURE/particle_smoke.png",
	//"data/TEXTURE/cloud_sakura.png",
};

static BOOL							g_Load = FALSE;



//=============================================================================
// ����������
//=============================================================================
HRESULT InitCloud(void)
{

	//// ���_���̍쐬
	//MakeVertexCloud();

	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	// �p�[�e�B�N�����[�N�̏�����
	for(int nCntCloud = 0; nCntCloud < MAX_CLOUD_PART; nCntCloud++)
	{
		g_aCloud[nCntCloud].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aCloud[nCntCloud].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aCloud[nCntCloud].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aCloud[nCntCloud].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_aCloud[nCntCloud].material, sizeof(g_aCloud[nCntCloud].material));
		g_aCloud[nCntCloud].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aCloud[nCntCloud].fSizeX = CLOUD_SIZE_X;
		g_aCloud[nCntCloud].fSizeY = CLOUD_SIZE_Y;
		g_aCloud[nCntCloud].nLife = 0;
		g_aCloud[nCntCloud].use = FALSE;
		g_aCloud[nCntCloud].type = 1;



		for (int i = 0; i < 4; i++)
		{
			g_vpos[i] = g_aCloud[nCntCloud].pos;
		}

		g_vpos[0].x -= CLOUD_SIZE_X / 2;
		g_vpos[0].y += CLOUD_SIZE_Y / 2;


		g_vpos[1].x += CLOUD_SIZE_X / 2;
		g_vpos[1].y += CLOUD_SIZE_Y / 2;


		g_vpos[2].x -= CLOUD_SIZE_X / 2;
		g_vpos[2].y -= CLOUD_SIZE_Y / 2;


		g_vpos[3].x += CLOUD_SIZE_X / 2;
		g_vpos[3].y -= CLOUD_SIZE_Y / 2;



		g_VertexArray[nCntCloud][0] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
		g_VertexArray[nCntCloud][1] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
		g_VertexArray[nCntCloud][2] = { g_vpos[1], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) };
		g_VertexArray[nCntCloud][3] = { g_vpos[2], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) };
		g_VertexArray[nCntCloud][4] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };
		g_VertexArray[nCntCloud][5] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };


		

	}

	for (int x = 0; x < MAX_MPOINT+1; x++)
	{

		for (int z = 0; z < MAX_MPOINT+1; z++)
		{
			hight[x][z][0] = -10.0f;
			hight[x][z][1] = -10.0f;
		}
	}


	XMFLOAT3 clopos = XMFLOAT3(-200000.0f, 50000.0f, 0.0f);
	InitRand(1000, FALSE);
	SetCloud(clopos, 1.0f, 1.0f, 1.0f);



	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCloud(void)
{
	if (g_Load == FALSE) return;

	//�e�N�X�`���̉��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCloud(void)
{

		XMMATRIX mtxWorld;

		//mtxWorld = XMMatrixIdentity();
		CAMERA* cam = GetCamera();

		XMFLOAT3 vpos[4];

		vpos[0].x = -CLOUD_SIZE_X / 2;
		vpos[0].y = +CLOUD_SIZE_Y / 2;
		vpos[0].z = 0.0f;

		vpos[1].x = +CLOUD_SIZE_X / 2;
		vpos[1].y = +CLOUD_SIZE_Y / 2;
		vpos[1].z = 0.0f;

		vpos[2].x = -CLOUD_SIZE_X / 2;
		vpos[2].y = -CLOUD_SIZE_Y / 2;
		vpos[2].z = 0.0f;

		vpos[3].x = +CLOUD_SIZE_X / 2;
		vpos[3].y = -CLOUD_SIZE_Y / 2;
		vpos[3].z = 0.0f;

		XMVECTOR vvec[4];


		XMVECTOR axx;

		XMVECTOR axy;

		XMFLOAT4X4 view = cam->mtxView;

		XMFLOAT3 vx = XMFLOAT3(view._11, view._21, view._31);
		axx = XMLoadFloat3(&vx);
		XMFLOAT3 vy = XMFLOAT3(view._12, view._22, view._32);
		axy = XMLoadFloat3(&vy);
		for (int i = 0; i < 4; i++)
		{

			vvec[i] += XMLoadFloat3(&vpos[i]);
			vvec[i] = XMVector3Normalize(vvec[i]);
			switch (i)
			{
			case 0:

				vvec[i] = axy * CLOUD_SIZE_Y;
				break;
			case 1:
				vvec[i] = axx * CLOUD_SIZE_X;
				break;


			case 2:


				vvec[i] = axx * -CLOUD_SIZE_X;
				break;

			case 3:

				vvec[i] = axy * -CLOUD_SIZE_Y;
				break;


			default:
				break;
			}

			//XMVector3Rotate(vvec[i],cam->qtonrot);
			XMStoreFloat3(&vpos[i], vvec[i]);

		}


		for(int nCntCloud = 0; nCntCloud < MAX_CLOUD_PART; nCntCloud++)
		{
			if(g_aCloud[nCntCloud].use)
			{// �g�p��








				for (int i = 0; i < 4; i++)
				{
					g_vpos[i] = g_aCloud[nCntCloud].pos;
				}



				g_vpos[0].x += vpos[0].x;
				g_vpos[0].y += vpos[0].y;
				g_vpos[0].z += vpos[0].z;
				
				g_vpos[1].x += vpos[1].x;
				g_vpos[1].y += vpos[1].y;
				g_vpos[1].z += vpos[1].z;

				g_vpos[2].x += vpos[2].x;
				g_vpos[2].y += vpos[2].y;
				g_vpos[2].z += vpos[2].z;

				g_vpos[3].x += vpos[3].x;
				g_vpos[3].y += vpos[3].y;
				g_vpos[3].z += vpos[3].z;





				g_VertexArray[nCntCloud][0] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(0.0f, 0.0f) };
				g_VertexArray[nCntCloud][1] = { g_vpos[0], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(0.0f, 0.0f) };
				g_VertexArray[nCntCloud][2] = { g_vpos[1], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(1.0f, 0.0f) };
				g_VertexArray[nCntCloud][3] = { g_vpos[2], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(0.0f, 1.0f) };
				g_VertexArray[nCntCloud][4] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(1.0f, 1.0f) };
				g_VertexArray[nCntCloud][5] = { g_vpos[3], XMFLOAT3(0.0f, 0.0f,1.0f), g_aCloud[nCntCloud].material.Diffuse, XMFLOAT2(1.0f, 1.0f) };

			}
			else
			{
				g_VertexArray[nCntCloud][0] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
				g_VertexArray[nCntCloud][1] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
				g_VertexArray[nCntCloud][2] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) };
				g_VertexArray[nCntCloud][3] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) };
				g_VertexArray[nCntCloud][4] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };
				g_VertexArray[nCntCloud][5] = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };



			}


		}




		if (g_VertexBuffer)			g_VertexBuffer->Release();
		MakeVertexCloud();




}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCloud(void)
{
	//XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	//CAMERA* cam = GetCamera();

	

	SetCullingMode(CULL_MODE_FRONT);

	// ���C�e�B���O�𖳌���
	SetLightEnable(FALSE);

	// ���Z�����ɐݒ�
	SetBlendState(BLEND_MODE_ADD);

	// Z��r�L��
	SetDepthEnable(FALSE);



	// �t�H�O����
	SetFogEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	CAMERA* cam = GetCamera();


	XMMATRIX mtxWorld, mtxView;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �r���[�}�g���b�N�X���擾
	mtxView = XMLoadFloat4x4(&cam->mtxView);







	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);



	// �|���S���`��
	int cnt = sizeof(g_VertexArray) / sizeof(VERTEX_3D);
	GetDeviceContext()->Draw(cnt, 0);		// cnt���_����0�Ԗڂ̒��_�ԍ�����`��

#ifdef _DEBUG	// �f�o�b�O����\������

	
#endif


// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// �ʏ�u�����h�ɖ߂�
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	SetDepthEnable(TRUE);



	// �t�H�O�L��
	SetFogEnable(GetFogEnable());

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexCloud(void)
{



	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4 * MAX_POLYGON;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	// �O���[�o���̒��_�z�񂩂璸�_�o�b�t�@������
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = g_VertexArray;

	GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);



	return S_OK;
}

//=============================================================================
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorCloud(int nIdxCloud, XMFLOAT4 col)
{
	g_aCloud[nIdxCloud].material.Diffuse = col;
}

//=============================================================================
// �_�̔�������
//=============================================================================
void SetCloud(
	XMFLOAT3 pos,		//�_�̒��S���W


	float fSizeX,		//�_�̃T�C�Y
	float fSizeY,
	float fSizeZ		
)
{

	int x1 = 0;
	int x2 = MAX_MPOINT;
	int z1 = 0;
	int z2 = MAX_MPOINT;

	SetCloudHight1(x1,x2,z1,z2, 0, hight[0][MAX_MPOINT][1], hight[MAX_MPOINT][MAX_MPOINT][1],hight[0][0][1],hight[MAX_MPOINT][0][1]);

	

	int nIdxCloud = -1;

	for (int x = 0; x < MAX_MPOINT+1; x++)
	{
		for (int z = 0; z <MAX_MPOINT+1; z++)
		{

			hight[x][z][1] -= 5.0f;
			int yleng =(int)( hight[x][z][1]);

			for (int y = 0; y < yleng; y++)
			{
				for (int nCntCloud = 0; nCntCloud < MAX_CLOUD_PART; nCntCloud++)
				{
					if (!g_aCloud[nCntCloud].use)
					{


						{
							g_aCloud[nCntCloud].pos.x = x * CLOUD_PART_DIST + pos.x + (float)(rand() % 10-5) * 0.1f* CLOUD_PART_DIST;
							g_aCloud[nCntCloud].pos.z = z * CLOUD_PART_DIST + pos.z + (float)(rand() % 10-5) * 0.1f*CLOUD_PART_DIST;
							//g_aCloud[nCntCloud].pos.y = pos.y + hight[x][z][1]/ 2;
							g_aCloud[nCntCloud].pos.y = pos.y +( hight[x][z][1] - hight[x][z][1] / 2) + ((hight[x][z][1]* CLOUD_PART_DIST) / yleng) * y - hight[x][z][1] / 2 + (float)(rand() % 10 - 5) * 0.1f* CLOUD_PART_DIST-yleng*CLOUD_HIGHT;
							//g_aCloud[nCntCloud].pos.y -= (float)((rand()%yleng)/2)*(float)(CLOUD_HIGHT);
							g_aCloud[nCntCloud].rot = { 0.0f, 0.0f, 0.0f };
							g_aCloud[nCntCloud].scale = { 1.0f, 1.0f, 1.0f };
							g_aCloud[nCntCloud].fSizeX = 1.0f;
							g_aCloud[nCntCloud].fSizeY = 1.0f;
							g_aCloud[nCntCloud].use = TRUE;
							g_aCloud[nCntCloud].material.Diffuse = { 1.0f,1.0f,1.0f,CLOUD_ALPHA };

							g_partcnt += 1;
							for (int i = 0; i < 4; i++)
							{
								g_vpos[i] = g_aCloud[nCntCloud].pos;
							}




							nIdxCloud = nCntCloud;
						}

						break;
					}
				}



			}


		}

	}


	return ;
}












void SetCloudHight1(int xl,int xr,int zd,int zu, int n, float tl,float tr,float bl,float br)
{
	if (n == MPOINT_SPLIT)
	{
		return;
	}
	//����
	float top = (tl + tr) / 2;
	float bottom = (bl + br) / 2;
	float left = (tl+ bl) / 2;
	float right = (tr + br) / 2 ;
	float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	
	//float top = (tl + tr) / 2 + GetRandomHightPM(n+1);
	//float bottom = (bl + br) / 2 + GetRandomHightPM(n+1);
	//float left = (tl+ bl) / 2 + GetRandomHightPM(n+1);
	//float right = (tr + br) / 2 + GetRandomHightPM(n+1);
	//float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	//
	hight[(xl + xr) / 2][zu][1] = top;
	hight[(xl + xr) / 2][zd][1] = bottom;
	hight[xl][(zu + zd) / 2][1] = left;
	hight[xr][(zu + zd) / 2][1] = right;
	hight[(xl + xr) / 2][(zu + zd) / 2][1] = midpoint;


	SetCloudHight1(xl, (xl + xr) / 2, (zu + zd) / 2, zu, n + 1, tl, top, left, midpoint);
	SetCloudHight1((xl + xr) / 2, xr, (zu + zd) / 2, zu, n + 1, top, tr, midpoint, right);
	SetCloudHight1(xl, (xl + xr) / 2, zd, (zu + zd) / 2, n + 1, left, midpoint, bl, bottom);
	SetCloudHight1((xl + xr) / 2, xr, zd, (zu + zd) / 2, n + 1, midpoint, right, bottom, br);
}
void SetCloudHight2(int xl,int xr,int zd,int zu, int n, float tl,float tr,float bl,float br)
{
	if (n == MPOINT_SPLIT)
	{
		return;
	}
	//����
	float top = (tl + tr) / 2;
	float bottom = (bl + br) / 2;
	float left = (tl+ bl) / 2;
	float right = (tr + br) / 2 ;
	float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	
	//float top = (tl + tr) / 2 + GetRandomHightPM(n+1);
	//float bottom = (bl + br) / 2 + GetRandomHightPM(n+1);
	//float left = (tl+ bl) / 2 + GetRandomHightPM(n+1);
	//float right = (tr + br) / 2 + GetRandomHightPM(n+1);
	//float midpoint = (tl + tr + bl + br) / 4+GetRandomHightPM(n);
	//
	hight[(xl + xr) / 2][zu][1] = top;
	hight[(xl + xr) / 2][zd][1] = bottom;
	hight[xl][(zu + zd) / 2][1] = left;
	hight[xr][(zu + zd) / 2][1] = right;
	hight[(xl + xr) / 2][(zu + zd) / 2][1] = midpoint;


	SetCloudHight2(xl, (xl + xr) / 2, (zu + zd) / 2, zu, n + 1, tl, top, left, midpoint);
	SetCloudHight2((xl + xr) / 2, xr, (zu + zd) / 2, zu, n + 1, top, tr, midpoint, right);
	SetCloudHight2(xl, (xl + xr) / 2, zd, (zu + zd) / 2, n + 1, left, midpoint, bl, bottom);
	SetCloudHight2((xl + xr) / 2, xr, zd, (zu + zd) / 2, n + 1, midpoint, right, bottom, br);
}
float GetRandomHightPM(int n)
{
	float h = 0.0f;
	h = (rand() % (int)(CLOUD_HIGHT / powf(2, n))) * 0.01f;
	//h = (((rand() % (int)(CLOUD_HIGHT / powf(2, n)))- ((CLOUD_HIGHT / powf(2, n)))/2)) * 0.01f;

	return h;
}
float GetRandomHightP(int n)
{
	float h = 0.0f;
	h = (rand() % (int)(CLOUD_HIGHT / powf(2, n))) * 0.01f;
	//h = (((rand() % (int)(CLOUD_HIGHT / powf(2, n)))- ((CLOUD_HIGHT / powf(2, n)))/2)) * 0.01f;

	return h;
}

