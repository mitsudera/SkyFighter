#include "level.h"
#include "gameobject.h"
#include "player.h"

Level::Level(Main*main)
{
	this->pMain = main;
	Player* player = new Player(this);
	gameObject.push_back(player);

}

Level::~Level()
{
}

void Level::Init(void)
{



	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Init();
	}
}

void Level::Uninit(void)
{
	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Uninit();
	}

}

void Level::Update(void)
{
	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Update();
	}

}

void Level::Draw(void)
{
	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Draw();
	}

}
