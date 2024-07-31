#include "Title.h"
#include "gameobject.h"
Title::Title(Main* main)
{
	this->pMain = main;
}

Title::~Title()
{

}

void Title::Init(void)
{
	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Init();
	}

}

void Title::Uninit(void)
{
	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Uninit();
	}

}

void Title::Update(void)
{
	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Update();
	}

}

void Title::Draw(void)
{

	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Draw();
	}

}
