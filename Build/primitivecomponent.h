#pragma once
#include "transformcomponent.h"
class Primitivecomponent :public Transformcomponent
{
public:
	Primitivecomponent();
	~Primitivecomponent();

	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) = 0;

protected:



};

