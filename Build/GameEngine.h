//=============================================================================
//
// 
// Author : 
//
//=============================================================================
#pragma once
#include "Engine.h"

// 前方宣言 (これによりincludeが不要になる)
class Renderer;
class World;
class AssetsManager;
class GUI;
class SaveFile;
class InputSystem;
// ゲームエンジンクラス
class GameEngine : public Engine
{
public:
	GameEngine();
	~GameEngine();
	Renderer* GetRenderer(void);
	AssetsManager* GetAssetsManager(void);
	World* GetWorld(void);
	GUI* GetGUI();
	SaveFile* GetSaveSystem(void);
	InputSystem* GetInputSystem(void);

	void Play(void);
	void Pause(void);
	void End(void);
protected:
	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Uninit(void) override;
private:
	Renderer* renderer;
	World* world;
	AssetsManager* assetsManager;
	GUI* gui;
	SaveFile* saveFile;
	InputSystem* inputSystem;
};