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
	cameraComponent->SetPosition(XMFLOAT3(4.0f, 8.0f, -15.0f));
	cameraComponent->SetMode(CameraComponent::MODE::TRACKING);
	
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
