#include "camera.h"
Camera::Camera(Level* level)
{
	this->pLevel = level;
	this->cameraComponent = new CameraComponent(this);
}

Camera::~Camera()
{
	delete cameraComponent;
}

void Camera::Init(void)
{
	this->cameraComponent->Init();
	cameraComponent->SetPosition(XMFLOAT3(0.0f, 3.0f, -30.0f));
	cameraComponent->SetMode(CameraComponent::MODE::TRACKING_SKY);
	
}

void Camera::Uninit(void)
{
	this->cameraComponent->Uninit();
}

void Camera::Update(void)
{
	this->cameraComponent->Update();
}

void Camera::Draw(void)
{
	this->cameraComponent->Draw();
}

void Camera::SetLookObject(GameObject* object)
{
	this->cameraComponent->SetLookObject(object);
}

void Camera::SetUp(XMFLOAT3 up)
{
	this->cameraComponent->SetCameraUp(up);
}

XMMATRIX Camera::GetView(void)
{
	return this->cameraComponent->GetView();
}
