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
#define MAX_DATA	(256)
#define PLAYER_1	(0)			// プレイヤー1

//-------------------------------------------------------------------
// キー操作の状態
//-------------------------------------------------------------------
typedef enum
{
	JPINFO_SHOOT = 0,		// 弾発射
	JPINFO_COLOR_CHANGE,	// 弾の色変更		
	JPINFO_OKorJAMP,		// 決定かジャンプ
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
}JOYPADINFO;

//-------------------------------------------------------------------
// スティックのプレス情報
//-------------------------------------------------------------------
typedef enum
{
	STICKPRESS_UP = 0,	// 上
	STICKPRESS_DOWN,		// 下
	STICKPRESS_RIGHT,		// 右
	STICKPRESS_LEFT,		// 左
	STICKPRESS_MAX,
}STICKPRESS;	

//-------------------------------------------------------------------
// スティックのトリガー情報
//-------------------------------------------------------------------
typedef enum
{
	STICKTRIGGER_UP = 0,		// 上
	STICKTRIGGER_DOWN,			// 下
	STICKTRIGGER_RIGHT,			// 右
	STICKTRIGGER_LEFT,			// 左
	STICKTRIGGER_MAX,
}STICKTRIGGER;	

//-------------------------------------------------------------------
// 十字キーのプレス情報
//-------------------------------------------------------------------
typedef enum
{
	CROSSKEYPRESS_UP = 0,	// 上
	CROSSKEYPRESS_DOWN,		// 下
	CROSSKEYPRESS_RIGHT,	// 右
	CROSSKEYPRESS_LEFT,		// 左
	CROSSKEYPRESS_MAX,
}CROSSKEYPRESS;

//-------------------------------------------------------------------
// 十字キーのトリガー情報
//-------------------------------------------------------------------
typedef enum
{
	CROSSKEYTRIGGER_UP = 0,		// 上
	CROSSKEYTRIGGER_DOWN,		// 下
	CROSSKEYTRIGGER_RIGHT,		// 右
	CROSSKEYTRIGGER_LEFT,		// 左
	CROSSKEYTRIGGER_MAX,
}CROSSKEYTRIGGER;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);										// ゲームパッドの初期化処理
void UninitGamepad(void);																	// ゲームパッドの終了処理
void UpdateGamepad(void);																	// ゲームパッドの更新処理

void StickPressInfo(int nCnt,DIJOYSTATE2 State);											// スティックのプレス処理
void StickTriggerInfo(int nCnt, DIJOYSTATE2 State);											// スティックのトリガー処理

void CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State);										// ボタンのプレス処理
void CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State);										// ボタンのトリガー処理

bool GetJoypadPress(int nCnt,int nButton);													// ボタンのプレス情報の取得
bool GetJoypadTrigger(int nCnt, int nButton);												// ボタンのトリガー情報の取得
bool GetJoypadReleace(int nCnt, int nButton);												// ボタンのリリース情報の取得

DIJOYSTATE2 GetControllerPless(int nCnt);													// コントローラーのプレス情報の取得
bool *GetStickPress(int nCnt);																// スティックのプレス情報の取得
bool *GetStickTrigger(int nCnt);															// スティックのトリガー情報の取得

bool *GetCrossKeyPress(int nCnt);															// ボタンのプレス情報の取得
bool *GetCrossKeyTrigger(int nCnt);															// ボタンのトリガー情報の取得

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// コールバック関数
																							
int GetnCntController(void);																// 接続されているコントローラー数の取得

#endif
