#include "player.h"


Player::Player(Level* level)
{
	this->pLevel = level;

	this->meshComponent = new MeshComponent(this);
	this->camera = new CameraComponent(this);
}

Player::~Player()
{
}

void Player::Init(void)
{
	this->transformComponent->Init();
	this->meshComponent->Init();
	this->camera->Init();

	camera->SetPosition(XMFLOAT3(0.0f, 5.0f, -50.0f));
	
	transformComponent->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

	meshComponent->SetMeshComponent(
		"data/MODEL/mesh/sentouki.fbx");

	meshComponent->SetMeshDataList();


}

void Player::Uninit(void)
{
	transformComponent->Uninit();
	meshComponent->Uninit();
	camera->Uninit();
}

void Player::Update(void)
{
	transformComponent->Update();
	meshComponent->SetWorldMtx(this->transformComponent->GetWorldMtx());
	camera->SetCameraAT(this->transformComponent->GetPosition());

	meshComponent->Update();

	camera->Update();
}

void Player::Draw(void)
{

	camera->Draw();
	
	meshComponent->Draw();
}
