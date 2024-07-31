#pragma once
#include "component.h"
class MovementComponent :public Component
{
public:

	MovementComponent();
	MovementComponent(GameObject* gameObject);
	
	~MovementComponent();

	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

private:
	float			spd;		// 移動スピード
	float			spdmax;		//最高速度
	float			spdup;		//加速
	float			spdupmax;		//加速
	float			spdupvalue;		//加速度
	float			brake;		//ブレーキ



};

