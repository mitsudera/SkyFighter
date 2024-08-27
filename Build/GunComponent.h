#pragma once
#include "component.h"
#include "Coreminimal.h"


class GunComponent :public Component
{
public:
	GunComponent(GameObject* gameObject);
	~GunComponent();
	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void Launch(XMFLOAT3 pos, XMFLOAT3 dir, float spd);



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

	Parent parent;


};

