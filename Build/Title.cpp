#pragma once
#include "Title.h"
#include "player.h"
#include "SkySphire.h"
#include "Renderer.h"
#include "Light.h"
#include "ShadowMapping.h"
#include "BlueField.h"
#include "camera.h"

Title::Title(Main* main)
{
	this->pMain = main;

	this->shdowMap = new ShadowMapping(this);

	this->mainCamera = new Camera(this);
	this->gameObject.push_back(mainCamera);


	Player* player = new Player(this);
	gameObject.push_back(player);
	shadowObject.push_back(player);

	SkySphire* sky = new SkySphire(this);
	gameObject.push_back(sky);

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
	this->shdowMap->SetTarget(gameObject[1]);


	this->shdowMap->SetDirection(XMFLOAT3(0.5f, 1.0f, 0.5f));
	this->shdowMap->SetLen(200.0f);

	this->mainCamera->SetLookObject(gameObject[1]);
	

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
	this->shdowMap->SetShadowMap(
		XMFLOAT3(100.0f, 100.0f, -100.0f),
		this->gameObject[0]->GetTransFormComponent()->GetPosition(),
		XMFLOAT3(0.0f, 1.0f, 0.0f));
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
