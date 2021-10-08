//=============================================================================
//
// キーボード処理 [input_keyboard.cpp]
// Author : 
//
//=============================================================================
#include "input_keyboard.h"

//--------------------------------------------------------------------------------------------------------------
// CInputKeyboardのコンストラクタ
//--------------------------------------------------------------------------------------------------------------
CInputKeyboard::CInputKeyboard()
{
	// メンバ変数の初期化
	memset(&m_aKeyState[0],0,MAX_DATA);
	memset(&m_aKeyStateReleace[0], 0, MAX_DATA);
	memset(&m_aKeyStateRepeat[0], 0, MAX_DATA);
	memset(&m_aKeyStateTrigger[0], 0, MAX_DATA);
}

//--------------------------------------------------------------------------------------------------------------
// CInputKeyboardのデストラクタ
//--------------------------------------------------------------------------------------------------------------
CInputKeyboard::~CInputKeyboard()
{

}

//--------------------------------------------------------------------------------------------------------------
// 初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 共通した入力処理の初期化処理
	CInput::Init(hInstance, hWnd);

	// 入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(
		GUID_SysKeyboard,
		&m_pDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットの設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(m_pDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// アクセス権の取得
	m_pDevice->Acquire();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// 終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void CInputKeyboard::Uninit(void)
{
	// 終了処理
	CInput::Uninit();
}

//--------------------------------------------------------------------------------------------------------------
// 更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void CInputKeyboard::Update(void)
{
	// 変数宣言
	BYTE aKeyState[MAX_DATA];

	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nKey = 0; nKey < MAX_DATA; nKey++)
		{
			m_aKeyStateTrigger[nKey] = (m_aKeyState[nKey] & aKeyState[nKey]) ^ aKeyState[nKey];

			m_aKeyStateReleace[nKey] = (m_aKeyState[nKey] | aKeyState[nKey]) ^ aKeyState[nKey];

			m_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{// アクセス権の取得
		m_pDevice->Acquire();
	}
}

//--------------------------------------------------------------------------------------------------------------
// キーボード情報の取得
// 引数		：nkey	- どのキーを押したか
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[m_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[m_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool CInputKeyboard::GetReleace(int nKey)
{
	return (m_aKeyStateReleace[m_aKeyInfo[nKey]] & 0x80) ? true : false;
}