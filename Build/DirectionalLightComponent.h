#pragma once
#include "LightComponent.h"
class DirectionalLightComponent :public LightComponent
{
public:

	DirectionalLightComponent(GameObject* gameObject);
	~DirectionalLightComponent();

	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void SetLight(int index,XMFLOAT3 forward, XMFLOAT4 diffuse, XMFLOAT4 amb);
};

