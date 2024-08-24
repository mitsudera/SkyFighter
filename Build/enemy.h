#pragma once
#include "gameobject.h"

class Level;
class MeshComponent;
class EnemyStatusComponent;

class Enemy :public GameObject
{
	
public:
	Enemy(Level* level);
	~Enemy();

	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;


private:


	MeshComponent* meshComponent;
	EnemyStatusComponent* status;

};
