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

	void Fire(void);

	void SetCoolTimeMax(int ct);

private:

	int coolTimeMax;
	int coolTimeCount;

	BOOL isEnable;



};

