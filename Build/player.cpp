#include "player.h"
#include "transformcomponent.h"
#include "level.h"
#include "meshcomponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"

Player::Player(Level* level)
{
	this->pLevel = level;

	this->meshComponent = new MeshComponent(this);

	this->collider = new ColliderComponent(this);
}

Player::~Player()
{
}

void Player::Init(void)
{
	this->transformComponent->Init();
	this->meshComponent->Init();
	this->collider->Init();


	transformComponent->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	transformComponent->SetForwardDiection(XMFLOAT3(0.0f, 0.0f, 1.0f));

	collider->SetSphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f);

	collider->SetTag(ObjectTag::TagPlayer);

	meshComponent->SetMeshComponent(
		"data/MODEL/mesh/sentouki.fbx");

	meshComponent->SetMeshDataList();


}

void Player::Uninit(void)
{
	transformComponent->Uninit();
	meshComponent->Uninit();
	collider->Uninit();

}

void Player::Update(void)
{
	XMFLOAT3 pos = this->transformComponent->GetPosition();

	//if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_DOWN))
	//this->transformComponent->SetPosition(XMFLOAT3(pos.x, pos.y - 0.01f, pos.z));
	if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_UP))
	{
		this->transformComponent->MoveForward(1.0f);

	}

	transformComponent->Update();
	meshComponent->SetWorldMtx(this->transformComponent->GetWorldMtx());

	meshComponent->Update();

	collider->Update();
	
	BOOL hit = collider->GetHitTag(TagEnemy);


}

void Player::Draw(void)
{

	
	meshComponent->Draw();
}
