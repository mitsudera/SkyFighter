#include "GunComponent.h"
#include "SquareParticle.h"
#include "BulletList.h"

#define BULLET_MAX (256)
#define BULLET_LIFE (300)
#define BULLET_SIZE (1.0f)

GunComponent::GunComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

GunComponent::~GunComponent()
{
}

void GunComponent::Init(void)
{

	this->coolTimeMax = 10;
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

void GunComponent::Launch(XMFLOAT3 pos, XMFLOAT3 dir, float spd)
{
	if (isEnable)
	{

		isEnable = FALSE;

		coolTimeCount = coolTimeMax;


		pGameObject->GetLevel()->GetBulletList()->SetPlayerBullet(pos, dir, spd);

	}
}


void GunComponent::SetCoolTimeMax(int ct)
{
	this->coolTimeMax = ct;

}

void GunComponent::SetParent(Parent p)
{
	this->parent = p;
}
