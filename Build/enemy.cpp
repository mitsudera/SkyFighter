#include "Enemy.h"
#include "transformcomponent.h"
#include "level.h"
#include "meshcomponent.h"
#include "ColliderComponent.h"


Enemy::Enemy(Level* level)
{
	this->pLevel = level;

	this->meshComponent = new MeshComponent(this);

	this->collider = new ColliderComponent(this);
}

Enemy::~Enemy()
{
}

void Enemy::Init(void)
{
	this->transformComponent->Init();
	this->meshComponent->Init();
	this->collider->Init();
	

	transformComponent->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	transformComponent->SetForwardDiection(XMFLOAT3(0.0f, 0.0f, 1.0f));

	
	//collider->SetSphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f);
	
	collider->SetCapsuleCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 10.0f, 0.0f), 5.0f);
	collider->SetTag(ObjectTag::TagEnemy);



	meshComponent->SetMeshComponent(
		"data/MODEL/mesh/robot.fbx");

	meshComponent->SetMeshDataList();


}

void Enemy::Uninit(void)
{
	transformComponent->Uninit();
	meshComponent->Uninit();
	collider->Uninit();
}

void Enemy::Update(void)
{

	transformComponent->Update();
	meshComponent->SetWorldMtx(this->transformComponent->GetWorldMtx());

	meshComponent->Update();

	collider->Update();

}

void Enemy::Draw(void)
{
	meshComponent->Draw();

}
