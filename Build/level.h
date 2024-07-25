#pragma once
#include "main.h"
#include "gameobject.h"

class Level
{
public:

	Level();
	~Level();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:
	vector<Gameobject*> gameObject;
	
	

};

