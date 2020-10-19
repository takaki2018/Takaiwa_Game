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
	KEYINFO_UP,				// 上移動
	KEYINFO_DOWN,			// 下移動
	KEYINFO_RIGHT,			// 右移動
	KEYINFO_LEFT,			// 左移動
	KEYINFO_RIGHT_TURN,		// 右回転
	KEYINFO_LEFT_TURN,		// 左回転
	KEYINFO_SHOOT,			// ショット
	KEYINFO_COLOR_CHANGE,	// 弾の色変更
	KEYINFO_ITEM,			// アイテム
	KEYINFO_PAUSE,			// ポーズ
	KEYINFO_MAX,
}KEYINFO;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);		// キーボードの初期化処理
void UninitKeyboard(void);									// キーボードの終了処理
void UpdateKeyboard(void);									// キーボードの更新処理
bool GetKeyboardPress(int nKey);							// プレス情報
bool GetKeyboardTrigger(int nKey);							// トリガー情報
bool GetKeyboardReleace(int nKey);							// リリース情報

#endif