//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	POS_X_CAM			(0.0f)			// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM			(0.0f)			// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM			(-150.0f)		// �J�����̏����ʒu(Z���W)

//#define	POS_X_CAM		(0.0f)			// �J�����̏����ʒu(X���W)
//#define	POS_Y_CAM		(200.0f)		// �J�����̏����ʒu(Y���W)
//#define	POS_Z_CAM		(-400.0f)		// �J�����̏����ʒu(Z���W)


#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(100000000000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE_CAMERA	(2.0f)										// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)								// �J�����̉�]��
#define VALUE_ROTATE_CAMERA_MOUSE (XM_PI * 0.003f)
#define VALUE_ROTATE_PAD	(0.00001f)

#define VALUE_ROTATE_CAMERA_PAD (0.00001f)

#define CAMERA_DILAY		10
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA			g_Camera;		// �J�����f�[�^

static int				g_ViewPortType = TYPE_FULL_SCREEN;

static XMVECTOR			cameradilayZ[CAMERA_DILAY];
static XMVECTOR			cameradilayY[CAMERA_DILAY];
static XMVECTOR			cameradilayX[CAMERA_DILAY];

static int				cameradilay;
static int				cameramode;
//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = { POS_X_CAM, POS_Y_CAM, POS_Z_CAM };
	g_Camera.at  = { 0.0f, 0.0f, 0.0f };
	g_Camera.up  = { 0.0f, 1.0f, 0.0f };
	g_Camera.rot = { 0.0f, 0.0f, 0.0f };

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);

	PLAYER *g_player = GetPlayer();
	for (int i = 0; i < CAMERA_DILAY; i++)
	{
		cameradilayZ[i] = g_player->axisZ;

	}
	cameradilay = 0;
	cameramode = 0;
	// �r���[�|�[�g�^�C�v�̏�����
	SetViewPort(g_ViewPortType);
}


//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{

	if (IsButtonTriggered(0, BUTTON_R3))
	{
		cameramode += 1;
		cameramode %= 2;
	}


PLAYER* g_Player = GetPlayer();

cameradilayZ[CAMERA_DILAY - 1] = g_Player->axisZ;

for (int i = 1; i < CAMERA_DILAY; i++)
{
	cameradilayZ[i - 1] = cameradilayZ[i];
}


//��]��-180�x�`180�x�ŊǗ����邽��
		if (g_Camera.rot.y < -XM_PI)
		{
			g_Camera.rot.y += XM_PI * 2.0f;
		}
		if (g_Camera.rot.y > XM_PI)
		{
			g_Camera.rot.y -= XM_PI * 2.0f;
		}
		if (g_Camera.rot.x < -XM_PI)
		{
			g_Camera.rot.x += XM_PI * 2.0f;
		}
		if (g_Camera.rot.x > XM_PI)
		{
			g_Camera.rot.x -= XM_PI * 2.0f;
		}
		if (g_Player->rot.x<0)
		{
			g_Camera.up = { 0.0f, -1.0f, 0.0f };
		}
		if (g_Player->rot.x < 0)
		{
			g_Camera.up = { 0.0f, 1.0f, 0.0f };

		}

	//g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * cosf(g_Camera.rot.x);
	//g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * cosf(g_Camera.rot.x);
	//g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x);
//
//XMFLOAT3 upvec;
//
//float uprotx = g_Player->rot.x+XM_PIDIV2;
//
//
//float uproty = g_Player->rot.y;
//
//g_Camera.up = { g_Player->pos.x- sinf(uproty) * cosf(uprotx),g_Player->pos.y - sinf(uprotx) ,g_Player->pos.z - cosf(uproty) * cosf(uprotx) };
//
//

#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Camera:ZC QE TB YN UM R\n");
	//PrintDebugProc("padrot%f\n", leftstickrot);
	PrintDebugProc("Camera:posZ%fposY%fposX%f\n", g_Camera.pos.z, g_Camera.pos.y, g_Camera.pos.x);
	PrintDebugProc("Camera:atZ%fatY%fatX%f\n", g_Camera.at.z, g_Camera.at.y, g_Camera.at.x);
#endif
}


//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(void) 
{
	// �r���[�}�g���b�N�X�ݒ�
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Camera.pos), XMLoadFloat3(&g_Camera.at), XMLoadFloat3(&g_Camera.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Camera.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Camera.mtxInvView, mtxInvView);


	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, cameramode ?100.0f: VIEW_NEAR_Z, VIEW_FAR_Z);
	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Camera.mtxProjection, mtxProjection);

	//SetWorldViewProjection2D();
	//PLAYER* player = GetPlayer();


	//XMFLOAT3 g_SunUp = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//XMFLOAT3 g_Opos = XMFLOAT3(player[0].pos.x, player[0].pos.y, player[0].pos.z);
	//XMFLOAT3 g_SunPos = XMFLOAT3(g_Opos.x + 3000.0f, g_Opos.y + 3000.0f, g_Opos.z - 3000.0f);


	if (GetKeyboardPress(DIK_C))
	{
		PLAYER* player = GetPlayer();

		XMFLOAT3 g_SunPos = XMFLOAT3(player[0].pos.x , player[0].pos.x+ 3000.0f, player[0].pos.z);
		XMFLOAT3 g_SunUp = XMFLOAT3(0.0f, 0.0f, 1.0f);
		XMFLOAT3 g_Opos = XMFLOAT3(player[0].pos.x, player[0].pos.y, player[0].pos.z);
		XMFLOAT3 g_SunDirection = XMFLOAT3(-300.0f, -300.0f, 300.0f);



		XMMATRIX matShadowMapView;
		matShadowMapView = XMMatrixLookAtLH(XMLoadFloat3(&g_SunPos), XMLoadFloat3(&g_Opos), XMLoadFloat3(&g_SunUp));
		matShadowMapView = XMMatrixLookToLH(XMLoadFloat3(&g_SunPos), XMLoadFloat3(&g_SunDirection), XMLoadFloat3(&g_SunUp));
		SetViewMatrix(&matShadowMapView);



		XMMATRIX matShadowMapProj;
		//matShadowMapProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(120.0f), g_ViewPortShadowMap[0].Width / g_ViewPortShadowMap[0].Height,  1.0f, 100000.0f);
		matShadowMapProj = XMMatrixOrthographicLH(10000.0f, 10000.0f, 1.0f, 10000.0f);
		SetProjectionMatrix(&matShadowMapProj);


		//XMMATRIX matShadowMapView;
		//matShadowMapView = XMMatrixLookAtLH(XMLoadFloat3(&g_SunPos), XMLoadFloat3(&g_Opos), XMLoadFloat3(&g_SunUp));
		//SetViewMatrix(&matShadowMapView);

		//XMMATRIX matShadowMapProj;
		////matShadowMapProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 400.0f);
		//mtxProjection = XMMatrixOrthographicLH(10000.0f, 10000.0f, 1.0f, 10000.0f);
		//SetProjectionMatrix(&mtxProjection);
	}

	

	SetShaderCamera(g_Camera.pos);
}


//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

//=============================================================================
// �r���[�|�[�g�̐ݒ�
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext *g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	g_ViewPortType = type;

	// �r���[�|�[�g�ݒ�
	switch (g_ViewPortType)
	{
	case TYPE_FULL_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_LEFT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_RIGHT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = 0;
		break;

	case TYPE_UP_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_DOWN_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;

	case TYPE_LU_QUOTER_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_LD_QUOTER_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;

	case TYPE_RU_QUOTER_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = 0;
		break;

	case TYPE_RD_QUOTER_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;

	}
	g_ImmediateContext->RSSetViewports(1, &vp);

}


int GetViewPortType(void)
{
	return g_ViewPortType;
}



// �J�����̎��_�ƒ����_���Z�b�g
void SetCameraAT()
{
	// �J�����̒����_�������̍��W�ɂ��Ă݂�
	float camkatamuki=XM_PI/30;
	//float camkatamuki=0.0f;
	PLAYER* g_Player = GetPlayer();

	XMFLOAT3 pos = g_Player->pos;
	//g_Camera.rot.x = -g_Player->rot.x- camkatamuki;
	//g_Camera.rot.y = g_Player->rot.y + XM_PI;
	////// �J�����̎��_���J������Y����]�ɑΉ������Ă���
	//g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * cosf(g_Camera.rot.x);
	//g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * cosf(g_Camera.rot.x);
	//g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x);
	float rsticky;
	float rstickx;
	rsticky = GetRightStickY(0) * 0.001f;
	rstickx = GetRightStickX(0) * 0.001f;
	g_Camera.rot.y = rstickx*2.0f;
	g_Camera.rot.x = rsticky+camkatamuki;
	g_Camera.rot.z = 0.0f;


	if (cameramode == 0)				//3�l�̎��_
	{
		g_Camera.at = pos;

		//g_Camera.axisZ = cameradilayZ[0];
		g_Camera.axisZ = g_Player->axisZ;
		g_Camera.axisY = g_Player->axisY;
		g_Camera.axisX = g_Player->axisX;


		//���[��Z����]
		//��]���̃x�N�g����rot����]����N�H�[�^�j�I�������߂�
		g_Camera.qtonZ = XMQuaternionRotationAxis(g_Camera.axisZ, g_Camera.rot.z);
		//Z���N�H�[�^�j�I����Z����]�s��ɕϊ�����
		g_Camera.mtxrotz = XMMatrixRotationQuaternion(g_Camera.qtonZ);


		//�s�b�`X����]
		//��]���̃x�N�g����rot����]����N�H�[�^�j�I�������߂�
		g_Camera.qtonX = XMQuaternionRotationAxis(g_Camera.axisX, g_Camera.rot.x);
		//X���N�H�[�^�j�I����Y����]�s��ɕϊ�����
		g_Camera.mtxrotx = XMMatrixRotationQuaternion(g_Camera.qtonX);

		//���[

		//��]���̃x�N�g����rot����]����N�H�[�^�j�I�������߂�
		g_Camera.qtonY = XMQuaternionRotationAxis(g_Camera.axisY, g_Camera.rot.y);
		//Y���N�H�[�^�j�I����Y����]�s��ɕϊ�����
		g_Camera.mtxroty = XMMatrixRotationQuaternion(g_Camera.qtonY);


		//�e���̉�]�����߂�

		//Z
		XMMATRIX lzrotmtx = XMMatrixMultiply(g_Camera.mtxrotx, g_Camera.mtxroty);
		XMVECTOR lzrotqton = XMQuaternionRotationMatrix(lzrotmtx);
		//X
		XMMATRIX lxrotmtx = XMMatrixMultiply(g_Camera.mtxroty, g_Camera.mtxrotz);
		XMVECTOR lxrotqton = XMQuaternionRotationMatrix(lxrotmtx);
		//Y
		XMMATRIX lyrotmtx = XMMatrixMultiply(g_Camera.mtxrotz, g_Camera.mtxrotx);
		XMVECTOR lyrotqton = XMQuaternionRotationMatrix(lyrotmtx);




		g_Camera.axisZ = XMVector3Rotate(g_Camera.axisZ, lzrotqton);
		g_Camera.axisX = XMVector3Rotate(g_Camera.axisX, lxrotqton);
		g_Camera.axisY = XMVector3Rotate(g_Camera.axisY, lyrotqton);


		//g_Camera.axisZ = XMVector3Rotate(g_Player->axisZ, g_Camera.rot.y);


		//g_Camera.pos.x = g_Camera.at.x + g_Player->axisZ.m128_f32[0] * g_Camera.len - g_Player->axisY.m128_f32[0] * camkatamuki;
		//g_Camera.pos.y = g_Camera.at.y + g_Player->axisZ.m128_f32[1] * g_Camera.len - g_Player->axisY.m128_f32[1] * camkatamuki;
		//g_Camera.pos.z = g_Camera.at.z + g_Player->axisZ.m128_f32[2] * g_Camera.len - g_Player->axisY.m128_f32[2] * camkatamuki;






		//XMVECTOR qtonY = XMQuaternionRotationAxis(g_Player->axisY, g_Camera.rot.y);

		g_Camera.pos.x = g_Camera.at.x + g_Camera.axisZ.m128_f32[0] * g_Camera.len - g_Camera.axisY.m128_f32[0];
		g_Camera.pos.y = g_Camera.at.y + g_Camera.axisZ.m128_f32[1] * g_Camera.len - g_Camera.axisY.m128_f32[1];
		g_Camera.pos.z = g_Camera.at.z + g_Camera.axisZ.m128_f32[2] * g_Camera.len - g_Camera.axisY.m128_f32[2];

		g_Camera.up = { -g_Player[0].axisY.m128_f32[0],-g_Player[0].axisY.m128_f32[1],-g_Player[0].axisY.m128_f32[2] };


		g_Camera.mtxrot = XMMatrixMultiply(lzrotmtx, g_Camera.mtxrotz);
		g_Camera.qtonrot = lzrotqton;


	}
	

	if (cameramode == 1)				//1�l�̎��_
	{

		g_Camera.pos = pos;

		g_Camera.axisZ = g_Player->axisZ;
		g_Camera.axisY = g_Player->axisY;
		g_Camera.axisX = g_Player->axisX;


		//���[��Z����]
		//��]���̃x�N�g����rot����]����N�H�[�^�j�I�������߂�
		g_Camera.qtonZ = XMQuaternionRotationAxis(g_Camera.axisZ, g_Camera.rot.z);
		//Z���N�H�[�^�j�I����Z����]�s��ɕϊ�����
		g_Camera.mtxrotz = XMMatrixRotationQuaternion(g_Camera.qtonZ);


		//�s�b�`X����]
		//��]���̃x�N�g����rot����]����N�H�[�^�j�I�������߂�
		g_Camera.qtonX = XMQuaternionRotationAxis(g_Camera.axisX, g_Camera.rot.x);
		//X���N�H�[�^�j�I����Y����]�s��ɕϊ�����
		g_Camera.mtxrotx = XMMatrixRotationQuaternion(g_Camera.qtonX);

		//���[

		//��]���̃x�N�g����rot����]����N�H�[�^�j�I�������߂�
		g_Camera.qtonY = XMQuaternionRotationAxis(g_Camera.axisY, g_Camera.rot.y);
		//Y���N�H�[�^�j�I����Y����]�s��ɕϊ�����
		g_Camera.mtxroty = XMMatrixRotationQuaternion(g_Camera.qtonY);


		//�e���̉�]�����߂�

		//Z
		XMMATRIX lzrotmtx = XMMatrixMultiply(g_Camera.mtxrotx, g_Camera.mtxroty);
		XMVECTOR lzrotqton = XMQuaternionRotationMatrix(lzrotmtx);
		//X
		XMMATRIX lxrotmtx = XMMatrixMultiply(g_Camera.mtxroty, g_Camera.mtxrotz);
		XMVECTOR lxrotqton = XMQuaternionRotationMatrix(lxrotmtx);
		//Y
		XMMATRIX lyrotmtx = XMMatrixMultiply(g_Camera.mtxrotz, g_Camera.mtxrotx);
		XMVECTOR lyrotqton = XMQuaternionRotationMatrix(lyrotmtx);

		g_Camera.mtxrot = lzrotmtx;
		g_Camera.qtonrot = lzrotqton;

		g_Camera.axisZ = XMVector3Rotate(g_Camera.axisZ, lzrotqton);
		g_Camera.axisX = XMVector3Rotate(g_Camera.axisX, lxrotqton);
		g_Camera.axisY = XMVector3Rotate(g_Camera.axisY, lyrotqton);


		//g_Camera.axisZ = XMVector3Rotate(g_Player->axisZ, g_Camera.rot.y);


		//g_Camera.pos.x = g_Camera.at.x + g_Player->axisZ.m128_f32[0] * g_Camera.len - g_Player->axisY.m128_f32[0] * camkatamuki;
		//g_Camera.pos.y = g_Camera.at.y + g_Player->axisZ.m128_f32[1] * g_Camera.len - g_Player->axisY.m128_f32[1] * camkatamuki;
		//g_Camera.pos.z = g_Camera.at.z + g_Player->axisZ.m128_f32[2] * g_Camera.len - g_Player->axisY.m128_f32[2] * camkatamuki;





		//XMVECTOR qtonY = XMQuaternionRotationAxis(g_Player->axisY, g_Camera.rot.y);

		g_Camera.at.x = g_Camera.pos.x - g_Camera.axisZ.m128_f32[0] * g_Camera.len - g_Camera.axisY.m128_f32[0];
		g_Camera.at.y = g_Camera.pos.y - g_Camera.axisZ.m128_f32[1] * g_Camera.len - g_Camera.axisY.m128_f32[1];
		g_Camera.at.z = g_Camera.pos.z - g_Camera.axisZ.m128_f32[2] * g_Camera.len - g_Camera.axisY.m128_f32[2];

		g_Camera.up = { -g_Player[0].axisY.m128_f32[0],-g_Player[0].axisY.m128_f32[1],-g_Player[0].axisY.m128_f32[2] };


	}
	

#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Camera:rotZ%frotY%frotX%f\n", g_Camera.rot.z, g_Camera.rot.y, g_Camera.rot.x);

	PrintDebugProc("PadRight:X:%f Y:%f\n", rstickx, rsticky);
#endif


	//// �J�����̎��_���J������Y����]�ɑΉ������Ă���
	//g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * cosf(g_Camera.rot.x ) * g_Camera.len;
	//g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * cosf(g_Camera.rot.x ) * g_Camera.len;
	//g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;


}
// �J�����̎��_�ƒ����_���Z�b�g
//void SetCameraBACK(XMFLOAT3 pos)
//{
//	PLAYER *g_Player = GetPlayer();
//	// �J�����̒����_�������̍��W�ɂ��Ă݂�
//	g_Camera.at = pos;
//
//	g_Camera.pos.x = sinf(g_Player->rot.x);
//	g_Camera.pos.z = cosf(g_Player->rot.z);
//
//
//}
//


XMINT2 WorldToScreen(XMFLOAT4 inpos)
{

	XMINT2 ans;
	XMMATRIX mtxworld = XMMatrixIdentity();
	XMMATRIX wvpmat;
	XMMATRIX view = XMLoadFloat4x4(&g_Camera.mtxView);
	XMMATRIX proj = XMLoadFloat4x4(&g_Camera.mtxProjection);
	
	wvpmat = XMMatrixMultiply(mtxworld, view);
	wvpmat = XMMatrixMultiply(wvpmat, proj);

	XMFLOAT4X4 wvp;

	XMStoreFloat4x4(&wvp, wvpmat);

	XMFLOAT4 sp = mul4and4x4(inpos,wvp);


	ans.x = sp.x;
	ans.y = sp.y;
	return ans;


}
//XMINT2 WorldToScreen(XMFLOAT3 pos)
//{
//
//	XMINT2 ans;
//
//	float s = sinf(VIEW_ANGLE);
//	
//	
//	float camtoscreenlen=s*SCREEN_WIDTH;
//	XMVECTOR posv = XMLoadFloat3(&pos);
//	XMVECTOR camposv = XMLoadFloat3(&g_Camera.pos);
//
//	XMVECTOR camtopos = posv - camposv;
//
//	camtopos = XMVector3Normalize(camtopos);
//
//	
//	return ans;
//
//
//}


XMFLOAT4 mul4and4x4(XMFLOAT4 f4, XMFLOAT4X4 f44)
{
	XMFLOAT4 ans;


	ans.x = (f4.x * f44._11) + (f4.y * f44._21) + (f4.z * f44._31) + (f4.w * f44._41);
	ans.y = (f4.x * f44._12) + (f4.y * f44._22) + (f4.z * f44._32) + (f4.w * f44._42);
	ans.z = (f4.x * f44._13) + (f4.y * f44._23) + (f4.z * f44._33) + (f4.w * f44._43);
	ans.w = (f4.x * f44._14) + (f4.y * f44._24) + (f4.z * f44._34) + (f4.w * f44._44);


	return ans;
}