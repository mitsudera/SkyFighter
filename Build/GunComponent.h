#pragma once
#include "component.h"
#include "Coreminimal.h"


class SquareParticle;
class ColliderComponent;

struct Bullet
{
	XMFLOAT3 pos;
	XMFLOAT3 direction;
	float speed;
	int life;
	BOOL use;

	int partIndex;


};

class GunComponent :public Component
{
public:
	GunComponent(GameObject* gameObject);
	~GunComponent();
	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void Fire(XMFLOAT3 pos, XMFLOAT3 dir, float spd);

	void DeleteBullet(int n);

	void SetCoolTimeMax(int ct);

	enum class Parent
	{
		Player,
		Enemy,
	};


	void SetParent(Parent p);


private:

	int coolTimeMax;
	int coolTimeCount;

	BOOL isEnable;

	Bullet* bullets;

	Parent parent;

	SquareParticle* particle;

};

