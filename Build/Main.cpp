#include "Main.h"
#include "renderer.h"


#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"SkyFighter"				// ウインドウのキャプション名

long g_mouseX = 0;
long g_mouseY = 0;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// GameEngineとしてインスタンス化するためにWinMainをこのクラスに定義
	Main* main = new Main;


	int result;
	result = main->Run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	delete main;
	return result;
}

LRESULT Main::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

Main::Main()
{
}

Main::~Main()
{
}

HWND* Main::GetWindowHangle(void)
{
	return &this->hWnd;

}

HINSTANCE* Main::GetInstanceHandle(void)
{
	return this->hInstance;

}

int Main::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
		Main::WndProc,
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


long Main::GetMousePosX(void)
{
	return g_mouseX;
}

long Main::GetMousePosY(void)
{
	return g_mouseY;
}

float Main::GetDeltaTime(void)
{
	return this->deltaTime / 1000.0f;
}

void Main::SetScene(SCENE scene)
{

	this->scene = scene;
	this->levelList[scene]->Init();

}

Main::SCENE Main::GetScene(void)
{
	return this->scene;
}

void Main::Init(void)
{
	//Renderer生成
	this->renderer = new Renderer(this);

	this->renderer->InitRenderer(*GetInstanceHandle(), *GetWindowHangle(), true);

	//各レベルの生成
	Level* title = new Level(this);
	levelList.push_back(title);
	Level* stage1 = new Level(this);
	levelList.push_back(stage1);
	Level* result = new Level(this);
	levelList.push_back(result);

	SetScene(SCENE::TITLE);
	

}

void Main::Update(void)
{

	this->levelList[this->scene]->Update();
}

void Main::Draw(void)
{
	this->levelList[this->scene]->Draw();

}

void Main::Uninit(void)
{
	for (int i = 0; i < levelList.size(); i++)
	{
		if (levelList[i]!=nullptr)
		{
			levelList[i]->Uninit();
			delete levelList[i];
			levelList[i] = nullptr;

		}
	}
	this->renderer->UninitRenderer();
	delete this->renderer;

}
