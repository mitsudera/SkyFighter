#include "GunComponent.h"

GunComponent::GunComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

GunComponent::~GunComponent()
{
}

void GunComponent::Init(void)
{
	this->coolTimeMax = 0;
	this->coolTimeCount = 0;
	this->isEnable = TRUE;

}

void GunComponent::Uninit(void)
{

}

void GunComponent::Update(void)
{
	if (!isEnable)
	{
		coolTimeCount -= 1;

		if (coolTimeCount<=0)
		{
			isEnable = TRUE;
		}
		
	}

}

void GunComponent::Fire(void)
{
	if (isEnable)
	{
		coolTimeCount = coolTimeMax;
	}
}

void GunComponent::SetCoolTimeMax(int ct)
{
	this->coolTimeMax = ct;

}
