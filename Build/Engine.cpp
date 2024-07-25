//=============================================================================
//
// 
//
//=============================================================================
#include "Engine.h"



//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"SkyFighter"				// ウインドウのキャプション名

#define SCREEN_WIDTH	(1980)				// 初期化ウィンドウの横幅pixel
#define SCREEN_HEIGHT	(1020)				// 初期化ウィンドウの縦幅pixel

long g_mouseX = 0;
long g_mouseY = 0;

Engine::Engine()
{
	screenSize.x = SCREEN_WIDTH;
	screenSize.y = SCREEN_HEIGHT;
}

Engine::~Engine()
{
}

/// <summary>
/// エンジン起動 メイン関数のようなもの
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int Engine::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// インスタンスハンドルのポインターを保持
	this->hInstance = &hInstance;

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;


	// ウィンドウクラスの初期化
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		Engine::WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};


	MSG			msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW, //WS_POPUPタイトルバー消す
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);


	// DirectXの初期化(ウィンドウを作成してから行う)
	Init();
	//if (FAILED(Init(hInstance, hWnd, mode)))
	//{
	//	return -1;
	//}

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// 経過時間
	this->deltaTime = 0;
	this->beforeTime = timeGetTime();

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000.0f)	// 1秒ごとに実行
			{
				//g_CountFPS = dwFrameCount;
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000.0f / this->fps))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

				this->deltaTime = timeGetTime() - this->beforeTime;
				this->beforeTime = timeGetTime();

				Update();			// 更新処理
				Draw();				// 描画処理
				dwFrameCount++;
			}
		}
	}
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();
	return (int)msg.wParam;
}

XMFLOAT2 Engine::GetWindowSize(void)
{
	//switch (resolution)
	//{
	//case RESOLUTION_nHD:
	//	g_screenWidth = 640;
	//	g_screenHeight = 360;
	//	break;
	//case RESOLUTION_qHD:
	//	g_screenWidth = 960;
	//	g_screenHeight = 540;
	//	break;
	//case RESOLUTION_HD:
	//	g_screenWidth = 1280;
	//	g_screenHeight = 720;
	//	break;
	//case RESOLUTION_HDPlus:
	//	g_screenWidth = 1600;
	//	g_screenHeight = 900;
	//	break;
	//case RESOLUTION_FullHD:
	//	g_screenWidth = 1920;
	//	g_screenHeight = 1080;
	//	break;
	//default:
	//	break;
	//}
	return screenSize;
}

void Engine::SetWindowSize(float width, float height)
{

	screenSize.x = width;
	screenSize.y = height;
	
}

HINSTANCE* Engine::GetInstanceHandle(void)
{
	return this->hInstance;
}

/// <summary>
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
HWND* Engine::GetWindowHangle(void)
{
	return &this->hWnd;
}


int Engine::GetFps(void)
{
	return this->fps;
}

void Engine::SetFps(int fps)
{
	this->fps = fps;
}

/// <summary>
/// プロシージャ
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK Engine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{



	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_mouseX = LOWORD(lParam);
		g_mouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

float Engine::GetDeltaTime(void)
{
	return this->deltaTime / 1000.0f;
}

void Engine::Init(void)
{
	this->fps = 60;

	//マウス位置の初期化
	GetCursorPos(&this->mousePos);
}

void Engine::Update(void)
{
}

void Engine::Draw(void)
{
}

void Engine::Uninit(void)
{
}

INPUTMODE Engine::GetInputMode(void)
{
	return INPUTMODE();
}


//=============================================================================
// マウスの横の移動量を取得する
//=============================================================================
long Engine::GetMousePosX(void)
{
	return g_mouseX;
}

//=============================================================================
// マウスの縦の移動量を取得する
//=============================================================================
long Engine::GetMousePosY(void)
{
	return g_mouseY;
}


//=============================================================================
// 入力モードをGAMEのみにする
//=============================================================================
void Engine::SetInputModeGameOnly(void)
{
	if (inputMode == INPUTMODE_UIONLY) return;
	inputMode = INPUTMODE_UIONLY;
	while (ShowCursor(FALSE) == FALSE);

	// マウス移動範囲の取得
	RECT rc;
	GetClipCursor(&rc);

	//cache
	GetCursorPos(&this->mousePos);

	RECT rect;
	GetWindowRect(hWnd, &rect);
	XMFLOAT2 res = GetWindowSize();
	// マウス移動範囲の設定
	rc.left = rect.left + (rect.right - rect.left) / (LONG)2.0f;	// 左上隅のX座標
	rc.right = rc.left;					// 右下隅のX座標
	rc.top = rect.top + (rect.bottom - rect.top) / (LONG)2.0f;	// 左上隅のY座標
	rc.bottom = rc.top;						// 右下隅のY座標	
	ClipCursor(&rc);


}

//=============================================================================
// 入力モードをUIのみにする
//=============================================================================
void Engine::SetInputModeUIOnly(void)
{
	if (inputMode == INPUTMODE_GAMEONLY) return;
	inputMode = INPUTMODE_GAMEONLY;
	while (ShowCursor(TRUE) == FALSE);

	// マウス移動範囲の解除
	ClipCursor(NULL);

	SetCursorPos(this->mousePos.x, this->mousePos.y);
}