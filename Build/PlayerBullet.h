#pragma once
#include "gameobject.h"

class ColliderComponent;
class BulletComponent;

class PlayerBullet :public GameObject
{
public:
	PlayerBullet();
	PlayerBullet(Level* level);
	~PlayerBullet();

	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

	void SetLevel(Level* level);

	BulletComponent* GetBullet(void);

	void SetBullet(XMFLOAT3 pos, XMFLOAT3 dir, float spd);


private:

	BulletComponent* bulletComponent;
	

};

