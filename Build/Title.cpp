#include "Title.h"
#include "player.h"

Title::Title(Main* main)
{
	this->pMain = main;
	player = new Player(this);
}

Title::~Title()
{
}

void Title::Init(void)
{
	player->Init();
}

void Title::Uninit(void)
{
	player->Uninit();
}

void Title::Update(void)
{
	player->Update();
}

void Title::Draw(void)
{
	player->Draw();
}
