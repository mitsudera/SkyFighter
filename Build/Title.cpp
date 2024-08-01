#pragma once
#include "Title.h"
#include "player.h"
#include "SkySphire.h"
#include "Renderer.h"

Title::Title(Main* main)
{
	this->pMain = main;
	Player* player = new Player(this);
	gameObject.push_back(player);
	SkySphire* sky = new SkySphire(this);
	gameObject.push_back(sky);
	LIGHT light;
	light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f,0.1f);
	light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f);
	light.Direction = XMFLOAT3(1.0f, 1.0f, 1.0f);
	light.Enable = TRUE;
	light.Type = 1;

	this->GetMain()->GetRenderer()->SetLight(0, &light);
	this->GetMain()->GetRenderer()->SetLightEnable(TRUE);
	this->GetMain()->GetRenderer()->SetLightBuffer();
	
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
