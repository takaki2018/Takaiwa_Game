//==============================================================================================================
//
// ゲームパッドヘッダー (gamepad.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_DATA	(256)
#define PLAYER_1	(0)			// プレイヤー1

//--------------------------------------------------------------------------------------------------------------
// キー操作の状態
//--------------------------------------------------------------------------------------------------------------
typedef enum
{
	JPINFO_X = 0,			// 使用しない
	JPINFO_Y,				// 使用しない		
	JPINFO_OKorJUMP,		// 決定かジャンプ
	JPINFO_CANSEL,			// キャンセルボタン
	JPINFO_LB,				// 使用しない
	JPINFO_RB,				// 使用しない
	JPINFO_LT,				// 使用しない
	JPINFO_RT,				// 使用しない
	JPINFO_LS,				// 使用しない
	JPINFO_RS,				// 使用しない
	JPINFO_BACK,			// 使用しない
	JPINFO_PAUSE,			// ポーズ
	JPINFO_GUIDE			// 使用しない
}JOYPADINFO;

//--------------------------------------------------------------------------------------------------------------
// Lスティックのプレス情報
//--------------------------------------------------------------------------------------------------------------
typedef enum
{
	LSTICKPRESS_UP = 0,	// 上
	LSTICKPRESS_DOWN,		// 下
	LSTICKPRESS_RIGHT,		// 右
	LSTICKPRESS_LEFT,		// 左
	LSTICKPRESS_MAX,
}LSTICKPRESS;

//--------------------------------------------------------------------------------------------------------------
// Rスティックのプレス情報
//--------------------------------------------------------------------------------------------------------------
typedef enum
{
	RSTICKPRESS_UP = 0,		// 上
	RSTICKPRESS_DOWN,		// 下
	RSTICKPRESS_RIGHT,		// 右
	RSTICKPRESS_LEFT,		// 左
	RSTICKPRESS_MAX,
}RSTICKPRESS;

//--------------------------------------------------------------------------------------------------------------
// スティックのトリガー情報
//--------------------------------------------------------------------------------------------------------------
typedef enum
{
	STICKTRIGGER_UP = 0,		// 上
	STICKTRIGGER_DOWN,			// 下
	STICKTRIGGER_RIGHT,			// 右
	STICKTRIGGER_LEFT,			// 左
	STICKTRIGGER_MAX,
}STICKTRIGGER;	

//--------------------------------------------------------------------------------------------------------------
// 十字キーのプレス情報
//--------------------------------------------------------------------------------------------------------------
typedef enum
{
	CROSSKEYPRESS_UP = 0,	// 上
	CROSSKEYPRESS_DOWN,		// 下
	CROSSKEYPRESS_RIGHT,	// 右
	CROSSKEYPRESS_LEFT,		// 左
	CROSSKEYPRESS_MAX,
}CROSSKEYPRESS;

//--------------------------------------------------------------------------------------------------------------
// 十字キーのトリガー情報
//--------------------------------------------------------------------------------------------------------------
typedef enum
{
	CROSSKEYTRIGGER_UP = 0,		// 上
	CROSSKEYTRIGGER_DOWN,		// 下
	CROSSKEYTRIGGER_RIGHT,		// 右
	CROSSKEYTRIGGER_LEFT,		// 左
	CROSSKEYTRIGGER_MAX,
}CROSSKEYTRIGGER;

//--------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);										// ゲームパッドの初期化処理
void UninitGamepad(void);																	// ゲームパッドの終了処理
void UpdateGamepad(void);																	// ゲームパッドの更新処理

void LStickPressInfo(int nCnt,DIJOYSTATE2 State);											// Lスティックのプレス処理
void RStickPressInfo(int nCnt, DIJOYSTATE2 State);											// Rスティックのプレス処理
void StickTriggerInfo(int nCnt, DIJOYSTATE2 State);											// スティックのトリガー処理

void CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State);										// ボタンのプレス処理
void CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State);										// ボタンのトリガー処理

bool GetJoypadPress(int nCnt,int nButton);													// ボタンのプレス情報の取得
bool GetJoypadTrigger(int nCnt, int nButton);												// ボタンのトリガー情報の取得
bool GetJoypadReleace(int nCnt, int nButton);												// ボタンのリリース情報の取得

DIJOYSTATE2 GetControllerPless(int nCnt);													// コントローラーのプレス情報の取得
bool *GetLStickPress(int nCnt);																// Lスティックのプレス情報の取得
bool *GetRStickPress(int nCnt);																// Rスティックのプレス情報の取得
bool *GetStickTrigger(int nCnt);															// スティックのトリガー情報の取得

bool *GetCrossKeyPress(int nCnt);															// ボタンのプレス情報の取得
bool *GetCrossKeyTrigger(int nCnt);															// ボタンのトリガー情報の取得

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// コールバック関数
																							
int GetnCntController(void);																// 接続されているコントローラー数の取得

#endif
