//==============================================================================================================
//
// ゲームパッド (gamepad.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "gamepad.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_CONTROLLER	(1)			// 接続可能なコントローラーの最大数
#define DEADZONE        (2500)      // 各軸の25%を無効ゾーンとする
#define RANGE_MAX       (10000)     // 有効範囲の最大値
#define RANGE_MIN       (-10000)    // 有効範囲の最小値

#define MAX_DIRECTION	(4)			// キー方向の最大数

//--------------------------------------------------------------------------------------------------------------
// 構造体の定義
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevjoypad = NULL;	// ジョイパッドのデバイス情報
	DIJOYSTATE2 State;						// プレス
	DIJOYSTATE2 Trigger;					// トリガー
	DIJOYSTATE2 Release;					// リリース
}CONTROLLER;								// コントローラーの情報

typedef struct
{
	bool bLStickPress[LSTICKPRESS_MAX];				// Lスティックのプレス情報
	bool bRStickPress[RSTICKPRESS_MAX];				// Rスティックのプレス情報
	bool bStickTrigger[STICKTRIGGER_MAX];			// スティックのトリガー情報
	bool bStickOldTrigger[STICKTRIGGER_MAX];		// スティックのトリガー情報を保存
	bool bCrossKeyPress[CROSSKEYPRESS_MAX];			// 十字キーのプレス情報
	bool bCrossKeyTrigger[CROSSKEYTRIGGER_MAX];		// 十字キーのトリガー情報
	bool bCrossKeyOldTrigger[CROSSKEYTRIGGER_MAX];	// 十字キーのトリガー情報を保存
}JPINFO;											// コントローラー情報

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECTINPUT8 g_pInputGamepad = NULL;					// Dinputのポインタ
CONTROLLER g_Controller[MAX_CONTROLLER];				// コントローラー情報のポインタ
int g_nCntController = 0;								// 使用するコントローラーのカウント
JPINFO stick[MAX_CONTROLLER];							// スティック情報

//--------------------------------------------------------------------------------------------------------------
// 初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// 変数の初期化
	for (int nCntController = 0; nCntController < MAX_CONTROLLER; nCntController++)
	{
		for (int nCntDirection = 0; nCntDirection < MAX_DIRECTION; nCntDirection++)
		{
			stick[nCntController].bLStickPress[nCntDirection] = false;
			stick[nCntController].bRStickPress[nCntDirection] = false;
			stick[nCntController].bStickTrigger[nCntDirection] = false;
			stick[nCntController].bStickOldTrigger[nCntDirection] = false;
			stick[nCntController].bCrossKeyPress[nCntDirection] = false;
			stick[nCntController].bCrossKeyTrigger[nCntDirection] = false;
			stick[nCntController].bCrossKeyOldTrigger[nCntDirection] = false;
		}
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

	for (int nCnt = 0; nCnt < g_nCntController; nCnt++)
	{
		// コントローラー情報の初期化
		if (!g_Controller[nCnt].pDevjoypad)
		{
			return E_FAIL;
		}

		// ボタンの初期化
		for (int nCntButton = 0; nCntButton < 13; nCntButton++)
		{
			g_Controller[nCnt].State.rgbButtons[nCntButton] = '\0';
			g_Controller[nCnt].Trigger.rgbButtons[nCntButton] = '\0';
			g_Controller[nCnt].Release.rgbButtons[nCntButton] = '\0';
		}

		// 十字キーの初期化
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			g_Controller[nCnt].State.rgdwPOV[0] = -1;
		}

		// データフォーマットの設定
		if (FAILED(g_Controller[nCnt].pDevjoypad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return E_FAIL;
		}

		// 協調モードの設定
		if (FAILED(g_Controller[nCnt].pDevjoypad->SetCooperativeLevel(
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
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_RX;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_RY;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Z;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z回転の範囲を設定
		diprg.diph.dwObj = DIJOFS_RZ;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 右スティックの初期化
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = 0;

		// X軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_X;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_Y;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// X軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_RX;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_RY;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_Z;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z回転の範囲を設定
		dipdw.diph.dwObj = DIJOFS_RZ;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// アクセス権の取得
		g_Controller[nCnt].pDevjoypad->Acquire();
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// 終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitGamepad(void)
{
	for (int nCnt = 0; nCnt < g_nCntController; nCnt++)
	{
		// 入力デバイスの破棄
		if (g_Controller[nCnt].pDevjoypad != NULL)
		{
			// アクセス権の破棄
			g_Controller[nCnt].pDevjoypad->Unacquire();

			g_Controller[nCnt].pDevjoypad->Release();
			g_Controller[nCnt].pDevjoypad = NULL;
		}
	}
	// DirectInputオブジェクトの破棄
	if (g_pInputGamepad != NULL)
	{
		g_pInputGamepad->Release();
		g_pInputGamepad = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// 更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateGamepad(void)
{
	// 変数宣言
	DIJOYSTATE2 ControllerState;	// コントローラーのボタン情報
	HRESULT hr;						// 成功したか判別
	int nCntButton;					// ボタンの個数

	for (int nCnt = 0; nCnt < g_nCntController; nCnt++)
	{
		if (g_Controller[nCnt].pDevjoypad != NULL)
		{// ジョイパッドが接続されていたらポーリング開始
			hr = g_Controller[nCnt].pDevjoypad->Poll();

			// デバイスからデータを取得
			if (SUCCEEDED(hr = g_Controller[nCnt].pDevjoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
			{// スティックに情報を代入
				LStickPressInfo(nCnt,ControllerState);						// Lスティックのプレス処理
				RStickPressInfo(nCnt, ControllerState);						// Rスティックのプレス処理
				StickTriggerInfo(nCnt, ControllerState);					// スティックのトリガー処理
				CrossKeyPressInfo(nCnt, ControllerState);					// ボタンのプレス処理
				CrossKeyTriggerInfo(nCnt, ControllerState);					// ボタンのトリガー処理

				// 左スティックの情報
				g_Controller[nCnt].State.lY = ControllerState.lY;			// lYのプレス
				g_Controller[nCnt].State.lX = ControllerState.lX;			// lXのプレス
				g_Controller[nCnt].State.lRx = ControllerState.lRx;			// lRxのプレス
				g_Controller[nCnt].State.lRy = ControllerState.lRy;			// lRyのプレス
				g_Controller[nCnt].State.lZ = ControllerState.lZ;			// lZのプレス
				g_Controller[nCnt].State.lRz = ControllerState.lRz;			// lRzのプレス

				for (nCntButton = 0; nCntButton < 13; nCntButton++)
				{// トリガー情報の保存
					g_Controller[nCnt].Trigger.rgbButtons[nCntButton] =
						(g_Controller[nCnt].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					// リリース情報の保存
					g_Controller[nCnt].Release.rgbButtons[nCntButton] =
						(ControllerState.rgbButtons[nCntButton] ^ g_Controller[nCnt].State.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					// プレス情報の保存
					g_Controller[nCnt].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
				}
			}
			else
			{// アクセス権の取得
				g_Controller[nCnt].pDevjoypad->Acquire();
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// Lスティックのプレス情報
// 引数		：nCnt	- 何番目のコントローラーか
//			：State	- コントローラー情報
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void LStickPressInfo(int nCnt,DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bLStickPress[LSTICKPRESS_MAX] = { false,false,false,false};

	if (State.lY >= 7000)
	{// 左
		bLStickPress[LSTICKPRESS_UP] = true;
	}
	else if (State.lY <= -7000)
	{// 右
		bLStickPress[LSTICKPRESS_DOWN] = true;
	}
	else if (State.lX >= 7000)
	{// 左
		bLStickPress[LSTICKPRESS_RIGHT] = true;
	}
	else if (State.lX <= -7000)
	{// 右
		bLStickPress[LSTICKPRESS_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < LSTICKPRESS_MAX; nCntPress++)
	{// プレス情報
		stick[nCnt].bLStickPress[nCntPress] = bLStickPress[nCntPress];
	}
}

//--------------------------------------------------------------------------------------------------------------
// Rスティックのプレス情報
// 引数		：nCnt	- 何番目のコントローラーか
//			：State	- コントローラー情報
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void RStickPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bRStickPress[LSTICKPRESS_MAX] = { false,false,false,false };

	if (State.lRz <= -7000)
	{// 左
		bRStickPress[LSTICKPRESS_UP] = true;
	}
	else if (State.lRz >= 7000)
	{// 右
		bRStickPress[LSTICKPRESS_DOWN] = true;
	}
	else if (State.lZ >= 7000)
	{// 左
		bRStickPress[LSTICKPRESS_RIGHT] = true;
	}
	else if (State.lZ <= -7000)
	{// 右
		bRStickPress[LSTICKPRESS_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < LSTICKPRESS_MAX; nCntPress++)
	{// プレス情報
		stick[nCnt].bRStickPress[nCntPress] = bRStickPress[nCntPress];
	}
}

//--------------------------------------------------------------------------------------------------------------
// スティックのトリガー情報
// 引数		：nCnt	- 何番目のコントローラーか
//			：State	- コントローラー情報
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void StickTriggerInfo(int nCnt, DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bStickTrigger[STICKTRIGGER_MAX] = {false,false,false,false};

	if (State.lY <= -7000)
	{// 上
		bStickTrigger[STICKTRIGGER_UP] = true;
	}
	else if (State.lY >= 7000)
	{// 下
		bStickTrigger[STICKTRIGGER_DOWN] = true;
	}
	else if (State.lX >= 7000)
	{// 左
		bStickTrigger[STICKTRIGGER_RIGHT] = true;
	}
	else if (State.lX <= -7000)
	{// 右
		bStickTrigger[STICKTRIGGER_LEFT] = true;
	}

	for (int nCntTrigger = 0; nCntTrigger < STICKTRIGGER_MAX; nCntTrigger++)
	{// トリガー情報
		stick[nCnt].bStickTrigger[nCntTrigger] = (stick[nCnt].bStickOldTrigger[nCntTrigger] ^ bStickTrigger[nCntTrigger]) & bStickTrigger[nCntTrigger];

		// 情報を保存
		stick[nCnt].bStickOldTrigger[nCntTrigger] = bStickTrigger[nCntTrigger];
	}
}

//--------------------------------------------------------------------------------------------------------------
// 十字キーのプレス情報
// 引数		：nCnt	- 何番目のコントローラーか
//			：State	- コントローラー情報
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bCrossKeyPress[LSTICKPRESS_MAX] = { false,false ,false,false};

	if (State.rgdwPOV[0] >= 3000 && State.rgdwPOV[0] <= 15000)
	{// 左
		bCrossKeyPress[LSTICKPRESS_RIGHT] = true;
	}
	else if (State.rgdwPOV[0] >= 21000 && State.rgdwPOV[0] <= 33000)
	{// 右
		bCrossKeyPress[LSTICKPRESS_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < LSTICKPRESS_MAX; nCntPress++)
	{// プレス情報
		stick[nCnt].bCrossKeyPress[nCntPress] = bCrossKeyPress[nCntPress];
	}
}

//--------------------------------------------------------------------------------------------------------------
// 十字キーのトリガー情報
// 引数		：nCnt	- 何番目のコントローラーか
//			：State	- コントローラー情報
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bCrossKeyTrigger[STICKTRIGGER_MAX] = { false,false,false,false };

	if ((State.rgdwPOV[0] >= 0 && State.rgdwPOV[0] <= 6000) || (State.rgdwPOV[0] >= 30000 && State.rgdwPOV[0] < 36000))
	{// 上
		bCrossKeyTrigger[STICKTRIGGER_UP] = true;
	}
	else if (State.rgdwPOV[0] >= 12000 && State.rgdwPOV[0] <= 24000)
	{// 下
		bCrossKeyTrigger[STICKTRIGGER_DOWN] = true;
	}
	else if (State.rgdwPOV[0] >= 3000 && State.rgdwPOV[0] <= 15000)
	{// 左
		bCrossKeyTrigger[STICKTRIGGER_RIGHT] = true;
	}
	else if (State.rgdwPOV[0] >= 21000 && State.rgdwPOV[0] <= 33000)
	{// 右
		bCrossKeyTrigger[STICKTRIGGER_LEFT] = true;
	}

	for (int nCntTrigger = 0; nCntTrigger < STICKTRIGGER_MAX; nCntTrigger++)
	{// トリガー情報
		stick[nCnt].bCrossKeyTrigger[nCntTrigger] = (stick[nCnt].bCrossKeyOldTrigger[nCntTrigger] ^ bCrossKeyTrigger[nCntTrigger]) & bCrossKeyTrigger[nCntTrigger];

		// 情報を保存
		stick[nCnt].bCrossKeyOldTrigger[nCntTrigger] = bCrossKeyTrigger[nCntTrigger];
	}
}

//--------------------------------------------------------------------------------------------------------------
// ボタン入力情報の取得
// 引数		：nCnt		- 何番目のコントローラーか
//			：nButton	- どのボタンを押したか
// 返り値	：void		- 何も返さない
//--------------------------------------------------------------------------------------------------------------
bool GetJoypadPress(int nCnt, int nButton)
{// プレス情報の確認
	return (g_Controller[nCnt].State.rgbButtons[nButton] & 0x80) ? true : false;
}

bool GetJoypadTrigger(int nCnt, int nButton)
{// トリガー情報の確認
	return (g_Controller[nCnt].Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

bool GetJoypadReleace(int nCnt, int nButton)
{// リリース情報の確認
	return (g_Controller[nCnt].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//--------------------------------------------------------------------------------------------------------------
// スティック情報の取得
// 引数		：nCnt		- 何番目のコントローラーか
// 返り値	：void		- 何も返さない
//--------------------------------------------------------------------------------------------------------------
DIJOYSTATE2 GetControllerPless(int nCnt)
{// プレス情報
	return g_Controller[nCnt].State;
}

bool *GetLStickPress(int nCnt)
{// Lスティックのプレス情報
	return &stick[nCnt].bLStickPress[0];
}

bool *GetRStickPress(int nCnt)
{// Rスティックのプレス情報の取得
	return &stick[nCnt].bRStickPress[0];
}

bool *GetStickTrigger(int nCnt)
{// トリガー情報
	return &stick[nCnt].bStickTrigger[0];
}

//--------------------------------------------------------------------------------------------------------------
// 十字キー情報の取得
// 引数		：nCnt		- 何番目のコントローラーか
// 返り値	：void		- 何も返さない
//--------------------------------------------------------------------------------------------------------------
bool *GetCrossKeyPress(int nCnt)
{// プレス情報
	return &stick[nCnt].bCrossKeyPress[0];
}

bool *GetCrossKeyTrigger(int nCnt)
{// トリガー情報
	return &stick[nCnt].bCrossKeyTrigger[0];
}

//--------------------------------------------------------------------------------------------------------------
// デバイス発見時に実行される
//--------------------------------------------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	// 変数宣言
	HRESULT hr;

	// ジョイパッドが接続されているか
	hr = g_pInputGamepad->CreateDevice(pdidInstance->guidInstance, &g_Controller[g_nCntController].pDevjoypad, NULL);

	// 接続されているコントローラーのカウント
	g_nCntController++;

	// 続ける
	return DIENUM_CONTINUE;
}

//--------------------------------------------------------------------------------------------------------------
// 接続されたコントローラーのカウント
// 引数		：void	- 何もなし
// 返り値	：int	- 接続されているコントローラー数を渡す
//--------------------------------------------------------------------------------------------------------------
int GetnCntController(void)
{
	return g_nCntController;
}