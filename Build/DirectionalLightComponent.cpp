#include "DirectionalLightComponent.h"
#include "gameobject.h"
DirectionalLightComponent::DirectionalLightComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	this->type = DIRECTIONAL;
	
}

DirectionalLightComponent::~DirectionalLightComponent()
{
}

void DirectionalLightComponent::Init(void)
{
	LightComponent::Init();

}

void DirectionalLightComponent::Uninit(void)
{
	LightComponent::Uninit();
}

void DirectionalLightComponent::Update(void)
{
	LightComponent::Update();
}

void DirectionalLightComponent::SetLight(int index, XMFLOAT3 forward, XMFLOAT4 diffuse, XMFLOAT4 amb)
{
	this->direction = forward;
	this->diffuse = diffuse;
	this->ambient = amb;
	this->enable = TRUE;
	this->type = DIRECTIONAL;
	this->SetIndex(index);
}

