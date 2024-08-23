#pragma once
#include "gameobject.h"

class PlayerBullet;

class BulletList :public GameObject
{
public:
	BulletList(Level* level);
	~BulletList();

	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

	void SetPlayerBullet(XMFLOAT3 pos, XMFLOAT3 dir, float spd);

private:
	PlayerBullet* playerBulletArray;
	

};

