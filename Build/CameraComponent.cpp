//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 
//
//=============================================================================
#pragma once
#include "CameraComponent.h"
#include "Renderer.h"
#include "gameobject.h"
#include "level.h"
#include "Main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(100000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)								// カメラの回転量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int				g_ViewPortType = TYPE_FULL_SCREEN;


CameraComponent::CameraComponent()
{


}

CameraComponent::CameraComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Init(void)
{
	PrimitiveComponent::Init();

	this->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f));

	this->at = { 0.0f, 0.0f, 0.0f };
	this->up = { 0.0f, 1.0f, 0.0f };

	// 視点と注視点の距離を計算
	float vx, vz;
	XMFLOAT3 location = { 0.0f,0.0f,10.0f };
	vx = location.x - this->at.x;
	vz = location.z - this->at.z;
	this->len = sqrtf(vx * vx + vz * vz);



	// 仮
	this->aspect = VIEW_ASPECT;	// アスペクト比 
	this->angle = VIEW_ANGLE;	// 視野角
	this->nearZ = VIEW_NEAR_Z;
	this->farZ = VIEW_FAR_Z;

	this->len = 50.0f;

	this->mode = MODE::WORLD;

	// ビューポートタイプの初期化
	//SetViewPort(g_ViewPortType);
}

void CameraComponent::Update(void)
{
	PrimitiveComponent::Update();



}

void CameraComponent::Draw(void)
{
	PrimitiveComponent::Draw();

	SetViewPort(g_ViewPortType);
	SetCamera();


	//SetCameraAT(XMFLOAT3(0, 0, 1));
}

void CameraComponent::Uninit(void)
{
	PrimitiveComponent::Uninit();


}



void CameraComponent::SetMode(MODE mode)
{
	this->mode = mode;

}

XMMATRIX CameraComponent::GetView(void)
{
	return this->mtxView;
}

//=============================================================================
// カメラの更新
//=============================================================================
void CameraComponent::SetCamera(void)
{
	Renderer* renderer = GetGameObject()->GetLevel()->GetMain()->GetRenderer();
	// ビューマトリックス設定
	XMMATRIX mtxView;


	SetCameraAT(this->lookObject->GetTransFormComponent()->GetPosition());


	XMFLOAT3 pos = {.0f,.0f,.0f};




	switch (this->mode)
	{
	case MODE::TRACKING:

		XMMATRIX pMtx = this->lookObject->GetTransFormComponent()->GetWorldMtx();
		XMFLOAT3 lPos = this->pos;
		XMVECTOR wPos = XMLoadFloat3(&lPos);
		wPos = XMVector3Transform(wPos, pMtx);
		XMStoreFloat3(&lPos, wPos);
		pos = lPos;
		break;
	case MODE::TRACKING_SKY:

		
		XMStoreFloat3(&this->up, lookObject->GetTransFormComponent()->GetAxisY());


		XMFLOAT3 pPos = lookObject->GetTransFormComponent()->GetPosition();

		XMVECTOR pPosVec = XMLoadFloat3(&pPos);
		this->at = pPos;
		

		XMVECTOR lxVec = lookObject->GetTransFormComponent()->GetAxisX();
		XMVECTOR lyVec = lookObject->GetTransFormComponent()->GetAxisY();
		XMVECTOR lzVec = lookObject->GetTransFormComponent()->GetAxisZ();


		lxVec *= this->pos.x;
		lyVec *= this->pos.y;
		lzVec *= this->pos.z;

		XMVECTOR v = (lxVec+lyVec+lzVec);

		v = v + pPosVec;

		XMStoreFloat3(&pos, v);

		break;
	case MODE::WORLD:
		
		pos = this->pos;
		break;

	default:
		break;
	}

	XMFLOAT3 At = this->at;
	XMFLOAT3 Up = this->up;


	//mtxView = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&this->at), XMLoadFloat3(&this->up));

	XMVECTOR posv = XMLoadFloat3(&pos);

	mtxView = XMMatrixLookAtLH(posv, XMLoadFloat3(&At), XMLoadFloat3(&Up));
	renderer->SetViewMatrix(&mtxView);
	this->mtxView = mtxView;

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);


	// プロジェクションマトリックス設定
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	renderer->SetProjectionMatrix(&mtxProjection);

	renderer->SetShaderCamera(GetWorldPos());
}

//=============================================================================
// ビューポートの設定
//=============================================================================
void CameraComponent::SetViewPort(int m_type)
{
	Renderer* renderer = GetGameObject()->GetLevel()->GetMain()->GetRenderer();
	ID3D11DeviceContext *ImmediateContext = renderer->GetDeviceContext();
	D3D11_VIEWPORT vp;

	g_ViewPortType = m_type;



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
	case TYPE_VIEWPORT:
		vp.Width = (FLOAT)SCREEN_WIDTH * 0.6f;
		vp.Height = (FLOAT)SCREEN_HEIGHT * 0.6f + GetSystemMetrics(SM_CYCAPTION);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH * 0.2f;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT * 0.1f;
		break;
	}
	ImmediateContext->RSSetViewports(1, &vp);

}


int CameraComponent::GetViewPortType(void)
{
	return g_ViewPortType;
}



// カメラの視点と注視点をセット
void CameraComponent::SetCameraAT(XMFLOAT3 pos)
{
	this->at = pos;

	
}

void CameraComponent::SetCameraUp(XMFLOAT3 up)
{
	this->up = up;
}

void CameraComponent::SetLookObject(GameObject* gameObject)
{
	this->lookObject = gameObject;
}

