#pragma once
#include "Title.h"
#include "player.h"
#include "SkySphere.h"
#include "Renderer.h"
#include "Light.h"
#include "ShadowMapping.h"
#include "BlueField.h"
#include "camera.h"
#include "enemy.h"
#include "CollisionManger.h"
#include "SquareParticle.h"
#include "BulletList.h"
#include "MIssleList.h"
#include "EffectList.h"
#include "cloud.h"

Title::Title(Main* main)
{
	this->pMain = main;


	mainCamera = new Camera(this);

	squarePartAdd = new SquareParticle(this);
	squarePartSub = new SquareParticle(this);

	effectList = new EffectList(this);

	bulletList = new BulletList(this);


	Player* player = new Player(this);
	gameObject.push_back(player);
	shadowObject.push_back(player);

	Cloud* cloud = new Cloud(this);
	gameObject.push_back(cloud);
	cloudList.push_back(cloud);

	Enemy* enemy = new Enemy(this);
	gameObject.push_back(enemy);
	shadowObject.push_back(enemy);
	enemyList.push_back(enemy);

	missileList = new MissleList(this);
	gameObject.push_back(missileList);
	shadowObject.push_back(missileList);




	SkySphere* sky = new SkySphere(this);
	gameObject.push_back(sky);

	BlueField* blueField = new BlueField(this);
	gameObject.push_back(blueField);


	Light* light = new Light(this);
	gameObject.push_back(light);


	
}

Title::~Title()
{
	delete mainCamera;
	delete squarePartAdd;
	delete squarePartSub;
	delete bulletList;
	delete effectList;

	for (int i = 0; i < gameObject.size(); i++)
	{
		delete gameObject[i];
	}

	gameObject.clear();
}

void Title::Init(void)
{
	this->shdowMap->Init();
	this->collisionManager->Init();
	this->GetMain()->GetRenderer()->SetLightEnable(TRUE);

	this->mainCamera->Init();

	this->squarePartAdd->Init();
	this->squarePartAdd->SetBlend(BLEND_MODE::BLEND_MODE_ADD);
	this->squarePartSub->Init();
	this->squarePartSub->SetBlend(BLEND_MODE::BLEND_MODE_SUBTRACT);

	effectList->Init();

	this->bulletList->Init();


	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Init();
	}


	this->shdowMap->SetTarget(gameObject[0]);


	this->shdowMap->SetForward(XMFLOAT3(0.5f, 1.0f, 0.5f));
	this->shdowMap->SetLen(1000.0f);
	this->shdowMap->SetNear(100.0f);
	this->shdowMap->SetFar(3000.0f);


	this->mainCamera->SetLookObject(gameObject[0]);
	

}

void Title::Uninit(void)
{
	this->shdowMap->Uninit();
	squarePartAdd->Uninit();
	squarePartSub->Uninit();

	effectList->Uninit();

	bulletList->Uninit();


	this->collisionManager->Uninit();

	this->mainCamera->Uninit();
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

	this->collisionManager->Update();

	this->mainCamera->Update();


	for (int i = 0; i < gameObject.size(); i++)
	{
		gameObject[i]->Update();
	}


	bulletList->Update();

	effectList->Update();

	squarePartAdd->Update();
	squarePartSub->Update();



	//XMFLOAT3 up;
	//XMStoreFloat3(&up, gameObject[1]->GetTransFormComponent()->GetAxisY());
	//this->mainCamera->SetUp(up);


}

void Title::Draw(void)
{
	this->shdowMap->Draw();

	this->mainCamera->Draw();
	this->DrawGameObject();
	squarePartAdd->Draw();
	squarePartSub->Draw();



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
