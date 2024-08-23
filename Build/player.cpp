#include "player.h"
#include "transformcomponent.h"
#include "level.h"
#include "meshcomponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "JetEngineComponent.h"
#include "GunComponent.h"

Player::Player(Level* level)
{
	this->pLevel = level;

	this->meshComponent = new MeshComponent(this);

	this->collider = new ColliderComponent(this);

	this->jetEngine = new JetEngineComponent(this);

	this->gun = new GunComponent(this);
}

Player::~Player()
{
}

void Player::Init(void)
{
	this->transformComponent->Init();
	this->meshComponent->Init();
	this->collider->Init();
	this->jetEngine->Init();
	this->gun->Init();

	collider->onCollider();

	transformComponent->SetTransForm(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	transformComponent->SetForwardDiection(XMFLOAT3(0.0f, 0.0f, 1.0f));

	collider->SetSphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f);

	collider->SetTag(ObjectTag::TagPlayer);

	meshComponent->SetMeshComponent(
		"data/MODEL/mesh/sentouki.fbx");

	meshComponent->SetMeshDataList();

	jetEngine->SetSpeed(0.0f);
	jetEngine->SetSpeedMax(160.0f);
	jetEngine->SetBrake(0.99f);
	jetEngine->SetSpeedUpValue(0.002f);
	jetEngine->SetSpeedUpMax(1.0f);

}

void Player::Uninit(void)
{
	transformComponent->Uninit();
	meshComponent->Uninit();
	collider->Uninit();
	jetEngine->Uninit();
	gun->Uninit();

}

void Player::Update(void)
{
	XMFLOAT3 pos = this->transformComponent->GetPosition();

	//if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_DOWN))
	//this->transformComponent->SetPosition(XMFLOAT3(pos.x, pos.y - 0.01f, pos.z));
	if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_W))
	{
		jetEngine->Accel();
	}
	else if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_S))
	{
		jetEngine->Brake();
	}

	if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_RIGHT))
	{
		this->transformComponent->RotRoll(-0.1f);

	}
	if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_LEFT))
	{
		this->transformComponent->RotRoll(0.1f);

	}

	if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_UP))
	{
		this->transformComponent->RotPitch(0.1f);

	}
	if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_DOWN))
	{
		this->transformComponent->RotPitch(-0.1f);

	}


	if (pLevel->GetMain()->GetInput()->GetKeyboardPress(DIK_SPACE))
	{
		gun->Fire(this->transformComponent->GetPosition(),this->transformComponent->GetDirection(),this->jetEngine->GetSpeed());
	}





	this->jetEngine->Update();


	transformComponent->Update();

	meshComponent->Update();



	collider->Update();

	gun->Update();

	BOOL hit = collider->GetHitTag(TagEnemy);


}

void Player::Draw(void)
{
	meshComponent->Draw();
}
