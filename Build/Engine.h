//=============================================================================
//
// 
// Author : 
//
//=============================================================================
#pragma once
#include "CoreMinimal.h"

enum INPUTMODE	// 入力モード
{
	INPUTMODE_NONE,
	INPUTMODE_UIONLY,
	INPUTMODE_GAMEONLY
};

class Engine
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Engine();
	~Engine();
	HWND* GetWindowHangle(void);
	HINSTANCE* GetInstanceHandle(void);
	XMFLOAT2 GetWindowSize(void);
	void SetWindowSize(float width, float height);
	//void SetWindowType();

	int Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	int GetFps(void);
	void SetFps(int fps);
	
	long GetMousePosX(void);
	long GetMousePosY(void);

	void SetInputModeGameOnly(void);
	void SetInputModeUIOnly(void);
	INPUTMODE GetInputMode(void);

	// 前フレームからの経過ミリ秒を取得する
	float GetDeltaTime(void);
protected:
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Uninit(void);
private:
	HWND hWnd;
	HINSTANCE* hInstance;
	int fps;
	POINT mousePos;	//マウスのキャッシュ位置
	INPUTMODE inputMode;

	float deltaTime;
	float beforeTime;

	XMFLOAT2 screenSize;
};
