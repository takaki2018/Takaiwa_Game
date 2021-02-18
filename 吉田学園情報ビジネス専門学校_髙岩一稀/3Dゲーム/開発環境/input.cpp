//-------------------------------------------------------------------
//
// インプットファイル (input.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "input.h"

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[MAX_DATA];
BYTE g_aKeyStateTrigger[MAX_DATA];
BYTE g_aKeyStateReleace[MAX_DATA];
BYTE g_aKeyStateRepeat[MAX_DATA];

int g_aKeyInfo[] =
{
	DIK_RETURN,		// 決定
	DIK_W,			// 上選択
	DIK_S,			// 下選択
	DIK_D,			// 右移動
	DIK_A,			// 左移動
	DIK_E,			// 視点右回転
	DIK_Q,			// 視点左回転
	DIK_Z,			// 注視点右回転
	DIK_C,			// 注視点左回転
	DIK_T,			// 視点上移動
	DIK_B,			// 視点下移動
	DIK_R,			// 注視点上移動
	DIK_V,			// 注視点下移動
	DIK_UP,			// 上選択
	DIK_DOWN,		// 下選択
	DIK_RIGHT,		// 右移動
	DIK_LEFT,		// 左移動
	DIK_RSHIFT,		// モデル右回転
	DIK_LSHIFT,		// モデル左回転
	DIK_P,			// ポーズ
	DIK_SPACE,		// 弾の発射
};

//-------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイスの生成
	if (FAILED(g_pInput->CreateDevice(
		GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットの設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// アクセス権の取得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//-------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------
void UninitKeyboard(void)
{
	// 入力デバイスの破棄
	if (g_pDevKeyboard != NULL)
	{
		// アクセス権の破棄
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//-------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------
void UpdateKeyboard(void)
{
	// 変数宣言
	BYTE aKeyState[MAX_DATA];

	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{	
		for (int nKey = 0; nKey < MAX_DATA; nKey++)
		{
			g_aKeyStateTrigger[nKey] = (g_aKeyState[nKey] & aKeyState[nKey]) ^ aKeyState[nKey];

			g_aKeyStateReleace[nKey] = (g_aKeyState[nKey] | aKeyState[nKey]) ^ aKeyState[nKey];

			g_aKeyState[nKey] = aKeyState[nKey]; 
		}
	}
	else
	{// アクセス権の取得
		g_pDevKeyboard->Acquire();
	}
}

//-------------------------------------------------------------------
// キーボード情報の取得
//-------------------------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool GetKeyboardReleace(int nKey)
{
	return (g_aKeyStateReleace[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}