#pragma once
#include "main.h"

class Component
{
public:
	Component();
	~Component();
	
	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

protected:

};

