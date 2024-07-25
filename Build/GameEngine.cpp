//=============================================================================
//
// 
//
//=============================================================================
#include "GameEngine.h"
#include "Renderer.h"
#include "Level.h"
#include "AssetsManager.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// GameEngineとしてインスタンス化するためにWinMainをこのクラスに定義
	GameEngine* gameEngine = new GameEngine();

	

	int result;
	result = gameEngine->Engine::Run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	delete gameEngine;
	return result;
}


GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::Init(void)
{
	Engine::Init();
	// レンダラー生成、初期化
	this->renderer = new Renderer(this);
	renderer->InitRenderer(*GetInstanceHandle(), *GetWindowHangle(), true);


	this->assetsManager = new AssetsManager(this);

	this->assetsManager->Init();


	this->inputSystem = new InputSystem(this);
	this->inputSystem->Init();

	// ワールド生成、初期化,最後にやる
	this->world = new World(true);
	this->world->SetGameEngine(this);


	this->world->Init();


	this->gui = new GUI(this);
	this->gui->Init();
	this->gui->SetShowGUI(true);
}

void GameEngine::Update(void)
{
	Engine::Update();

	this->inputSystem->Update();
	this->gui->Update();
	this->world->Update();
}

void GameEngine::Draw(void)
{
	Engine::Draw();
	renderer->Clear();
	//this->gui->PreDraw();

	this->world->Draw();
	this->gui->Draw();

	renderer->Present();
}

void GameEngine::Uninit(void)
{
	Engine::Uninit();

	

	//this->world->Uninit();
	//delete this->world;

	//this->inputSystem->Uninit();
	//delete this->inputSystem;

	delete this->saveFile;

	//this->renderer->UninitRenderer();
	//delete this->renderer;

}

Renderer* GameEngine::GetRenderer(void)
{
	return this->renderer;
}

AssetsManager* GameEngine::GetAssetsManager(void)
{
	return this->assetsManager;
}

World* GameEngine::GetWorld(void)
{
	return this->world;
}

GUI* GameEngine::GetGUI()
{
	return this->gui;
}

SaveFile* GameEngine::GetSaveSystem(void)
{
	return this->saveFile;
}

InputSystem* GameEngine::GetInputSystem(void)
{
	return this->inputSystem;
}


void GameEngine::Pause(void)
{
}
