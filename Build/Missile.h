#pragma once
#include "gameobject.h"

class MeshComponent;
class MissileComponent;

class Missile : public GameObject
{
public:
	Missile(Level* level);
	~Missile();

	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;



private:

	MeshComponent* mesh;
	MissileComponent* missileComponent;

};

