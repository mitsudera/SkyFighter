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
	use = FALSE;
	particle = pGameObject->GetLevel()->GetSquareParticle();


}

void BulletComponent::Uninit(void)
{
}

void BulletComponent::Update(void)
{

	if (use == TRUE)
	{
		if (life <= 0)
		{
			DeleteBullet();
		}
		pos.x += direction.x * speed;
		pos.y += direction.y * speed;
		pos.z += direction.z * speed;
		life -= 1;

		particle->UodatePositon(partIndex,pos);

	}


}

void BulletComponent::FireBullet(XMFLOAT3 pos, XMFLOAT3 dir, float spd)
{
	use = TRUE;

	direction = XMFLOAT3Normalize(dir);

	speed = spd+100.0f;

	this->pos = pos;

	life = BULLET_LIFE;

	partIndex = particle->AddParticle(pos, BULLET_SIZE);


}


void BulletComponent::DeleteBullet(void)
{
	use = FALSE;
	particle->DeleteParticle(partIndex);
	pGameObject->GetCollider()->offCollider();
}

BOOL BulletComponent::GetUse(void)
{
	return use;
}
