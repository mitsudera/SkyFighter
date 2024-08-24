#include "EnemyStatusComponent.h"

EnemyStatusComponent::EnemyStatusComponent(GameObject* gameObject)
{
	this->pGameObject = gameObject;
}

EnemyStatusComponent::~EnemyStatusComponent()
{
}

void EnemyStatusComponent::Init(void)
{
	maxHp = 1;
	hp = 1;
	use = TRUE;
}

void EnemyStatusComponent::Uninit(void)
{
}

void EnemyStatusComponent::Update(void)
{
	if (hp<=0)
	{
		use = FALSE;
	}

}

void EnemyStatusComponent::SetMaxHp(int hp)
{
	this->maxHp = hp;
	this->hp = maxHp;
}

BOOL EnemyStatusComponent::GetUse(void)
{
	return use;
}

void EnemyStatusComponent::SetUse(BOOL use)
{
	this->use;
}

void EnemyStatusComponent::SubHp(int n)
{
	hp -= n;
}

void EnemyStatusComponent::AddHp(int n)
{
	hp += n;
}

int EnemyStatusComponent::GetHp(void)
{
	return hp;
}
