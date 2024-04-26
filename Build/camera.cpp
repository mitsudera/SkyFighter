//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POS_X_CAM			(0.0f)			// カメラの初期位置(X座標)
#define	POS_Y_CAM			(0.0f)			// カメラの初期位置(Y座標)
#define	POS_Z_CAM			(-150.0f)		// カメラの初期位置(Z座標)

//#define	POS_X_CAM		(0.0f)			// カメラの初期位置(X座標)
//#define	POS_Y_CAM		(200.0f)		// カメラの初期位置(Y座標)
//#define	POS_Z_CAM		(-400.0f)		// カメラの初期位置(Z座標)


#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(100000000000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)								// カメラの回転量
#define VALUE_ROTATE_CAMERA_MOUSE (XM_PI * 0.003f)
#define VALUE_ROTATE_PAD	(0.00001f)

#define VALUE_ROTATE_CAMERA_PAD (0.00001f)

#define CAMERA_DILAY		10
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA			g_Camera;		// カメラデータ

static int				g_ViewPortType = TYPE_FULL_SCREEN;

static XMVECTOR			cameradilayZ[CAMERA_DILAY];
static XMVECTOR			cameradilayY[CAMERA_DILAY];
static XMVECTOR			cameradilayX[CAMERA_DILAY];

static int				cameradilay;
static int				cameramode;
//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = { POS_X_CAM, POS_Y_CAM, POS_Z_CAM };
	g_Camera.at  = { 0.0f, 0.0f, 0.0f };
	g_Camera.up  = { 0.0f, 1.0f, 0.0f };
	g_Camera.rot = { 0.0f, 0.0f, 0.0f };

	// 視点と注視点の距離を計算
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
	// ビューポートタイプの初期化
	SetViewPort(g_ViewPortType);
}


//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// カメラの更新処理
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


//回転を-180度～180度で管理するため
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

#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Camera:ZC QE TB YN UM R\n");
	//PrintDebugProc("padrot%f\n", leftstickrot);
	PrintDebugProc("Camera:posZ%fposY%fposX%f\n", g_Camera.pos.z, g_Camera.pos.y, g_Camera.pos.x);
	PrintDebugProc("Camera:atZ%fatY%fatX%f\n", g_Camera.at.z, g_Camera.at.y, g_Camera.at.x);
#endif
}


//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(void) 
{
	// ビューマトリックス設定
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Camera.pos), XMLoadFloat3(&g_Camera.at), XMLoadFloat3(&g_Camera.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Camera.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Camera.mtxInvView, mtxInvView);


	// プロジェクションマトリックス設定
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
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

//=============================================================================
// ビューポートの設定
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext *g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	g_ViewPortType = type;

	// ビューポート設定
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



// カメラの視点と注視点をセット
void SetCameraAT()
{
	// カメラの注視点を引数の座標にしてみる
	float camkatamuki=XM_PI/30;
	//float camkatamuki=0.0f;
	PLAYER* g_Player = GetPlayer();

	XMFLOAT3 pos = g_Player->pos;
	//g_Camera.rot.x = -g_Player->rot.x- camkatamuki;
	//g_Camera.rot.y = g_Player->rot.y + XM_PI;
	////// カメラの視点をカメラのY軸回転に対応させている
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


	if (cameramode == 0)				//3人称視点
	{
		g_Camera.at = pos;

		//g_Camera.axisZ = cameradilayZ[0];
		g_Camera.axisZ = g_Player->axisZ;
		g_Camera.axisY = g_Player->axisY;
		g_Camera.axisX = g_Player->axisX;


		//ロールZ軸回転
		//回転軸のベクトルでrot分回転するクォータニオンを求める
		g_Camera.qtonZ = XMQuaternionRotationAxis(g_Camera.axisZ, g_Camera.rot.z);
		//Z軸クォータニオンをZ軸回転行列に変換する
		g_Camera.mtxrotz = XMMatrixRotationQuaternion(g_Camera.qtonZ);


		//ピッチX軸回転
		//回転軸のベクトルでrot分回転するクォータニオンを求める
		g_Camera.qtonX = XMQuaternionRotationAxis(g_Camera.axisX, g_Camera.rot.x);
		//X軸クォータニオンをY軸回転行列に変換する
		g_Camera.mtxrotx = XMMatrixRotationQuaternion(g_Camera.qtonX);

		//ヨー

		//回転軸のベクトルでrot分回転するクォータニオンを求める
		g_Camera.qtonY = XMQuaternionRotationAxis(g_Camera.axisY, g_Camera.rot.y);
		//Y軸クォータニオンをY軸回転行列に変換する
		g_Camera.mtxroty = XMMatrixRotationQuaternion(g_Camera.qtonY);


		//各軸の回転を求める

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
	

	if (cameramode == 1)				//1人称視点
	{

		g_Camera.pos = pos;

		g_Camera.axisZ = g_Player->axisZ;
		g_Camera.axisY = g_Player->axisY;
		g_Camera.axisX = g_Player->axisX;


		//ロールZ軸回転
		//回転軸のベクトルでrot分回転するクォータニオンを求める
		g_Camera.qtonZ = XMQuaternionRotationAxis(g_Camera.axisZ, g_Camera.rot.z);
		//Z軸クォータニオンをZ軸回転行列に変換する
		g_Camera.mtxrotz = XMMatrixRotationQuaternion(g_Camera.qtonZ);


		//ピッチX軸回転
		//回転軸のベクトルでrot分回転するクォータニオンを求める
		g_Camera.qtonX = XMQuaternionRotationAxis(g_Camera.axisX, g_Camera.rot.x);
		//X軸クォータニオンをY軸回転行列に変換する
		g_Camera.mtxrotx = XMMatrixRotationQuaternion(g_Camera.qtonX);

		//ヨー

		//回転軸のベクトルでrot分回転するクォータニオンを求める
		g_Camera.qtonY = XMQuaternionRotationAxis(g_Camera.axisY, g_Camera.rot.y);
		//Y軸クォータニオンをY軸回転行列に変換する
		g_Camera.mtxroty = XMMatrixRotationQuaternion(g_Camera.qtonY);


		//各軸の回転を求める

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
	

#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Camera:rotZ%frotY%frotX%f\n", g_Camera.rot.z, g_Camera.rot.y, g_Camera.rot.x);

	PrintDebugProc("PadRight:X:%f Y:%f\n", rstickx, rsticky);
#endif


	//// カメラの視点をカメラのY軸回転に対応させている
	//g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * cosf(g_Camera.rot.x ) * g_Camera.len;
	//g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * cosf(g_Camera.rot.x ) * g_Camera.len;
	//g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;


}
// カメラの視点と注視点をセット
//void SetCameraBACK(XMFLOAT3 pos)
//{
//	PLAYER *g_Player = GetPlayer();
//	// カメラの注視点を引数の座標にしてみる
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