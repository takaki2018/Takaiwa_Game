//-------------------------------------------------------------------
//
// ゲームパッドヘッダー (gamepad.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

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
	JPINFO_SHOOT = 0,		// 弾発射
	JPINFO_COLOR_CHANGE,	// 弾の色変更		
	JPINFO_OK,				// 決定
	JPINFO_ITEM,			// アイテム使用
	JPINFO_LEFT_TURN,		// 左回転
	JPINFO_RIGHT_TURN,		// 右回転
	JPINFO_UP,				// 上
	JPINFO_DOWN,			// 下
	JPINFO_LS,				// 使用しない
	JPINFO_RS,				// 使用しない
	JPINFO_BACK,			// 使用しない
	JPINFO_PAUSE,			// ポーズ
	JPINFO_GUIDE			// 使用しない
}JOYPADINFO;				// JOYPADのキー情報

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);										// ゲームパッドの初期化処理
void UninitGamepad(void);																	// ゲームパッドの終了処理
void UpdateGamepad(void);																	// ゲームパッドの更新処理
void JoypadTriggerInfo(DIJOYSTATE2 State);													// 左スティックのトリガー情報の保存
bool GetJoypadPress(int nButton);															// ボタンのプレス情報
bool GetJoypadTrigger(int nButton);															// ボタンのトリガー情報
bool GetJoypadReleace(int nButton);															// ボタンのリリース情報
bool *GetControllerTrigger(void);															// 左スティックのトリガー情報
DIJOYSTATE2 GetControllerPless(void);														// スティックのプレス情報	
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// コールバック関数


#endif
