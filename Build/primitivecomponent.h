#pragma once
#include "transformcomponent.h"
class PrimitiveComponent :public TransformComponent
{
public:
	PrimitiveComponent();
	PrimitiveComponent(GameObject* gameObject);
	~PrimitiveComponent();

	virtual void Init(void) override=0;

	virtual void Uninit(void) override=0;

	virtual void Update(void) override=0;

	virtual void Draw(void) = 0;

protected:



};

