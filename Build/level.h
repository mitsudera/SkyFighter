#pragma once
#include "Coreminimal.h"
class Main;
class GameObject;

class Level
{
public:

	Level();
	Level(Main* main);
	~Level();

	virtual void Init(void)=0;
	virtual void Uninit(void)=0;
	virtual void Update(void)=0;
	virtual void Draw(void)=0;

	Main* GetMain(void);
	
	

protected:

	Main* pMain;
	
	
	

};

