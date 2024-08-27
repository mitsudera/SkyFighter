#include "level.h"
#include "gameobject.h"
#include "Main.h"
#include "camera.h"
#include "CollisionManger.h"
#include "ShadowMapping.h"
Level::Level()
{
	this->shdowMap = new ShadowMapping(this);

	this->collisionManager = new CollisionManger(this);

}

Level::Level(Main*main)
{
	this->pMain = main;
}

Level::~Level()
{
	delete shdowMap;
	delete collisionManager;
}


Main* Level::GetMain(void)
{
	return this->pMain;
}

CollisionManger* Level::GetCollisionManager(void)
{
	return this->collisionManager;
}

GameObject* Level::GetGameObject(int n)
{
	return gameObject[n];
}

Camera* Level::GetCamera(void)
{
	return this->mainCamera;
}

XMMATRIX Level::GetCameraView(void)
{
	return this->mainCamera->GetView();
}

SquareParticle* Level::GetSquareParticleAdd(void)
{
	return this->squarePartAdd;
}

SquareParticle* Level::GetSquareParticleSub(void)
{
	return this->squarePartSub;
}

BulletList* Level::GetBulletList(void)
{
	return this->bulletList;
}

MissleList* Level::GetMissileList(void)
{
	return missileList;
}

vector<GameObject*> Level::GetGameObjectList(void)
{
	return gameObject;
}

EffectList* Level::GetEffect(void)
{
	return effectList;
}
