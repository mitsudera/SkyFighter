#include "BulletComponent.h"
#include "SquareParticle.h"
#include "gameobject.h"
#include "ColliderComponent.h"


#define BULLET_LIFE (300)
#define BULLET_SIZE (1.0f)


BulletComponent::BulletComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;


}

BulletComponent::~BulletComponent()
{
}

void BulletComponent::Init(void)
{
	pGameObject->SetUse(TRUE);
	particle = pGameObject->GetLevel()->GetSquareParticleAdd();


}

void BulletComponent::Uninit(void)
{
}

void BulletComponent::Update(void)
{

	if (pGameObject->GetUse() == TRUE)
	{
		if (life <= 0)
		{
			DeleteBullet();
		}
		pos.x += direction.x * speed;
		pos.y += direction.y * speed;
		pos.z += direction.z * speed;
		life -= 1;

		particle->UpdatePositon(partIndex,pos);

	}


}

void BulletComponent::FireBullet(XMFLOAT3 pos, XMFLOAT3 dir, float spd)
{
	pGameObject->SetUse(TRUE);

	direction = XMFLOAT3Normalize(dir);

	speed = spd+100.0f;

	this->pos = pos;

	life = BULLET_LIFE;

	partIndex = particle->AddParticle(pos, BULLET_SIZE,0.8f);


}


void BulletComponent::DeleteBullet(void)
{
	pGameObject->SetUse(FALSE);
	particle->DeleteParticle(partIndex);
	pGameObject->GetCollider()->offCollider();
}

BOOL BulletComponent::GetUse(void)
{
	return pGameObject->GetUse();
}
