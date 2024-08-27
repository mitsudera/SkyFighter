#include "Missile.h"
#include"transformcomponent.h"
#include"meshcomponent.h"
#include"ColliderComponent.h"
#include"MissileComponent.h"

Missile::Missile(Level* level)
{
	this->pLevel = level;

	missileComponent = new MissileComponent(this);
	
	collider = new ColliderComponent(this);

	mesh = new MeshComponent(this);

}

Missile::~Missile()
{
	delete missileComponent;
	delete collider;
	delete mesh;
}

void Missile::Init(void)
{



	transformComponent->Init();

	transformComponent->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

	transformComponent->SetForwardDiection(XMFLOAT3(0.0f, 0.0f, 1.0f));

	

	missileComponent->Init();
	collider->Init();
	collider->offCollider();
	mesh->Init();

	mesh->SetMeshComponent(
		"data/MODEL/mesh/missile.fbx");

	mesh->SetMeshDataList();


}

void Missile::Uninit(void)
{
	transformComponent->Uninit();
	missileComponent->Uninit();
	collider->Uninit();
	mesh->Uninit();

}

void Missile::Update(void)
{
	if (missileComponent->GetUse())
	{
		transformComponent->Update();
		missileComponent->Update();
		collider->Update();
		mesh->Update();


	}

}

void Missile::Draw(void)
{
	if (missileComponent->GetUse())
	{
		mesh->Draw();
	}
}

void Missile::Launch(XMFLOAT3 pos, XMMATRIX rot, float spd, GameObject* target, ObjectTag tag)
{
	this->tag = tag;
	missileComponent->Launch(pos, rot, spd, target);
}

MissileComponent* Missile::GetMissileComponent(void)
{
	return this->missileComponent;
}
