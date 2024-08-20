#pragma once
#include "gameobject.h"
class Bullet :public GameObject
{
public:
	
	Bullet(Level* level);
	~Bullet();

	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

private:

	XMFLOAT3 direction;
	float speed;




};

