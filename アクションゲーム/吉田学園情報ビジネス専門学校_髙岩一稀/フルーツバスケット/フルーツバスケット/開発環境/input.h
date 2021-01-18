//-------------------------------------------------------------------
//
// インプットヘッダー (input.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_DATA (256)

//-------------------------------------------------------------------
// キー操作の状態
//-------------------------------------------------------------------
typedef enum
{
	KEYINFO_OK = 0,			// 決定
	KEYINFO_UP,				// 上選択
	KEYINFO_DOWN,			// 下選択
	KEYINFO_RIGHT,			// 右移動
	KEYINFO_LEFT,			// 左移動
	KEYINFO_JAMP,			// ジャンプ
	KEYINFO_PAUSE,			// ポーズ
	KEYINFO_MAX,
}KEYINFO;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardReleace(int nKey);

#endif