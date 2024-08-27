#include "Main.h"
#include "renderer.h"


#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"SkyFighter"				// ウインドウのキャプション名

long g_mouseX = 0;
long g_mouseY = 0;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	Main* main = new Main;


	int result;
	result = main->Run(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	delete main;
	return result;
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

			if ((dwCurrentTime - dwExecLastTime) >= (1000.0f / 60))	// 1/60秒ごとに実行
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
	this->title->Uninit();

	this->scene = scene;
	switch (scene)
	{
	case TITLE:
		this->title->Init();
		break;

	default:
		break;
	}

}

Main::SCENE Main::GetScene(void)
{
	return this->scene;
}

AssetsManager* Main::GetAssetsManager(void)
{
	return this->assetsManager;
}

Renderer* Main::GetRenderer(void)
{
	return this->renderer;
}

Input* Main::GetInput(void)
{
	return this->input;
}

void Main::Init(void)
{
	//マウス位置の初期化
	GetCursorPos(&this->mousePos);

	srand((unsigned)time(NULL));


	//Renderer生成
	this->renderer = new Renderer(this);

	this->renderer->InitRenderer(*GetInstanceHandle(), *GetWindowHangle(), true);

	this->assetsManager = new AssetsManager(this);

	this->input = new Input(this);

	this->input->Init(*GetInstanceHandle(), *GetWindowHangle());

	//各レベルの生成

	this->title = new Title(this);

	SetScene(SCENE::TITLE);
	

}

void Main::Update(void)
{
	this->input->Update();
	switch (this->scene)
	{
	case SCENE::TITLE:
		this->title->Update();
		break;
	case SCENE::STAGE1:
		break;
	case SCENE::RESULT:
		break;
	default:
		break;
	}
}

void Main::Draw(void)
{
	renderer->Clear();


	switch (this->scene)
	{
	case SCENE::TITLE:
		this->title->Draw();
		break;
	case SCENE::STAGE1:
		break;
	case SCENE::RESULT:
		break;
	default:
		break;
	}

	renderer->Present();

}

void Main::Uninit(void)
{
	this->input->Uninit();
	this->title->Uninit();
	this->assetsManager->Uninit();
	this->renderer->UninitRenderer();


	delete input;
	delete assetsManager;
	delete title;
	delete renderer;

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
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

