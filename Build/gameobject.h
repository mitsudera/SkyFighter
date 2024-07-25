#pragma once
#include"transformcomponent.h"
#include"main.h"
class Gameobject
{
public:
	Gameobject();
	~Gameobject();

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

protected:
	
	Transformcomponent* transformComponent;
	
	

};

