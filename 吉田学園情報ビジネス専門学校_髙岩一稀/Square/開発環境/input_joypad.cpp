//=============================================================================
//
// ジョイパッド処理 (input_joypad.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#include "input_joypad.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DEADZONE        (2500)      // 各軸の25%を無効ゾーンとする
#define RANGE_MAX       (10000)     // 有効範囲の最大値
#define RANGE_MIN       (-10000)    // 有効範囲の最小値

#define MAX_DIRECTION	(4)			// キー方向の最大数

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************	
CInputJoypad::CONTROLLER CInputJoypad::m_Controller[MAX_CONTROLLER] = {};		// コントローラー情報のポインタ
CInputJoypad::JPINFO CInputJoypad::m_Stick[MAX_CONTROLLER] = {};				// スティック情報
int CInputJoypad::m_nNumController = 0;											// 使用するコントローラーのカウント

//=============================================================================	
// CManagerのコンストラクタ
//=============================================================================	
CInputJoypad::CInputJoypad()
{

}

//=============================================================================	
// CManagerのデストラクタ
//=============================================================================	
CInputJoypad::~CInputJoypad()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// インプット処理の共通初期化処理
	CInput::Init(hInstance,hWnd);

	// 変数の初期化
	for (int nCntController = 0; nCntController < MAX_CONTROLLER; nCntController++)
	{
		for (int nCntDirection = 0; nCntDirection < MAX_DIRECTION; nCntDirection++)
		{
			m_Stick[nCntController].bLStickPress[nCntDirection] = false;
			m_Stick[nCntController].bRStickPress[nCntDirection] = false;
			m_Stick[nCntController].bStickTrigger[nCntDirection] = false;
			m_Stick[nCntController].bStickOldTrigger[nCntDirection] = false;
			m_Stick[nCntController].bCrossKeyPress[nCntDirection] = false;
			m_Stick[nCntController].bCrossKeyTrigger[nCntDirection] = false;
			m_Stick[nCntController].bCrossKeyOldTrigger[nCntDirection] = false;
		}
	}

	// ゲームパッドデバイスの取得
	if (FAILED(m_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < m_nNumController; nCnt++)
	{
		// コントローラー情報の初期化
		if (!m_Controller[nCnt].pDevjoypad)
		{
			return E_FAIL;
		}

		// ボタンの初期化
		for (int nCntButton = 0; nCntButton < 13; nCntButton++)
		{
			m_Controller[nCnt].State.rgbButtons[nCntButton] = '\0';
			m_Controller[nCnt].Trigger.rgbButtons[nCntButton] = '\0';
			m_Controller[nCnt].Release.rgbButtons[nCntButton] = '\0';
		}

		// 十字キーの初期化
		//for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			m_Controller[nCnt].State.rgdwPOV[0] = -1;
		}

		// データフォーマットの設定
		if (FAILED(m_Controller[nCnt].pDevjoypad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return E_FAIL;
		}

		// 協調モードの設定
		if (FAILED(m_Controller[nCnt].pDevjoypad->SetCooperativeLevel(
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
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_RX;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_RY;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Z;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z回転の範囲を設定
		diprg.diph.dwObj = DIJOFS_RZ;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 右スティックの初期化
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = 0;

		// X軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_X;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_Y;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// X軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_RX;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_RY;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z軸の範囲を設定
		dipdw.diph.dwObj = DIJOFS_Z;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z回転の範囲を設定
		dipdw.diph.dwObj = DIJOFS_RZ;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// アクセス権の取得
		m_Controller[nCnt].pDevjoypad->Acquire();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputJoypad::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nNumController; nCnt++)
	{
		// 入力デバイスの破棄
		if (m_Controller[nCnt].pDevjoypad != NULL)
		{
			// アクセス権の破棄
			m_Controller[nCnt].pDevjoypad->Unacquire();

			m_Controller[nCnt].pDevjoypad->Release();
			m_Controller[nCnt].pDevjoypad = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputJoypad::Update(void)
{
	// 変数宣言
	DIJOYSTATE2 ControllerState;	// コントローラーのボタン情報
	HRESULT hr;						// 成功したか判別
	int nCntButton;					// ボタンの個数

	for (int nCnt = 0; nCnt < m_nNumController; nCnt++)
	{
		if (m_Controller[nCnt].pDevjoypad != NULL)
		{// ジョイパッドが接続されていたらポーリング開始
			hr = m_Controller[nCnt].pDevjoypad->Poll();

			// デバイスからデータを取得
			if (SUCCEEDED(hr = m_Controller[nCnt].pDevjoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
			{// スティックに情報を代入
				LStickPressInfo(nCnt, ControllerState);						// Lスティックのプレス処理
				RStickPressInfo(nCnt, ControllerState);						// Rスティックのプレス処理
				StickTriggerInfo(nCnt, ControllerState);					// スティックのトリガー処理
				CrossKeyPressInfo(nCnt, ControllerState);					// ボタンのプレス処理
				CrossKeyTriggerInfo(nCnt, ControllerState);					// ボタンのトリガー処理

				// 左スティックの情報
				m_Controller[nCnt].State.lY = ControllerState.lY;			// lYのプレス
				m_Controller[nCnt].State.lX = ControllerState.lX;			// lXのプレス
				m_Controller[nCnt].State.lRx = ControllerState.lRx;			// lRxのプレス
				m_Controller[nCnt].State.lRy = ControllerState.lRy;			// lRyのプレス
				m_Controller[nCnt].State.lZ = ControllerState.lZ;			// lZのプレス
				m_Controller[nCnt].State.lRz = ControllerState.lRz;			// lRzのプレス

				for (nCntButton = 0; nCntButton < 13; nCntButton++)
				{// トリガー情報の保存
					m_Controller[nCnt].Trigger.rgbButtons[nCntButton] =
						(m_Controller[nCnt].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					// リリース情報の保存
					m_Controller[nCnt].Release.rgbButtons[nCntButton] =
						(ControllerState.rgbButtons[nCntButton] ^ m_Controller[nCnt].State.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					// プレス情報の保存
					m_Controller[nCnt].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
				}
			}
			else
			{// アクセス権の取得
				m_Controller[nCnt].pDevjoypad->Acquire();
			}
		}
	}
}

//=============================================================================
// Lスティックのプレス情報
//=============================================================================
void CInputJoypad::LStickPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bLStickPress[STICK_AND_CROSSKEY_MAX] = { false,false,false,false };

	if (State.lY >= 7000)
	{// 左
		bLStickPress[STICK_AND_CROSSKEY_UP] = true;
	}
	else if (State.lY <= -7000)
	{// 右
		bLStickPress[STICK_AND_CROSSKEY_DOWN] = true;
	}
	else if (State.lX >= 7000)
	{// 左
		bLStickPress[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.lX <= -7000)
	{// 右
		bLStickPress[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < STICK_AND_CROSSKEY_MAX; nCntPress++)
	{// プレス情報
		m_Stick[nCnt].bLStickPress[nCntPress] = bLStickPress[nCntPress];
	}
}

//=============================================================================
// Rスティックのプレス情報
//=============================================================================
void CInputJoypad::RStickPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bRStickPress[STICK_AND_CROSSKEY_MAX] = { false,false,false,false };

	if (State.lRz <= -7000)
	{// 左
		bRStickPress[STICK_AND_CROSSKEY_UP] = true;
	}
	else if (State.lRz >= 7000)
	{// 右
		bRStickPress[STICK_AND_CROSSKEY_DOWN] = true;
	}
	else if (State.lZ >= 7000)
	{// 左
		bRStickPress[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.lZ <= -7000)
	{// 右
		bRStickPress[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < STICK_AND_CROSSKEY_MAX; nCntPress++)
	{// プレス情報
		m_Stick[nCnt].bRStickPress[nCntPress] = bRStickPress[nCntPress];
	}
}

//=============================================================================
// スティックのトリガー情報
//=============================================================================
void CInputJoypad::StickTriggerInfo(int nCnt, DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bStickTrigger[STICK_AND_CROSSKEY_MAX] = { false,false,false,false };

	if (State.lY <= -7000)
	{// 上
		bStickTrigger[STICK_AND_CROSSKEY_UP] = true;
	}
	else if (State.lY >= 7000)
	{// 下
		bStickTrigger[STICK_AND_CROSSKEY_DOWN] = true;
	}
	else if (State.lX >= 7000)
	{// 左
		bStickTrigger[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.lX <= -7000)
	{// 右
		bStickTrigger[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntTrigger = 0; nCntTrigger < STICK_AND_CROSSKEY_MAX; nCntTrigger++)
	{// トリガー情報
		m_Stick[nCnt].bStickTrigger[nCntTrigger] = (m_Stick[nCnt].bStickOldTrigger[nCntTrigger] ^ bStickTrigger[nCntTrigger]) & bStickTrigger[nCntTrigger];

		// 情報を保存
		m_Stick[nCnt].bStickOldTrigger[nCntTrigger] = bStickTrigger[nCntTrigger];
	}
}

//=============================================================================
// 十字キーのプレス情報
//=============================================================================
void CInputJoypad::CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bCrossKeyPress[STICK_AND_CROSSKEY_MAX] = { false,false ,false,false };

	if (State.rgdwPOV[0] >= 3000 && State.rgdwPOV[0] <= 15000)
	{// 左
		bCrossKeyPress[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.rgdwPOV[0] >= 21000 && State.rgdwPOV[0] <= 33000)
	{// 右
		bCrossKeyPress[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < STICK_AND_CROSSKEY_MAX; nCntPress++)
	{// プレス情報
		m_Stick[nCnt].bCrossKeyPress[nCntPress] = bCrossKeyPress[nCntPress];
	}
}

//=============================================================================
// 十字キーのトリガー情報
//=============================================================================
void CInputJoypad::CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State)
{
	// 変数の初期化
	bool bCrossKeyTrigger[STICK_AND_CROSSKEY_MAX] = { false,false,false,false };

	if ((State.rgdwPOV[0] >= 0 && State.rgdwPOV[0] <= 6000) || (State.rgdwPOV[0] >= 30000 && State.rgdwPOV[0] < 36000))
	{// 上
		bCrossKeyTrigger[STICK_AND_CROSSKEY_UP] = true;
	}
	else if (State.rgdwPOV[0] >= 12000 && State.rgdwPOV[0] <= 24000)
	{// 下
		bCrossKeyTrigger[STICK_AND_CROSSKEY_DOWN] = true;
	}
	else if (State.rgdwPOV[0] >= 3000 && State.rgdwPOV[0] <= 15000)
	{// 左
		bCrossKeyTrigger[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.rgdwPOV[0] >= 21000 && State.rgdwPOV[0] <= 33000)
	{// 右
		bCrossKeyTrigger[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntTrigger = 0; nCntTrigger < STICK_AND_CROSSKEY_MAX; nCntTrigger++)
	{// トリガー情報
		m_Stick[nCnt].bCrossKeyTrigger[nCntTrigger] = (m_Stick[nCnt].bCrossKeyOldTrigger[nCntTrigger] ^ bCrossKeyTrigger[nCntTrigger]) & bCrossKeyTrigger[nCntTrigger];

		// 情報を保存
		m_Stick[nCnt].bCrossKeyOldTrigger[nCntTrigger] = bCrossKeyTrigger[nCntTrigger];
	}
}

//=============================================================================
// ボタン入力情報の取得
//=============================================================================
bool CInputJoypad::GetJoypadPress(int nCnt, int nButton)
{// プレス情報の確認
	return (m_Controller[nCnt].State.rgbButtons[nButton] & 0x80) ? true : false;
}

bool CInputJoypad::GetJoypadTrigger(int nCnt, int nButton)
{// トリガー情報の確認
	return (m_Controller[nCnt].Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

bool CInputJoypad::GetJoypadReleace(int nCnt, int nButton)
{// リリース情報の確認
	return (m_Controller[nCnt].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// スティック情報の取得
//=============================================================================
DIJOYSTATE2 CInputJoypad::GetControllerPless(int nCnt)
{// プレス情報
	return m_Controller[nCnt].State;
}

bool *CInputJoypad::GetLStickPress(int nCnt)
{// Lスティックのプレス情報
	return &m_Stick[nCnt].bLStickPress[0];
}

bool *CInputJoypad::GetRStickPress(int nCnt)
{// Rスティックのプレス情報の取得
	return &m_Stick[nCnt].bRStickPress[0];
}

bool *CInputJoypad::GetStickTrigger(int nCnt)
{// トリガー情報
	return &m_Stick[nCnt].bStickTrigger[0];
}

//=============================================================================
// 十字キー情報の取得
//=============================================================================
bool *CInputJoypad::GetCrossKeyPress(int nCnt)
{// プレス情報
	return &m_Stick[nCnt].bCrossKeyPress[0];
}

bool *CInputJoypad::GetCrossKeyTrigger(int nCnt)
{// トリガー情報
	return &m_Stick[nCnt].bCrossKeyTrigger[0];
}

//=============================================================================
// デバイス発見時に実行される
//=============================================================================
BOOL CALLBACK CInputJoypad::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	// 変数宣言
	HRESULT hr;

	// ジョイパッドが接続されているか
	hr = m_pInput->CreateDevice(pdidInstance->guidInstance, &m_Controller[m_nNumController].pDevjoypad, NULL);

	// 接続されているコントローラーのカウント
	m_nNumController++;

	// 続ける
	return DIENUM_CONTINUE;
}

//=============================================================================
// 接続されたコントローラーのカウント
//=============================================================================
int CInputJoypad::GetnCntController(void)
{
	return m_nNumController;
}