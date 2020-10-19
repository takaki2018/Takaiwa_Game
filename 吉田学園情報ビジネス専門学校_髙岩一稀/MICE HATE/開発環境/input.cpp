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
LPDIRECTINPUT8 g_pInput = NULL;					// インプット情報のポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		// デバイスのおインタ
BYTE g_aKeyState[MAX_DATA];						// プレス情報
BYTE g_aKeyStateTrigger[MAX_DATA];				// トリガー情報
BYTE g_aKeyStateReleace[MAX_DATA];				// リリース情報

int g_aKeyInfo[] =								// キーボード操作一覧
{
	DIK_RETURN,		// 決定
	DIK_W,			// 上移動
	DIK_S,			// 下移動
	DIK_D,			// 右移動
	DIK_A,			// 左移動
	DIK_E,			// 右回転
	DIK_Q,			// 左回転
	DIK_SPACE,		// ショット
	DIK_K,			// 弾の色変更
	DIK_H,			// アイテム
	DIK_P			// ポーズ
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
		{// 情報の保存
			// トリガー情報
			g_aKeyStateTrigger[nKey] = (g_aKeyState[nKey] & aKeyState[nKey]) ^ aKeyState[nKey];

			// リリース情報
			g_aKeyStateReleace[nKey] = (g_aKeyState[nKey] | aKeyState[nKey]) ^ aKeyState[nKey];

			// プレス情報
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
{// プレス情報
	return (g_aKeyState[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool GetKeyboardTrigger(int nKey)
{// トリガー情報
	return (g_aKeyStateTrigger[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool GetKeyboardReleace(int nKey)
{// リリース情報
	return (g_aKeyStateReleace[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}