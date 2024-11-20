#include "Light.h"
#include "DirectionalLightComponent.h"

Light::Light(Level* level)
{
	this->pLevel = level;
	this->dirLight = new DirectionalLightComponent(this);
}

Light::~Light()
{
	if (dirLight!=nullptr)
	{
		delete dirLight;

		dirLight = nullptr;
	}
}

void Light::Init(void)
{
	dirLight->SetLight(0,XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(.1f, .1f, .1f, 1.0f));
	dirLight->Init();
}

void Light::Uninit(void)
{
	dirLight->Uninit();
}

void Light::Update(void)
{
	dirLight->Update();
}

void Light::Draw(void)
{
}
