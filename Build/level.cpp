#include "level.h"
#include "gameobject.h"
#include "Main.h"

Level::Level()
{
}

Level::Level(Main*main)
{
	this->pMain = main;
}

Level::~Level()
{
}


Main* Level::GetMain(void)
{
	return this->pMain;
}
