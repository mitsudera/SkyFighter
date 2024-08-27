#pragma once
#include "Coreminimal.h"
class Main;
class GameObject;
class ShadowMapping;
class Camera;
class CollisionManger;
class SquareParticle;
class BulletList;
class MissleList;
class EffectList;

class Level
{
public:

	Level();
	Level(Main* main);
	~Level();

	virtual void Init(void)=0;
	virtual void Uninit(void)=0;
	virtual void Update(void)=0;
	virtual void Draw(void)=0;
	virtual void DrawShadowObject(void) = 0;
	virtual void DrawGameObject(void) = 0;

	Main* GetMain(void);
	
	CollisionManger* GetCollisionManager(void);
	
	GameObject* GetGameObject(int n);

	Camera* GetCamera(void);
	XMMATRIX GetCameraView(void);
	SquareParticle* GetSquareParticleAdd(void);
	SquareParticle* GetSquareParticleSub(void);
	BulletList* GetBulletList(void);
	MissleList* GetMissileList(void);

	vector<GameObject*> GetGameObjectList(void);

	EffectList* GetEffect(void);

protected:

	Main* pMain;
	
	vector<GameObject*> gameObject;
	vector<GameObject*> shadowObject;
	ShadowMapping* shdowMap;

	
	Camera* mainCamera;
	CollisionManger* collisionManager;
	SquareParticle* squarePartAdd;
	SquareParticle* squarePartSub;

	BulletList* bulletList;
	MissleList* missileList;

	EffectList* effectList;


	vector<GameObject*> enemyList;
	vector<GameObject*> cloudList;


};

