#pragma once
#include "component.h"
#include "Coreminimal.h"

class MissileLauncherComponent :public Component
{
public:
	MissileLauncherComponent(GameObject* gameObject);
	~MissileLauncherComponent();




	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void Launch(XMFLOAT3 pos, XMMATRIX rot, float spd);



	void SetCoolTimeMax(int ct);

	enum class Parent
	{
		Player,
		Enemy,
	};


	void SetParent(Parent p);

	void SetTargetObjectList(vector<GameObject*> list);
	GameObject* GetTargetObject(void);


private:
	int coolTimeMax;
	int coolTimeCount;

	BOOL isEnable;

	Parent parent;


	vector<GameObject*> TargetObjectList;
	GameObject* lockOnObject;


};

