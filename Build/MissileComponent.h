#pragma once
#include "component.h"
#include "Coreminimal.h"



class MissileComponent :public Component
{
public:
	MissileComponent(GameObject* gameObject);
	~MissileComponent();
	// Component を介して継承されました
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void SetSpeed(float f);
	void SetSpeedMax(float f);
	void SetSpeedUp(float f);
	void SetSpeedUpMax(float f);
	void SetSpeedUpValue(float f);
	void SetRotValue(float f);

	void Launch(XMFLOAT3 pos, XMMATRIX rot, float spd, GameObject* target);
	BOOL GetUse(void);

private:

	GameObject* target;
	float			spd;			// 移動スピード
	float			spdmax;			//最高速度
	float			spdup;			//加速
	float			spdupmax;		//加速度マックス
	float			spdupvalue;		//加速度
	float			rotValue;
	//BOOL use;
	int life;


};

