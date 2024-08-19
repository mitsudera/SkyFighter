#pragma once
#include "component.h"

class GameObject;

class JetEngineComponent :public Component
{
public:

	JetEngineComponent(GameObject* gameObject);
	~JetEngineComponent();


	// Component を介して継承されました
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	void SetSpeed(float f);
	float GetSpeed(void);
	void SetSpeedMax(float f);
	float GetSpeedMax(void);
	void SetSpeedUp(float f);
	float GetSpeedUp(void);
	void SetSpeedUpMax(float f);
	float GetSpeedUpMax(void);
	void SetSpeedUpValue(float f);
	float GetSpeedUpValue(void);
	void SetBrake(float f);
	float GetBrake(void);

	void Brake(void);
	void Accel(void);


private:

	float			spd;			// 移動スピード
	float			spdmax;			//最高速度
	float			spdup;			//加速
	float			spdupmax;		//加速度マックス
	float			spdupvalue;		//加速度
	float			brake;			//ブレーキ



};

