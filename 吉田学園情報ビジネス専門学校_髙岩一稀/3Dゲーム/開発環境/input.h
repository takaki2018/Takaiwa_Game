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
	KEYINFO_OK = 0,					// 決定
	KEYINFO_UP,						// 上選択
	KEYINFO_DOWN,					// 下選択
	KEYINFO_RIGHT,					// 右移動
	KEYINFO_LEFT,					// 左移動
	KEYINFO_ROT_CAMERA_RIGHT,		// 視点右回転
	KEYINFO_ROT_CAMERA_LEFT,		// 視点左回転
	KEYINFO_ROT_RIGHT,				// 注視点右回転
	KEYINFO_ROT_LEFT,				// 注視点左回転
	KEYINFO_ROT_CAMERA_UP,			// 視点右回転
	KEYINFO_ROT_CAMERA_DOWN,		// 視点左回転
	KEYINFO_ROT_UP,					// 注視点右回転
	KEYINFO_ROT_DOWN,				// 注視点左回転
	KEYINFO_MODEL_UP,				// 上選択
	KEYINFO_MODEL_DOWN,				// 下選択
	KEYINFO_MODEL_RIGHT,			// 右移動
	KEYINFO_MODEL_LEFT,				// 左移動
	KEYINFO_MODEL_RIGHT_TURN,		// 右回転
	KEYINFO_MODEL_LEFT_TURN,		// 左回転
	KEYINFO_PAUSE,					// ポーズ
	KEYINFO_SHOOT,					// 弾の発射
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