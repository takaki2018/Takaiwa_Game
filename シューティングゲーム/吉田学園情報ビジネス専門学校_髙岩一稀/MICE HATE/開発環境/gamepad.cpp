//-------------------------------------------------------------------
//
// ゲームパッド (gamepad.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "gamepad.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define DEADZONE        2500            // 各軸の25%を無効ゾーンとする
#define RANGE_MAX       10000           // 有効範囲の最大値
#define RANGE_MIN       -10000          // 有効範囲の最小値

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevjoypad = NULL;	// ジョイパッドのデバイス情報
	DIJOYSTATE2 State;						// プレス
	DIJOYSTATE2 Trigger;					// トリガー
	DIJOYSTATE2 Release;					// リリース
}CONTROLLER;								// コントローラーの情報

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
LPDIRECTINPUT8 g_pInputGamepad = NULL;	// Dinputのポインタ
CONTROLLER g_Controller;				// コントローラー情報のポインタ
bool g_bTrigger[4];						// スティックを倒したか
bool g_bStick[4];

//-------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// 変数の初期化
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		g_bTrigger[nCnt] = false;
		g_bStick[nCnt] = false;
	}

	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInputGamepad,
		NULL)))
	{
		return E_FAIL;
	}

	// ゲームパッドデバイスの取得
	if (FAILED(g_pInputGamepad->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	// コントローラー情報の初期化
	if (!g_Controller.pDevjoypad)
	{
		return E_FAIL;
	}

	// ボタンの初期化
	for (int nCntButton = 0; nCntButton < 13; nCntButton++)
	{
		g_Controller.State.rgbButtons[nCntButton] = '\0';
		g_Controller.Trigger.rgbButtons[nCntButton] = '\0';
		g_Controller.Release.rgbButtons[nCntButton] = '\0';
	}

	// 十字キーの初期化
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		g_Controller.State.rgdwPOV[nCnt] = -1;
	}

	// データフォーマットの設定
	if (FAILED(g_Controller.pDevjoypad->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(g_Controller.pDevjoypad->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// 左スティックの初期化
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = RANGE_MIN;
	diprg.lMax = RANGE_MAX;

	// X軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_X;
	g_Controller.pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Y軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_Y;
	g_Controller.pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Z軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_Z;
	g_Controller.pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Z回転の範囲を設定
	diprg.diph.dwObj = DIJOFS_RZ;
	g_Controller.pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);


	// 右スティックの初期化
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = DEADZONE;

	// X軸の範囲を設定
	dipdw.diph.dwObj = DIJOFS_X;
	g_Controller.pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	// Y軸の範囲を設定
	dipdw.diph.dwObj = DIJOFS_Y;
	g_Controller.pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	// Z軸の範囲を設定
	dipdw.diph.dwObj = DIJOFS_Z;
	g_Controller.pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	// Z回転の範囲を設定
	dipdw.diph.dwObj = DIJOFS_RZ;
	g_Controller.pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	// アクセス権の取得
	g_Controller.pDevjoypad->Acquire();

	return S_OK;
}

//-------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------
void UninitGamepad(void)
{
	// 入力デバイスの破棄
	if (g_Controller.pDevjoypad != NULL)
	{
		// アクセス権の破棄
		g_Controller.pDevjoypad->Unacquire();

		g_Controller.pDevjoypad->Release();
		g_Controller.pDevjoypad = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (g_Controller.pDevjoypad != NULL)
	{
		g_Controller.pDevjoypad->Release();
		g_Controller.pDevjoypad = NULL;
	}
}

//-------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------
void UpdateGamepad(void)
{
	// 変数宣言
	DIJOYSTATE2 ControllerState;	// コントローラーのボタン情報
	HRESULT hr;						// 成功したか判別
	int nCntButton;					// ボタンの個数

	if (g_Controller.pDevjoypad != NULL)
	{// ジョイパッドが接続されていたらポーリング開始
		hr = g_Controller.pDevjoypad->Poll();

		// デバイスからデータを取得
		if (SUCCEEDED(hr = g_Controller.pDevjoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
		{// スティックに情報を代入
			// 左スティックのトリガー情報
			JoypadTriggerInfo(ControllerState);
		
			// 左スティックの情報
			g_Controller.State.lY = ControllerState.lY;			// lYのプレス
			g_Controller.State.lX = ControllerState.lX;			// lXのプレス
			g_Controller.State.lZ = ControllerState.lZ;			// lZのプレス
			g_Controller.State.lRz = ControllerState.lRz;		// lRzのプレス

			for (int nCnt = 0; nCnt < 3; nCnt++) 
			{// プレス情報の保存
				g_Controller.State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];
			}

			for (nCntButton = 0; nCntButton < 13; nCntButton++)
			{// トリガー情報の保存
				g_Controller.Trigger.rgbButtons[nCntButton] =
					(g_Controller.State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
			
				// リリース情報の保存
				g_Controller.Release.rgbButtons[nCntButton] = 
					(ControllerState.rgbButtons[nCntButton] ^ g_Controller.State.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
			
				// プレス情報の保存
				g_Controller.State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
			}
		}
		else
		{// アクセス権の取得
			g_Controller.pDevjoypad->Acquire();
		}
	}
}

//-------------------------------------------------------------------
// スティックのトリガー情報
//-------------------------------------------------------------------
void JoypadTriggerInfo(DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bTrigger[4] = {false,false,false,false};

	if (State.lY <= -7000)
	{// 上
		bTrigger[0] = true;
	}
	else if (State.lY >= 7000)
	{// 下
		bTrigger[1] = true;
	}
	else if (State.lX >= 7000)
	{// 左
		bTrigger[2] = true;
	}
	else if (State.lX <= -7000)
	{// 右
		bTrigger[3] = true;
	}

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{// トリガー情報
		g_bStick[nCnt] = (g_bTrigger[nCnt] ^ bTrigger[nCnt]) & bTrigger[nCnt];

		g_bTrigger[nCnt] = bTrigger[nCnt];
	}
}

//-------------------------------------------------------------------
// 入力情報の取得
//-------------------------------------------------------------------
bool GetJoypadPress(int nButton)
{// プレス情報の確認
	return (g_Controller.State.rgbButtons[nButton] & 0x80) ? true : false;
}

bool GetJoypadTrigger(int nButton)
{// トリガー情報の確認
	return (g_Controller.Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

bool GetJoypadReleace(int nButton)
{// リリース情報の確認
	return (g_Controller.Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//-------------------------------------------------------------------
// ジョイパッド情報の取得
//-------------------------------------------------------------------
DIJOYSTATE2 GetControllerPless(void)
{// プレス情報
	return g_Controller.State;
}

bool *GetControllerTrigger(void)
{// トリガー情報
	return &g_bStick[0];
}

//-------------------------------------------------------------------
// デバイス発見時に実行される
//-------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	// 変数宣言
	HRESULT hr;

	// ジョイパッドが接続されているか
	hr = g_pInputGamepad->CreateDevice(pdidInstance->guidInstance, &g_Controller.pDevjoypad, NULL);

	// 続ける
	return DIENUM_CONTINUE;
}