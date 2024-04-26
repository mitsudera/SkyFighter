//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "field.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include"biru.h"
#include"sky.h"
#include"light.h"
#include"misairu.h"
#include"collision.h"
#include"sound.h"
#include"cloud.h"
#include"score.h"
#include"result.h"
#include"fade.h"
#include"title.h"
#include"tutorial.h"
#include"tree.h"
#include"battleui.h"
#include"meshfield.h"
#include"beffect.h"
#include"beffect1.h"
#include"sun.h"
#include"b36.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"GAME"		// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;

int g_Mode;
BOOL g_GameOver;
int g_Stage;
LONG stagetime[LAST_STAGE];

int crear;

BOOL stop;

BOOL infinitymode;

int totalscore;
int cnt;

BOOL end;

#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_NAME;		// デバッグ文字表示用

#endif


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
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
	HWND		hWnd;
	MSG			msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);



	// DirectXの初期化(ウィンドウを作成してから行う)
	BOOL mode = TRUE;

	int id = MessageBox(NULL, "Windowモードでプレイしますか？", "起動モード", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (id)
	{
	case IDYES:		// YesならWindowモードで起動
		mode = TRUE;
		break;
	case IDNO:		// Noならフルスクリーンモードで起動
		mode = FALSE;
		break;
	case IDCANCEL:	// CANCELなら終了
	default:
		return -1;
		break;
	}






	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
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

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif



				Update();			// 更新処理
				Draw();				// 描画処理

				if (end == TRUE)
				{
					return 0;
				}


#ifdef _DEBUG	// デバッグ版の時だけ表示する
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	g_Mode = 0;
	g_GameOver = FALSE;
	totalscore = 0;
	infinitymode = FALSE;
	g_Stage = 0;
	end = FALSE;
	stop = FALSE;
	for (int i = 0; i <LAST_STAGE ; i++)
	{
		stagetime[i] = 0;

	}


	InitRand(0, FALSE);


	// レンダラーの初期化
	InitRenderer(hInstance, hWnd, bWindow);

	InitSound(hWnd);

	InitLight();
	
	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	InitFade();

	InitCamera();

	// ライトを有効化
	SetLightEnable(TRUE);
	SetShadowEnable(TRUE);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);

	SetScat();

	//InitParticle();
	//InitParticle1();
	InitBeffect();
	InitBeffect1();

	InitFade();
	SetMode(0);



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	UninitScore();

	UninitBattleui();

	UninitSky();

	UninitCloud();

	UninitBiru();

	// エネミーの終了処理
	UninitEnemy();

	UninitB36();

	// プレイヤーの終了処理
	UninitPlayer();

	// フィールドの終了処理
	UninitField();

	UninitMeshField();

	UninitTree();


	UninitMisairu();

	UninitCamera();

	UninitFade();

	UninitSun();

	UninitBeffect();
	UninitBeffect1();

	UninitTitle();

	UninitTutorial();

	//入力の終了処理
	UninitInput();

	// レンダラーの終了処理
	UninitRenderer();

	UninitFade();
	// カメラの終了処理
	UninitCamera();


	UninitSound();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	if (GetKeyboardTrigger(DIK_L))
	{
		SetMode(MODE_TITLE);

	}
	switch (g_Mode)
	{
	case MODE_TITLE:
		// タイトル画面


		UpdateLight();

		UpdateSun();

		UpdateField();
		UpdateMeshField();
		// プレイヤーの初期化
		UpdatePlayer();
		// カメラの初期化
		UpdateCamera();

		UpdateCloud();

		UpdateSky();




		UpdateTitle();
		break;


	case MODE_TUTORIAL:
		UpdateTutorial();
		break;
	case MODE_GAME:

		UpdateScore();

		UpdateBattleui();

		// フィールドの更新処理
		UpdateField();
		UpdateMeshField();

		UpdateTree();

		UpdateFade();

		// プレイヤーの更新処理
		UpdatePlayer();
		// カメラ更新
		UpdateCamera();

		// エネミーの更新処理
		UpdateEnemy();

		UpdateB36();

		UpdateMisairu();

		UpdateBiru();

		UpdateSky();

		UpdateCloud();

		UpdateBeffect();
		UpdateBeffect1();

		UpdateLight();
		UpdateSun();

		cnt += 1;
		if (cnt==60)
		{
			stagetime[g_Stage] += 1;
			cnt = 0;
		}
		CheckHit();



		if (g_GameOver == TRUE)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}

		if (crear == -1)
		{
			crear = CheckStageClear();
			if (crear == 1)
			{
				g_Stage += 1;

				SetFade(FADE_OUT, MODE_RESULT);
			}
		}
		break;

	case MODE_RESULT:
		UpdateResult();
		break;

	case MODE_MAX:
		break;
	}


	// 入力の更新処理
	UpdateInput();


	UpdateFade();



	//SetScat();

	//SetShadowBuffer();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{



	DrawShadowMap();



	// エネミーの描画処理
	DrawEnemy();

	DrawB36();

	DrawBiru();

	DrawTree();

	// プレイヤーの描画処理
	DrawPlayer();

	DrawMisairu();







	DrawShadowMap2();



}

void DrawShadowTitle(void)
{



	DrawShadowMap();

	// プレイヤーの描画処理
	DrawPlayer();

	DrawShadowMap2();



}


void DrawObject(void)
{
	SetPolygonShader();

	// プレイヤーの描画処理
	DrawPlayer();


	// エネミーの描画処理
	DrawEnemy();

	DrawB36();

	DrawMisairu();

	DrawBiru();

	DrawTree();


	SetFieldShader();

	// フィールドの描画処理
	//DrawField();
	DrawMeshField();




}
void DrawObjectTitle(void)
{
	SetPolygonShader();

	// プレイヤーの描画処理
	DrawPlayer();




	SetFieldShader();

	// フィールドの描画処理
	//DrawField();
	DrawMeshField();




}

void DrawEfect(void)
{

	SetParticleShader();
	DrawBeffect();
	DrawBeffect1();
	DrawCloud();
	DrawSun();
}

void DrawEfectTitle(void)
{

	SetParticleShader();
	DrawCloud();
	DrawSun();
}

void DrawSkyObj(void)
{
	SetScat();
	SetSkyShader();
	DrawSky();

}


void Draw(void)
{
	// バックバッファクリア
	Clear();

	switch (g_Mode)
	{
	case MODE_TITLE:

		DrawShadowTitle();

		DrawObjectTitle();

		DrawSkyObj();

		DrawEfectTitle();








		// 2Dの物を描画する処理

		SetPolygonShader();

		SetViewPort(TYPE_FULL_SCREEN);

		// Z比較なし
		SetDepthEnable(FALSE);

		// ライティングを無効
		SetLightEnable(FALSE);

		DrawTitle();


		// ライティングを有効に
		SetLightEnable(TRUE);

		// Z比較あり
		SetDepthEnable(TRUE);

	


		break;
		
	case MODE_TUTORIAL:


		SetPolygonShader();

		SetViewPort(TYPE_FULL_SCREEN);

		// Z比較なし
		SetDepthEnable(FALSE);

		// ライティングを無効
		SetLightEnable(FALSE);

		DrawTutorial();


		// ライティングを有効に
		SetLightEnable(TRUE);

		// Z比較あり
		SetDepthEnable(TRUE);

		break;
	case MODE_GAME:

		DrawShadow();



		DrawObject();

		DrawSkyObj();

		DrawEfect();

	
		//
	// 2Dの物を描画する処理
	//
		{
			SetPolygonShader();
			SetViewPort(TYPE_FULL_SCREEN);

			// Z比較なし
			SetDepthEnable(FALSE);

			// ライティングを無効
			SetLightEnable(FALSE);

			//DrawFade();

			
			DrawBattleui();


			// 2Dスコアの描画処理
			DrawScore();

			// 他に何か２Ｄ物あればこの中に書く
			






			// ライティングを有効に
			SetLightEnable(TRUE);

			// Z比較あり
			SetDepthEnable(TRUE);

		}
		break;

	case MODE_RESULT:

		SetPolygonShader();

		SetViewPort(TYPE_FULL_SCREEN);

		// Z比較なし
		SetDepthEnable(FALSE);

		// ライティングを無効
		SetLightEnable(FALSE);

		DrawResult();


		// ライティングを有効に
		SetLightEnable(TRUE);

		// Z比較あり
		SetDepthEnable(TRUE);

		break;

	case MODE_MAX:
		break;
	}

	SetPolygonShader();
	SetViewPort(TYPE_FULL_SCREEN);

	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);

	DrawFade();

	// 他に何か２Ｄ物あればこの中に書く


	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);





	




#ifdef _DEBUG
	// デバッグ表示
	DrawDebugProc();
#endif

	// バックバッファ、フロントバッファ入れ替え
	Present();
}


long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif

void CheckHit(void)
{
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();
	MISAIRU* misairu = GetMisairu();
	BIRU* biru = GetBiru();
	B36* b36parts = GetB36Parts();
	BOOL hit;

	if (player->pos.y<0.0f)
	{
		g_GameOver = TRUE;
		SetBombEffect(player->pos);
	}
	//// レイキャストして足元の高さを求める
	//XMFLOAT3 HitPosition;		// 交点
	//XMFLOAT3 Normal;			// ぶつかったポリゴンの法線ベクトル（向き）
	//BOOL ans = RayHitField(player->pos, &HitPosition, &Normal);
	//if (ans==TRUE &&player->pos.y < HitPosition.y)
	//{
	//	g_GameOver = TRUE;
	//	SetBombEffect(player->pos);

	//}


	for (int i = 0; i < MAX_MISAIRU; i++)
	{
		if (misairu[i].use == TRUE)
		{
			switch (misairu[i].pore)
			{
			case 0:
				hit = FALSE;
				hit = CollisionRaySphire(misairu[i].oldpos, misairu[i].pos, player->pos, 100.0f);
				if (hit == TRUE)
				{
					misairu[i].use = FALSE;
					SetBombEffect(player[0].pos);
					PlaySound(SOUND_LABEL_SE_bomb001);

					player[0].hp -= 50;
					if (player[0].hp <= 0)
					{

						g_GameOver = TRUE;

					}



				}

				break;

			case 1:

				if (enemy[misairu[i].obj].use == TRUE)
				{


					hit = FALSE;
					hit = CollisionRaySphire(misairu[i].oldpos, misairu[i].pos, enemy[misairu[i].obj].pos, 100.0f);
					if (hit == TRUE)
					{
						misairu[i].use = FALSE;
						SetBombEffect(enemy[misairu[i].obj].pos);
						PlaySound(SOUND_LABEL_SE_bomb001);

						enemy[misairu[i].obj].hp -= 50;
						SetHit();
						if (enemy[misairu[i].obj].hp <= 0)
						{
							enemy[misairu[i].obj].use = FALSE;
							SetDestroy();
							AddScore(100);
							if (infinitymode==TRUE)
							{
								SetEnemy(rand()%3);
							}

						}
					}

				}
				else
				{
					misairu[i].pore=-1;
				}
				break;

			case 2:

				if (b36parts[misairu[i].obj].breaked == FALSE)
				{


					hit = FALSE;
					hit = CollisionRaySphire(misairu[i].oldpos, misairu[i].pos, b36parts[misairu[i].obj].wpos, 100.0f);
					if (hit == TRUE)
					{
						misairu[i].use = FALSE;
						SetBombEffect(b36parts[misairu[i].obj].wpos);
						PlaySound(SOUND_LABEL_SE_bomb001);

						b36parts[misairu[i].obj].hp -= 50;
						SetHit();
						if (b36parts[misairu[i].obj].hp <= 0)
						{
							b36parts[misairu[i].obj].breaked = TRUE;
							SetDestroy();
							AddScore(100);

						}
					}

				}
				else
				{
					misairu[i].pore=-1;
				}
				break;


			default:
				break;
			}





		}

	}



}

void SetMode(int mode)
{
	// モードを変える前に全部メモリを解放しちゃう
	StopSound();

	UninitTitle();

	UninitScore();

	UninitBattleui();

	UninitSky();

	UninitCloud();

	UninitBiru();

	// エネミーの終了処理
	UninitEnemy();

	UninitB36();

	// プレイヤーの終了処理
	UninitPlayer();


	UninitTree();
	// フィールドの終了処理
	UninitField();
	UninitMeshField();


	UninitMisairu();

	UninitSun();


	UninitBeffect();
	UninitBeffect1();

	UninitResult();

	UninitTutorial();


	g_Mode = mode;	// 次のモードをセットしている

	switch (g_Mode)
	{
	case MODE_TITLE:
		//SetFade(FADE_IN, 0);
		// タイトル画面の初期化
		InitTitle();

		g_GameOver = FALSE;
		totalscore = 0;
		infinitymode = FALSE;

		g_Stage = 0;

		cnt = 0;


		InitSun();
		SetSunPos(XMFLOAT3(0.0f, 0.08f, -1.0f));

		InitField(XMFLOAT3(0.0f, 900000.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 256, 256, 10000.0f, 10000.0f);

		InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 256, 256, 10000.0f, 10000.0f);
		// プレイヤーの初期化
		InitPlayer();
		// カメラの初期化
		InitCamera();

		InitCloud();

		InitSky();


		for (int i = 0; i < LAST_STAGE; i++)
		{
			stagetime[i] = 0;

		}

		PlaySound(SOUND_LABEL_BGM_maou1);


		break;

	case MODE_TUTORIAL:
		//SetFade(FADE_IN, 1);

		InitTutorial();
		break;
	case MODE_GAME:

		crear = -1;

		//SetFade(FADE_IN, 3);
		// フィールドの初期化
		//InitField(XMFLOAT3(0.0f, 900000.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 256, 256, 10000.0f, 10000.0f);
		InitField(XMFLOAT3(0.0f, 50000.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 256, 256, 10000.0f, 10000.0f);
		InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 256, 256, 10000.0f, 10000.0f);

		InitTree();

		// プレイヤーの初期化
		InitPlayer();
		// カメラの初期化
		InitCamera();

		// エネミーの初期化
		InitEnemy();

		InitB36();

		InitCloud();

		InitMisairu();

		InitBiru();

		InitSky();

		InitBeffect();
		InitBeffect1();

		InitScore();

		InitSun();

		InitBattleui();

		PlaySound(SOUND_LABEL_BGM_maou2);


		SetStage(g_Stage);

		break;

	case MODE_RESULT:

		SetFade(FADE_IN, MODE_RESULT);
		InitResult();

		break;

	case MODE_MAX:
		break;
	}
}

int CheckStageClear(void)
{
	int crear = 1;


	ENEMY* enemy = GetEnemy();

	B36* b36 = GetB36();

	if (b36->use==TRUE)
	{
		crear = -1;
		return crear;
	}

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use==TRUE)
		{
			crear = -1;
			return crear;
		}


	}
	return crear;
}

void InitRand(int seed, BOOL tin)
{
	if (tin==TRUE)
	{
		//ランダムの初期化処理
		srand((unsigned)time(NULL));

	}
	else
	{
		//ランダムの初期化処理
		srand((unsigned)seed);

	}

}

void SetStage(int stage)
{


	switch (g_Stage)
	{


	case 0:

		//SetB36(XMFLOAT3(0.0f, 60000.0f, 10000.0f));


		for (int i = 0; i <2 ; i++)
		{
			SetEnemy(0);

		}
		break;

	case 1:


		for (int i = 0; i < 4; i++)
		{

			SetEnemy(1);

		}



		break;

	case 2:


		for (int i = 0; i < 6; i++)
		{

			SetEnemy(2);

		}



		break;



	default:
		break;
	}


}

void ChangeStage(int stage)
{
	g_Stage = stage;

}

int GetStage(void)
{
	return g_Stage;
}

BOOL CheckClear(void)
{
	BOOL ans = FALSE;
	
	if (g_Stage == LAST_STAGE)
	{
		ans = TRUE;
	}
	return ans;
}

LONG GetTotalTime(void)
{
	LONG ans = 0;
	for (int i = 0; i < LAST_STAGE; i++)
	{
		ans += stagetime[i];
	}


	return ans;
}

LONG GetStageTime(int stage)
{
	return stagetime[stage];
}

void AddTime(int stage)
{
	stagetime[stage] += 1;


}

BOOL CGmaeOver(void)
{
	return g_GameOver;

}

BOOL GetInfMode(void)
{
	return infinitymode;
}
void SetInfMode(BOOL flag)
{
	infinitymode=flag;
}
void SetEnd(void)
{
	end = TRUE;
}

void Stop(void)
{
	if (stop==FALSE)
	{
		stop = TRUE;

		return;

	}
	else
	{
		stop = FALSE;
		return;
	}

}

int GetMode(void)
{
	return g_Mode;
}