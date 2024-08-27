#pragma once
#include "component.h"
#include "Coreminimal.h"

class SquareParticle;

class BulletComponent :public Component
{

public:

	BulletComponent(GameObject* gameObject);
	~BulletComponent();

	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void FireBullet(XMFLOAT3 pos, XMFLOAT3 dir, float spd);

	void DeleteBullet(void);

	BOOL GetUse(void);


private:
	XMFLOAT3 pos;
	XMFLOAT3 direction;
	float speed;
	int life;
	//BOOL use;

	int partIndex;

	SquareParticle* particle;

	


};

