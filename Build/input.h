//=============================================================================
//
// 入力処理 [input.h]
// Author : 
//
//=============================================================================
#pragma once
#include "Coreminimal.h"


#define	NUM_KEY_MAX			(256)

// game pad用設定値
#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
#define DEADZONER		1000			// 各軸の10%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値
#define RANGE_MAXRight	1500			// 有効範囲の最大値
#define RANGE_MINRight	-1500			// 有効範囲の最小値


/* game pad情報 */

#define BUTTON_UP		0x00000001l	// 方向キー上(.rgdwPOV<0)
#define BUTTON_DOWN		0x00000002l	// 方向キー下(.rgdwPOV>0)
#define BUTTON_LEFT		0x00000004l	// 方向キー左(.rgdwPOV<0)
#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.rgdwPOV>0)
#define BUTTON_A		0x00000010l	// □ボタン(.rgbButtons[0]&0x80)□
#define BUTTON_B		0x00000020l	// ×ボタン(.rgbButtons[1]&0x80)×
#define BUTTON_C		0x00000040l	// 〇ボタン(.rgbButtons[2]&0x80)〇
#define BUTTON_X		0x00000080l	// △ボタン(.rgbButtons[3]&0x80)△
#define BUTTON_Y		0x00000100l	// Lボタン(.rgbButtons[4]&0x80)L
#define BUTTON_Z		0x00000200l	// Rボタン(.rgbButtons[5]&0x80)R
#define BUTTON_L		0x00000400l	// L2ボタン(.rgbButtons[6]&0x80)L2
#define BUTTON_R		0x00000800l	// R2ボタン(.rgbButtons[7]&0x80)R2
#define BUTTON_START	0x00001000l	// shareボタン(.rgbButtons[8]&0x80)share
#define BUTTON_M		0x00002000l	// optionボタン(.rgbButtons[9]&0x80)option
#define BUTTON_L3		0x00004000l	// L3ボタン(.rgbButtons[8]&0x80)L3
#define BUTTON_R3		0x00008000l	// R3ボタン(.rgbButtons[9]&0x80)
#define BUTTON_PS		0x00008000l	// PSボタン(.rgbButtons[9]&0x80)PS
#define BUTTON_PAD		0x00010000l	// PADボタン(.rgbButtons[9]&0x80)PS
#define GAMEPADMAX		4			// 同時に接続するジョイパッドの最大数をセット


class Main;

class Input
{
public:
	Input(Main* main);
	~Input();
	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	void Update(void);

	HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
	void UninitKeyboard(void);
	HRESULT UpdateKeyboard(void);

	HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // マウスの初期化
	void UninitMouse();						// マウスの終了処理
	HRESULT UpdateMouse();					// マウスの更新処理

	HRESULT InitializePad(void);			// パッド初期化
	void UpdatePad(void);
	void UninitPad(void);


	//---------------------------- keyboard
	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRepeat(int nKey);
	bool GetKeyboardRelease(int nKey);

	//---------------------------- mouse
	BOOL IsMouseLeftPressed(void);      // 左クリックした状態
	BOOL IsMouseLeftTriggered(void);    // 左クリックした瞬間
	BOOL IsMouseRightPressed(void);     // 右クリックした状態
	BOOL IsMouseRightTriggered(void);   // 右クリックした瞬間
	BOOL IsMouseCenterPressed(void);    // 中クリックした状態
	BOOL IsMouseCenterTriggered(void);  // 中クリックした瞬間
	long GetMouseX(void);               // マウスがX方向に動いた相対値
	long GetMouseY(void);               // マウスがY方向に動いた相対値
	long GetMouseZ(void);               // マウスホイールが動いた相対値

	//---------------------------- game pad
	BOOL IsButtonPressed(int padNo, DWORD button);
	BOOL IsButtonTriggered(int padNo, DWORD button);


	LONG GetLeftStickX(int padNo);
	LONG GetLeftStickY(int padNo);
	LONG GetRightStickX(int padNo);
	LONG GetRightStickY(int padNo);
	LONG GetVALUER2(int padNo);
	LONG GetVALUEL2(int padNo);




private:

	//------------------------------- keyboard
	LPDIRECTINPUT8			pDInput;					// IDirectInput8インターフェースへのポインタ
	LPDIRECTINPUTDEVICE8	pDIDevKeyboard;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
	BYTE					keyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
	BYTE					keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
	BYTE					keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
	BYTE					keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
	int						keyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

	//--------------------------------- mouse
	LPDIRECTINPUTDEVICE8 pMouse; // mouse

	DIMOUSESTATE2   mouseState;		// マウスのダイレクトな状態
	DIMOUSESTATE2   mouseTrigger;	// 押された瞬間だけON

	//--------------------------------- game pad

	LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX];// パッドデバイス

	DWORD	padState[GAMEPADMAX];	// パッド情報（複数対応）
	DWORD	padTrigger[GAMEPADMAX];

	int		padCount;			// 検出したパッドの数
	LONG LeftStickY[GAMEPADMAX];
	LONG LeftStickX[GAMEPADMAX];
	LONG RightStickY[GAMEPADMAX];
	LONG RightStickX[GAMEPADMAX];
	LONG VALUER2[GAMEPADMAX];
	LONG VALUEL2[GAMEPADMAX];



};




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
