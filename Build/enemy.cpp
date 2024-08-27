#include "Enemy.h"
#include "transformcomponent.h"
#include "level.h"
#include "meshcomponent.h"
#include "ColliderComponent.h"
#include "EnemyStatusComponent.h"

Enemy::Enemy(Level* level)
{
	this->pLevel = level;

	this->meshComponent = new MeshComponent(this);

	this->collider = new ColliderComponent(this);

	this->status = new EnemyStatusComponent(this);
}

Enemy::~Enemy()
{
}

void Enemy::Init(void)
{
	this->transformComponent->Init();
	this->meshComponent->Init();
	this->collider->Init();
	this->status->Init();
	SetUse(TRUE);
	

	collider->onCollider();

	transformComponent->SetTransForm(XMFLOAT3(0.0f, 0.0f, 50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f));
	
	transformComponent->SetForwardDiection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	
	collider->SetSphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f);
	
	//collider->SetCapsuleCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 10.0f, 0.0f), 5.0f);
	tag = ObjectTag::TagEnemy;


	meshComponent->SetMeshComponent(
		"data/MODEL/mesh/EnemySentouki.fbx");

	meshComponent->SetMeshDataList();


}

void Enemy::Uninit(void)
{
	transformComponent->Uninit();
	meshComponent->Uninit();
	collider->Uninit();
	status->Uninit();
}

void Enemy::Update(void)
{

	if (collider->GetHitTag(ObjectTag::TagPlayerBullet))
	{
		status->SubHp(1);
	}


	transformComponent->Update();

	meshComponent->Update();

	collider->Update();

	status->Update();


}

void Enemy::Draw(void)
{
	if (status->GetUse())
	{
		meshComponent->Draw();

	}


}
