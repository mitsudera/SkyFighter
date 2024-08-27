#pragma once
#include "gameobject.h"

class Missile;

class MissleList :public GameObject
{
public:

	MissleList(Level* level);
	~MissleList();


	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

	void Launch(XMFLOAT3 pos, XMMATRIX rot, float spd, GameObject* target, ObjectTag tag);

private:

	vector<Missile*> missileArray;

};

