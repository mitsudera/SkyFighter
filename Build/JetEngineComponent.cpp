#include "JetEngineComponent.h"
#include "gameobject.h"

JetEngineComponent::JetEngineComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

JetEngineComponent::~JetEngineComponent()
{
}

void JetEngineComponent::Init(void)
{
	spd = 0.0f;
	spdmax = 0.0f;
	spdup = 0.0f;
	spdupmax = 0.0f;
	spdupvalue = 0.0f;
	brake = 0.0f;
}

void JetEngineComponent::Uninit(void)
{
}

void JetEngineComponent::Update(void)
{
	pGameObject->GetTransFormComponent()->MoveForward(this->spd);

}

void JetEngineComponent::SetSpeed(float f)
{
	spd = f;
}

float JetEngineComponent::GetSpeed(void)
{
	return spd;
}

void JetEngineComponent::SetSpeedMax(float f)
{
	spdmax = f;
}

float JetEngineComponent::GetSpeedMax(void)
{
	return spdmax;
}

void JetEngineComponent::SetSpeedUp(float f)
{
	spdup = f;
}

float JetEngineComponent::GetSpeedUp(void)
{
	return spdup;
}

void JetEngineComponent::SetSpeedUpMax(float f)
{
	spdupmax = f;
}

float JetEngineComponent::GetSpeedUpMax(void)
{
	return spdupmax;
}

void JetEngineComponent::SetSpeedUpValue(float f)
{
	spdupvalue = f;
}

float JetEngineComponent::GetSpeedUpValue(void)
{
	return spdupvalue;
}

void JetEngineComponent::SetBrake(float f)
{
	brake = f;
}

float JetEngineComponent::GetBrake(void)
{
	return brake;
}

void JetEngineComponent::Brake(void)
{
	spd *= brake;
	spdup *= brake;
}

void JetEngineComponent::Accel(void)
{
	spdup +=spdupvalue;
	if (spdup > spdupmax)
	{
		spdup = spdupmax;
	}
	spd += spdup;

}

