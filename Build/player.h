#pragma once
#include "gameobject.h"

class MeshComponent;
class JetEngineComponent;
class GunComponent;
class MissileLauncherComponent;

class Player:public GameObject
{


public:
	Player(Level* level);
	~Player();

	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;


private:
	
	
	MeshComponent* meshComponent;
	JetEngineComponent* jetEngine;
	GunComponent* gun;
	MissileLauncherComponent* missileLauncher;
};

