#pragma once
#include "gameobject.h"

class Level;
class MeshComponent;
class ColliderComponent;

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
	ColliderComponent* collider;
	

};

