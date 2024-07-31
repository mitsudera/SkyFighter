#include "player.h"

Player::Player(Level* level)
{
	this->pLevel = level;

	this->meshComponent = new MeshComponent;

}

Player::~Player()
{
}

void Player::Init(void)
{
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

}

void Player::Update(void)
{

}

void Player::Draw(void)
{

}
