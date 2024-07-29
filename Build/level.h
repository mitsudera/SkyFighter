#pragma once
#include "Coreminimal.h"

class Main;
class GameObject;

class Level
{
public:

	Level(Main* main);
	~Level();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:

	Main* pMain;
	vector<GameObject*> gameObject;
	
	

};

