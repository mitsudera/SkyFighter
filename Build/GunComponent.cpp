#include "GunComponent.h"
#include "SquareParticle.h"

#define BULLET_MAX (256)
#define BULLET_LIFE (300)
#define BULLET_SIZE (1.0f)

GunComponent::GunComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
	bullets = new Bullet[BULLET_MAX];
}

GunComponent::~GunComponent()
{
	delete[] bullets;
}

void GunComponent::Init(void)
{
	this->particle = GetGameObject()->GetLevel()->GetSquareParticle();

	this->coolTimeMax = 10;
	this->coolTimeCount = 0;
	this->isEnable = TRUE;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		bullets[i].use = FALSE;

		
	}
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

	for (int i = 0; i < BULLET_MAX; i++)
	{

		if (bullets[i].use==TRUE)
		{
			if (bullets[i].life <= 0)
			{
				DeleteBullet(i);
			}
			bullets[i].pos.x += bullets[i].direction.x * bullets[i].speed;
			bullets[i].pos.y += bullets[i].direction.y * bullets[i].speed;
			bullets[i].pos.z += bullets[i].direction.z * bullets[i].speed;
			bullets[i].life -= 1;

			particle->UodatePositon(bullets[i].partIndex, bullets[i].pos);

		}

	}


}

void GunComponent::Fire(XMFLOAT3 pos, XMFLOAT3 dir, float spd)
{
	if (isEnable)
	{
		coolTimeCount = coolTimeMax;

		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (bullets[i].use==FALSE)
			{
				bullets[i].use = TRUE;

				bullets[i].direction = XMFLOAT3Normalize(dir);

				bullets[i].speed = spd+1.0f;

				bullets[i].pos = pos;

				bullets[i].life = BULLET_LIFE;

				bullets[i].partIndex = particle->AddParticle(pos, BULLET_SIZE);

				isEnable = FALSE;

				return;
			}
		}
	}
}

void GunComponent::DeleteBullet(int n)
{

	bullets[n].use = FALSE;
	particle->DeleteParticle(bullets[n].partIndex);
}

void GunComponent::SetCoolTimeMax(int ct)
{
	this->coolTimeMax = ct;

}

void GunComponent::SetParent(Parent p)
{
	this->parent = p;
}
