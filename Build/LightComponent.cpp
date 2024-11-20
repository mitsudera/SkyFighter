#include "LightComponent.h"
#include "gameobject.h"
#include "level.h"
#include "Main.h"
#include "Renderer.h"

LightComponent::LightComponent()
{
	index = -1;
	direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	position= XMFLOAT3(0.0f, 0.0f, 0.0f);
	diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	attenuation = 1000.0f;
	type = DIRECTIONAL;
	enable = false;

}

LightComponent::LightComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;

}

LightComponent::~LightComponent()
{
}

void LightComponent::Init(void)
{
	this->Set();
}

void LightComponent::Uninit(void)
{
}

void LightComponent::Update(void)
{
}

void LightComponent::SetIndex(int n)
{
	if (n >= LIGHT_MAX)
	{
		return;
	}

	this->index = n;
}

int LightComponent::GetIndex(void)
{

	return this->index;
}

void LightComponent::SetDiffuse(XMFLOAT4 diffuse)
{
	this->diffuse = diffuse;
}

XMFLOAT4 LightComponent::GetDiffuse(void)
{
	return this->diffuse;
}

void LightComponent::SetEnable(BOOL enable)
{
	this->enable = enable;
}

BOOL LightComponent::GetEnable(void)
{
	return this->enable;
}

XMFLOAT3 LightComponent::GetDirection(void)
{
	return this->direction;
}

void LightComponent::SetDirecton(XMFLOAT3 forward)
{
	this->direction = forward;
}

XMFLOAT3 LightComponent::GetPosition(void)
{
	return this->position;
}

void LightComponent::SetPosition(XMFLOAT3 pos)
{
	this->position = pos;
}

XMFLOAT4 LightComponent::GetAmbient(void)
{
	return this->ambient;
}

void LightComponent::SetAmbient(XMFLOAT4 amb)
{
	this->ambient = amb;
}

float LightComponent::GetAttenuation(void)
{
	return this->attenuation;
}

void LightComponent::SetAttenuation(float att)
{
	this->attenuation = att;
}

BOOL LightComponent::GetType(void)
{
	return this->type;
}

void LightComponent::Set(void)
{
	this->GetGameObject()->GetLevel()->GetMain()->GetRenderer()->SetLight(this);

}

