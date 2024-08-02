#pragma once
#include "Title.h"
#include "player.h"
#include "SkySphire.h"
#include "Renderer.h"
#include "Light.h"
#include "ShadowMapping.h"
#include "BlueField.h"

Title::Title(Main* main)
{
	this->pMain = main;

	this->shdowMap = new ShadowMapping(this);

	SkySphire* sky = new SkySphire(this);
	gameObject.push_back(sky);

	Player* player = new Player(this);
	gameObject.push_back(player);
	shadowObject.push_back(player);

	BlueField* blueField = new BlueField(this);
	gameObject.push_back(blueField);


	Light* light = new Light(this);
	gameObject.push_back(light);


	
}

Title::~Title()
{
}

void Title::Init(void)
{
	this->shdowMap->Init();

	this->GetMain()->GetRenderer()->SetLightEnable(TRUE);
	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Init();
	}

}

void Title::Uninit(void)
{
	this->shdowMap->Uninit();

	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Uninit();
	}

}

void Title::Update(void)
{
	this->shdowMap->Update();
	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Update();
	}

}

void Title::Draw(void)
{
	this->shdowMap->Draw();
	this->DrawGameObject();

}

void Title::DrawShadowObject(void)
{
	for (int i = 0; i < shadowObject.size(); i++)
	{
		shadowObject[i]->Draw();
	}

}

void Title::DrawGameObject(void)
{
	GetMain()->GetRenderer()->SetShaderDefault();

	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Draw();
	}

}
