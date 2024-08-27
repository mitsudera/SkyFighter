#include "PlayerBullet.h"
#include "ColliderComponent.h"
#include "BulletComponent.h"

PlayerBullet::PlayerBullet()
{
	this->collider = new ColliderComponent(this);
	this->bulletComponent = new BulletComponent(this);

}

PlayerBullet::PlayerBullet(Level* level)
{

	this->pLevel = level;
	this->collider = new ColliderComponent(this);
	this->bulletComponent = new BulletComponent(this);
}

PlayerBullet::~PlayerBullet()
{
	delete collider;
	delete bulletComponent;
}

void PlayerBullet::Init(void)
{

	collider->Init();
	collider->offCollider();
	tag = ObjectTag::TagPlayerBullet;

	bulletComponent->Init();


}

void PlayerBullet::Uninit(void)
{
	collider->Uninit();
	bulletComponent->Uninit();

}

void PlayerBullet::Update(void)
{
	collider->Update();

	bulletComponent->Update();
}

void PlayerBullet::Draw(void)
{
}

void PlayerBullet::SetLevel(Level* level)
{
	this->pLevel = level;
}

BulletComponent* PlayerBullet::GetBullet(void)
{
	return bulletComponent;
}

void PlayerBullet::SetBullet(XMFLOAT3 pos, XMFLOAT3 dir, float spd)
{
	bulletComponent->FireBullet(pos, dir, spd);
	collider->SetLineCollider(spd);

	collider->onCollider();

}
