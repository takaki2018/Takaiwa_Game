//-------------------------------------------------------------------
//
// Xinputヘッダー (Xinput_gamepad.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _XINPUTGAMEPAD_H_
#define _XINPUTGAMEPAD_H_

#include "main.h"

//-------------------------------------------------------------------
// 列挙型
//-------------------------------------------------------------------
typedef enum
{
	XINPUTSTATE_SHOOT = 0,
	XINPUTSTATE_MAX,
}XinputState;

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	bool bPressA;					// Aボタンが押されてるかどうか
	bool bPressAold;				// 前回Aボタンが押されていたかどうか
	bool bPressLStick;				// 左スティックを傾けたかどうか
	float fAngle;					// 左スティックの入力角度
}XinputGamepad;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
void LoadGamepad(void);						// ゲームパッド情報の読み込み
void XinputButtons(void);					// ボタン処理
void XinputSticks(void);					// スティック処理
XinputGamepad *GetXinputGamepad(void);		// ゲームパッド情報を取得

#endif