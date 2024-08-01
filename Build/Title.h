#pragma once
#include "level.h"

class Player;
class Title :public Level
{
public:
	Title(Main* main);
	~Title();

	// Level ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
private:
	vector<GameObject*> gameObject;

};

