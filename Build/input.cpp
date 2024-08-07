//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************





Input::Input(Main* main)
{
}

Input::~Input()
{
}

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT Input::Init(HINSTANCE hInst, HWND hWnd)
{
	pDInput = NULL;
	pDIDevKeyboard = NULL;
	pMouse = NULL;
	for (int i = 0; i < GAMEPADMAX; i++)
	{
		pGamePad[i] = NULL;

	}
	padCount = 0;

	HRESULT hr;

	if(!pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&pDInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

 	// マウスの初期化
	InitializeMouse(hInst, hWnd);
	
	// パッドの初期化
	InitializePad();

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void Input:: Uninit(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	UninitMouse();

	// パッドの終了処理
	UninitPad();

	if(pDInput)
	{
		pDInput->Release();
		pDInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void Input::Update(void)
{
	// キーボードの更新
	UpdateKeyboard();
	
	// マウスの更新
	UpdateMouse();
	
	// パッドの更新
	UpdatePad();

}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT Input::InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIDevKeyboard, NULL);
	if(FAILED(hr) || pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void Input:: UninitKeyboard(void)
{
	if(pDIDevKeyboard)
	{
		pDIDevKeyboard->Release();
		pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新
//=============================================================================
HRESULT Input::UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = pDIDevKeyboard->GetDeviceState(sizeof(keyState), keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			keyStateTrigger[cnt] = (keyStateOld[cnt] ^ keyState[cnt]) & keyState[cnt];
			keyStateRelease[cnt] = (keyStateOld[cnt] ^ keyState[cnt]) & ~keyState[cnt];
			keyStateRepeat[cnt] = keyStateTrigger[cnt];

			if(keyState[cnt])
			{
				keyStateRepeatCnt[cnt]++;
				if(keyStateRepeatCnt[cnt] >= 20)
				{
					keyStateRepeat[cnt] = keyState[cnt];
				}
			}
			else
			{
				keyStateRepeatCnt[cnt] = 0;
				keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool Input::GetKeyboardPress(int key)
{
	return (keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool Input::GetKeyboardTrigger(int key)
{
	return (keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool Input::GetKeyboardRepeat(int key)
{
	return (keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool Input::GetKeyboardRelease(int key)
{
	return (keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// マウス関係の処理
//=============================================================================
// マウスの初期化
HRESULT Input::InitializeMouse(HINSTANCE hInst,HWND hWindow)
{
	HRESULT result;
	// デバイス作成
	result = pDInput->CreateDevice(GUID_SysMouse,&pMouse,NULL);
	if(FAILED(result) || pMouse==NULL)
	{
		MessageBox(hWindow,"No mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// データフォーマット設定
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Can't setup mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// 他のアプリと協調モードに設定
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse mode error","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	
	// デバイスの設定
	DIPROPDWORD prop;
	
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値

	result = pMouse->SetProperty(DIPROP_AXISMODE,&prop.diph);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse property error","Warning",MB_OK | MB_ICONWARNING);
		return result;	
	}
	
	// アクセス権を得る
	pMouse->Acquire();
	return result;
}
//---------------------------------------------------------
void Input::UninitMouse()
{
	if(pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = NULL;
	}

}
//-----------------------------------------------------------
HRESULT Input::UpdateMouse()
{
	HRESULT result;
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = mouseState;
	// データ取得
	result = pMouse->GetDeviceState(sizeof(mouseState),&mouseState);
	if(SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// マウスのボタン状態
		for(int i=0;i<8;i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		result = pMouse->Acquire();
	}
	return result;
	
}

//----------------------------------------------
BOOL Input::IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80) ? true : false;	// 押されたときに立つビットを検査
}
BOOL Input::IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80) ? true : false;
}
BOOL Input::IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80) ? true : false;
}
BOOL Input::IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80) ? true : false;
}
BOOL Input::IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80) ? true : false;
}
BOOL Input::IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80) ? true : false;
}
//------------------
long Input::GetMouseX(void)
{
	return mouseState.lX;
}
long Input::GetMouseY(void)
{
	return mouseState.lY;
}
long Input::GetMouseZ(void)
{
	return mouseState.lZ;
}
//================================================= game pad
//---------------------------------------- コールバック関数
BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID )
{

	return DIENUM_CONTINUE;	// 次のデバイスを列挙

}
//---------------------------------------- 初期化
HRESULT Input::InitializePad(void)			// パッド初期化
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// ジョイパッドを探す
	pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchPadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for ( i=0 ; i<padCount ; i++ ) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(result) )
			return FALSE; // データフォーマットの設定に失敗

		// モードを設定（フォアグラウンド＆非排他モード）
//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // モードの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_X; 
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.lMin = RANGE_MINRight;
		diprg.lMax = RANGE_MAXRight;

		// Z軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_Z;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Zr軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_RZ;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Xr軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_RX;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Yr軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_RY;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_X;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		dipdw.dwData = DEADZONER;

		//Z軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_Z;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//RZ軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_RZ;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//XR軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_RX;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//YR軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_RY;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		//ジョイスティック入力制御開始
		pGamePad[i]->Acquire();
	}
		
	return S_OK;

}
//------------------------------------------- 終了処理
void Input::UninitPad(void)
{
	for (int i=0 ; i<GAMEPADMAX ; i++) {
		if ( pGamePad[i] )
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}

}

//------------------------------------------ 更新
void Input::UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// 初期化

		result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* 十字キー上
		if (dijs.rgdwPOV[0] == 0 || dijs.rgdwPOV[0] == 4500 || dijs.rgdwPOV[0] == 31500)						padState[i] |= BUTTON_UP;
		//* 十字キー右
		if (dijs.rgdwPOV[0] == 9000 || dijs.rgdwPOV[0] == 4500 || dijs.rgdwPOV[0] == 13500)						padState[i] |= BUTTON_RIGHT;
		//* 十字キー下
		if (dijs.rgdwPOV[0] == 18000 || dijs.rgdwPOV[0] == 13500 || dijs.rgdwPOV[0] == 22500)					padState[i] |= BUTTON_DOWN;
		//* 十字キー左
		if (dijs.rgdwPOV[0] == 27000 || dijs.rgdwPOV[0] == 22500 || dijs.rgdwPOV[0] == 31500)					padState[i] |= BUTTON_LEFT;
		//* Ａボタン
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		//* Ｂボタン
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		//* Ｃボタン
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		//* Ｘボタン
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		//* Ｙボタン
		if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		//* Ｚボタン
		if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		//* Ｌボタン
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		//* Ｒボタン
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;
		//* ＳＴＡＲＴボタン
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_START;
		//* Ｍボタン
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_M;
		//* 
		if (dijs.rgbButtons[10] & 0x80)	padState[i] |= BUTTON_L3;
		//* Ｍボタン
		if (dijs.rgbButtons[11] & 0x80)	padState[i] |= BUTTON_R3;

		if (dijs.rgbButtons[12] & 0x80)	padState[i] |= BUTTON_PS;

		if (dijs.rgbButtons[13] & 0x80)	padState[i] |= BUTTON_PAD;

		LeftStickX[i] = dijs.lX;
		LeftStickY[i] = dijs.lY;
		RightStickX[i] = dijs.lZ;
		RightStickY[i] = dijs.lRz;

		VALUER2[i] = dijs.lRx;
		VALUEL2[i] = dijs.lRy;

		// Trigger設定
		padTrigger[i] = ((lastPadState ^ padState[i])	// 前回と違っていて
			& padState[i]);					// しかも今ONのやつ

	}

}
//----------------------------------------------- 検査
BOOL Input::IsButtonPressed(int padNo,DWORD button)
{
	return (button & padState[padNo]);
}

BOOL Input::IsButtonTriggered(int padNo,DWORD button)
{
	return (button & padTrigger[padNo]);
}



LONG Input::GetLeftStickX(int padNo)
{
	return LeftStickX[padNo];
}
LONG Input::GetLeftStickY(int padNo)
{
	return LeftStickY[padNo];
}
LONG Input::GetRightStickX(int padNo)
{
	return RightStickX[padNo];
}
LONG Input::GetRightStickY(int padNo)
{
	return RightStickY[padNo];
}
LONG Input::GetVALUER2(int padNo)
{
	return VALUER2[padNo];
}
LONG Input::GetVALUEL2(int padNo)
{
	return VALUEL2[padNo];
}