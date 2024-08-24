#pragma once
#include "component.h"
#include "Coreminimal.h"
class EnemyStatusComponent :public Component
{
public:
	EnemyStatusComponent(GameObject* gameObject);
	~EnemyStatusComponent();


	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void SetMaxHp(int hp);

	BOOL GetUse(void);

	void SetUse(BOOL use);

	void SubHp(int n);

	void AddHp(int n);

	int GetHp(void);


private:
	BOOL use;
	int maxHp;
	int hp;


};

