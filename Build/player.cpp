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

	camera->SetPosition(XMFLOAT3(0.0f, 0.0f, 1.0f));
	
	transformComponent->SetPosition(XMFLOAT3(0.0f, 0.0f, -10.0f));
	
	transformComponent->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	vector<string> animpath = {
	"data/animation/keyframe/robotwalk.fbx",
	"data/animation/keyframe/robotrun.fbx"
	};

	meshComponent->SetMeshComponent(
		"data/MODEL/mesh/robot.fbx",
		animpath,
		TRUE,
		30);

	meshComponent->SetMeshDataList();
	meshComponent->SetAnimationArray();
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
