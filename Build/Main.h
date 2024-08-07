#pragma once
#include "Coreminimal.h"
#include "level.h"
#include "AssetsManager.h"
#include "Title.h"
#include "input.h"

#define SCREEN_WIDTH	(1920)				// 初期化ウィンドウの横幅pixel
#define SCREEN_HEIGHT	(1080)				// 初期化ウィンドウの縦幅pixel


class Renderer;
class AssetsManager;

class Main
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Main();
	~Main();

	HWND* GetWindowHangle(void);
	HINSTANCE* GetInstanceHandle(void);
	//void SetWindowType();
	int Run(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd);



	long GetMousePosX(void);
	long GetMousePosY(void);

	// 前フレームからの経過ミリ秒を取得する
	float GetDeltaTime(void);

	enum SCENE
	{
		TITLE,
		STAGE1,
		RESULT,
	};
	void SetScene(SCENE scene);
	SCENE GetScene(void);

	AssetsManager* GetAssetsManager(void);
	Renderer* GetRenderer(void);
	Input* GetInput(void);

protected:
	void Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);
private:
	HWND hWnd;
	HINSTANCE* hInstance;
	int fps;
	POINT mousePos;	//マウスのキャッシュ位置

	float deltaTime;
	float beforeTime;

	SCENE scene;

	Title* title;
	


	Renderer* renderer;
	AssetsManager* assetsManager;
	Input* input;

};

