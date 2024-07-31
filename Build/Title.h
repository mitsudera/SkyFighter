#pragma once
#include "level.h"
class Title :public Level
{
public:
	
	Title(Main* main);
	~Title();

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:


};

