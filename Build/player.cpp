#include "player.h"


Player::Player(Level* level)
{
	this->pLevel = level;

	this->meshComponent = new MeshComponent(this);
}

Player::~Player()
{
}

void Player::Init(void)
{
	this->transformComponent->Init();
	this->meshComponent->Init();
	
	transformComponent->SetTransForm(XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

	meshComponent->SetMeshComponent(
		"data/MODEL/mesh/sentouki.fbx");

	meshComponent->SetMeshDataList();


}

void Player::Uninit(void)
{
	transformComponent->Uninit();
	meshComponent->Uninit();
}

void Player::Update(void)
{
	transformComponent->Update();
	meshComponent->SetWorldMtx(this->transformComponent->GetWorldMtx());

	meshComponent->Update();

}

void Player::Draw(void)
{

	
	meshComponent->Draw();
}
