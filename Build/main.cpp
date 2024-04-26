//=============================================================================
//
// ���C������ [main.cpp]
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
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"GAME"		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�:
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
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_NAME;		// �f�o�b�O�����\���p

#endif


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
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
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// �E�B���h�E�̍����W
		CW_USEDEFAULT,																		// �E�B���h�E�̏���W
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// �E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);



	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	BOOL mode = TRUE;

	int id = MessageBox(NULL, "Window���[�h�Ńv���C���܂����H", "�N�����[�h", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (id)
	{
	case IDYES:		// Yes�Ȃ�Window���[�h�ŋN��
		mode = TRUE;
		break;
	case IDNO:		// No�Ȃ�t���X�N���[�����[�h�ŋN��
		mode = FALSE;
		break;
	case IDCANCEL:	// CANCEL�Ȃ�I��
	default:
		return -1;
		break;
	}






	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif



				Update();			// �X�V����
				Draw();				// �`�揈��

				if (end == TRUE)
				{
					return 0;
				}


#ifdef _DEBUG	// �f�o�b�O�ł̎������\������
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// ����\��߂�

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
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
// ����������
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


	// �����_���[�̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	InitSound(hWnd);

	InitLight();
	
	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	InitFade();

	InitCamera();

	// ���C�g��L����
	SetLightEnable(TRUE);
	SetShadowEnable(TRUE);

	// �w�ʃ|���S�����J�����O
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
// �I������
//=============================================================================
void Uninit(void)
{
	UninitScore();

	UninitBattleui();

	UninitSky();

	UninitCloud();

	UninitBiru();

	// �G�l�~�[�̏I������
	UninitEnemy();

	UninitB36();

	// �v���C���[�̏I������
	UninitPlayer();

	// �t�B�[���h�̏I������
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

	//���͂̏I������
	UninitInput();

	// �����_���[�̏I������
	UninitRenderer();

	UninitFade();
	// �J�����̏I������
	UninitCamera();


	UninitSound();
}

//=============================================================================
// �X�V����
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
		// �^�C�g�����


		UpdateLight();

		UpdateSun();

		UpdateField();
		UpdateMeshField();
		// �v���C���[�̏�����
		UpdatePlayer();
		// �J�����̏�����
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

		// �t�B�[���h�̍X�V����
		UpdateField();
		UpdateMeshField();

		UpdateTree();

		UpdateFade();

		// �v���C���[�̍X�V����
		UpdatePlayer();
		// �J�����X�V
		UpdateCamera();

		// �G�l�~�[�̍X�V����
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


	// ���͂̍X�V����
	UpdateInput();


	UpdateFade();



	//SetScat();

	//SetShadowBuffer();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{



	DrawShadowMap();



	// �G�l�~�[�̕`�揈��
	DrawEnemy();

	DrawB36();

	DrawBiru();

	DrawTree();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	DrawMisairu();







	DrawShadowMap2();



}

void DrawShadowTitle(void)
{



	DrawShadowMap();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	DrawShadowMap2();



}


void DrawObject(void)
{
	SetPolygonShader();

	// �v���C���[�̕`�揈��
	DrawPlayer();


	// �G�l�~�[�̕`�揈��
	DrawEnemy();

	DrawB36();

	DrawMisairu();

	DrawBiru();

	DrawTree();


	SetFieldShader();

	// �t�B�[���h�̕`�揈��
	//DrawField();
	DrawMeshField();




}
void DrawObjectTitle(void)
{
	SetPolygonShader();

	// �v���C���[�̕`�揈��
	DrawPlayer();




	SetFieldShader();

	// �t�B�[���h�̕`�揈��
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
	// �o�b�N�o�b�t�@�N���A
	Clear();

	switch (g_Mode)
	{
	case MODE_TITLE:

		DrawShadowTitle();

		DrawObjectTitle();

		DrawSkyObj();

		DrawEfectTitle();








		// 2D�̕���`�悷�鏈��

		SetPolygonShader();

		SetViewPort(TYPE_FULL_SCREEN);

		// Z��r�Ȃ�
		SetDepthEnable(FALSE);

		// ���C�e�B���O�𖳌�
		SetLightEnable(FALSE);

		DrawTitle();


		// ���C�e�B���O��L����
		SetLightEnable(TRUE);

		// Z��r����
		SetDepthEnable(TRUE);

	


		break;
		
	case MODE_TUTORIAL:


		SetPolygonShader();

		SetViewPort(TYPE_FULL_SCREEN);

		// Z��r�Ȃ�
		SetDepthEnable(FALSE);

		// ���C�e�B���O�𖳌�
		SetLightEnable(FALSE);

		DrawTutorial();


		// ���C�e�B���O��L����
		SetLightEnable(TRUE);

		// Z��r����
		SetDepthEnable(TRUE);

		break;
	case MODE_GAME:

		DrawShadow();



		DrawObject();

		DrawSkyObj();

		DrawEfect();

	
		//
	// 2D�̕���`�悷�鏈��
	//
		{
			SetPolygonShader();
			SetViewPort(TYPE_FULL_SCREEN);

			// Z��r�Ȃ�
			SetDepthEnable(FALSE);

			// ���C�e�B���O�𖳌�
			SetLightEnable(FALSE);

			//DrawFade();

			
			DrawBattleui();


			// 2D�X�R�A�̕`�揈��
			DrawScore();

			// ���ɉ����Q�c������΂��̒��ɏ���
			






			// ���C�e�B���O��L����
			SetLightEnable(TRUE);

			// Z��r����
			SetDepthEnable(TRUE);

		}
		break;

	case MODE_RESULT:

		SetPolygonShader();

		SetViewPort(TYPE_FULL_SCREEN);

		// Z��r�Ȃ�
		SetDepthEnable(FALSE);

		// ���C�e�B���O�𖳌�
		SetLightEnable(FALSE);

		DrawResult();


		// ���C�e�B���O��L����
		SetLightEnable(TRUE);

		// Z��r����
		SetDepthEnable(TRUE);

		break;

	case MODE_MAX:
		break;
	}

	SetPolygonShader();
	SetViewPort(TYPE_FULL_SCREEN);

	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	DrawFade();

	// ���ɉ����Q�c������΂��̒��ɏ���


	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);





	




#ifdef _DEBUG
	// �f�o�b�O�\��
	DrawDebugProc();
#endif

	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
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
	//// ���C�L���X�g���đ����̍��������߂�
	//XMFLOAT3 HitPosition;		// ��_
	//XMFLOAT3 Normal;			// �Ԃ������|���S���̖@���x�N�g���i�����j
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
	// ���[�h��ς���O�ɑS������������������Ⴄ
	StopSound();

	UninitTitle();

	UninitScore();

	UninitBattleui();

	UninitSky();

	UninitCloud();

	UninitBiru();

	// �G�l�~�[�̏I������
	UninitEnemy();

	UninitB36();

	// �v���C���[�̏I������
	UninitPlayer();


	UninitTree();
	// �t�B�[���h�̏I������
	UninitField();
	UninitMeshField();


	UninitMisairu();

	UninitSun();


	UninitBeffect();
	UninitBeffect1();

	UninitResult();

	UninitTutorial();


	g_Mode = mode;	// ���̃��[�h���Z�b�g���Ă���

	switch (g_Mode)
	{
	case MODE_TITLE:
		//SetFade(FADE_IN, 0);
		// �^�C�g����ʂ̏�����
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
		// �v���C���[�̏�����
		InitPlayer();
		// �J�����̏�����
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
		// �t�B�[���h�̏�����
		//InitField(XMFLOAT3(0.0f, 900000.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 256, 256, 10000.0f, 10000.0f);
		InitField(XMFLOAT3(0.0f, 50000.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 256, 256, 10000.0f, 10000.0f);
		InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 256, 256, 10000.0f, 10000.0f);

		InitTree();

		// �v���C���[�̏�����
		InitPlayer();
		// �J�����̏�����
		InitCamera();

		// �G�l�~�[�̏�����
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
		//�����_���̏���������
		srand((unsigned)time(NULL));

	}
	else
	{
		//�����_���̏���������
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