#pragma once
#include "Coreminimal.h"

class Main;
class GameObject;

class Level
{
public:

	Level(Main* main);
	~Level();

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

protected:

	Main* pMain;
	vector<GameObject*> gameObject;
	
	

};

