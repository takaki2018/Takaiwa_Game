//=============================================================================
//
// ジョイパッド処理 [input_joypad.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _INPUTJOYPAD_H_
#define _INPUTJOYPAD_H_

#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DATA		(256)		// 最大データ数
#define MAX_CONTROLLER	(1)			// 接続可能なコントローラーの最大数
#define PLAYER_1		(0)			// プレイヤー1

//*****************************************************************************
// ジョイパッドクラス(派生クラス)
//*****************************************************************************
class CInputJoypad : public CInput
{
public:
	typedef enum
	{
		JPINFO_SHOT_LEFT = 0,	// 左方向に進む弾を撃つ
		JPINFO_SHOT_UP,			// 上方向に進む弾を撃つ	
		JPINFO_SHOT_DOWN,		// 下方向に進む弾を撃つ
		JPINFO_SHOT_RIGHT,		// 右方向に進む弾を撃つ
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

	typedef enum
	{
		STICK_AND_CROSSKEY_UP = 0,	// 上
		STICK_AND_CROSSKEY_DOWN,	// 下
		STICK_AND_CROSSKEY_RIGHT,	// 右
		STICK_AND_CROSSKEY_LEFT,	// 左
		STICK_AND_CROSSKEY_MAX,
	}StickAndCrossKey;

	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	void LStickPressInfo(int nCnt, DIJOYSTATE2 State);											// Lスティックのプレス処理
	void RStickPressInfo(int nCnt, DIJOYSTATE2 State);											// Rスティックのプレス処理
	void StickTriggerInfo(int nCnt, DIJOYSTATE2 State);											// スティックのトリガー処理

	void CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State);										// ボタンのプレス処理
	void CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State);										// ボタンのトリガー処理

	bool GetJoypadPress(int nCnt, int nButton);													// ボタンのプレス情報の取得
	bool GetJoypadTrigger(int nCnt, int nButton);												// ボタンのトリガー情報の取得
	bool GetJoypadReleace(int nCnt, int nButton);												// ボタンのリリース情報の取得

	DIJOYSTATE2 GetControllerPless(int nCnt);													// コントローラーのプレス情報の取得
	bool *GetLStickPress(int nCnt);																// Lスティックのプレス情報の取得
	bool *GetRStickPress(int nCnt);																// Rスティックのプレス情報の取得
	bool *GetStickTrigger(int nCnt);															// スティックのトリガー情報の取得

	bool *GetCrossKeyPress(int nCnt);															// ボタンのプレス情報の取得
	bool *GetCrossKeyTrigger(int nCnt);															// ボタンのトリガー情報の取得

	int GetnCntController(void);

private:
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// コールバック関数

	typedef struct 
	{
		LPDIRECTINPUTDEVICE8 pDevjoypad = NULL;	// ジョイパッドのデバイス情報
		DIJOYSTATE2 State;						// プレス
		DIJOYSTATE2 Trigger;					// トリガー
		DIJOYSTATE2 Release;					// リリース
	}CONTROLLER;								// コントローラーの情報							

	typedef struct 
	{
		bool bLStickPress[STICK_AND_CROSSKEY_MAX];			// Lスティックのプレス情報
		bool bRStickPress[STICK_AND_CROSSKEY_MAX];			// Rスティックのプレス情報
		bool bStickTrigger[STICK_AND_CROSSKEY_MAX];			// スティックのトリガー情報
		bool bStickOldTrigger[STICK_AND_CROSSKEY_MAX];		// スティックのトリガー情報を保存
		bool bCrossKeyPress[STICK_AND_CROSSKEY_MAX];		// 十字キーのプレス情報
		bool bCrossKeyTrigger[STICK_AND_CROSSKEY_MAX];		// 十字キーのトリガー情報
		bool bCrossKeyOldTrigger[STICK_AND_CROSSKEY_MAX];	// 十字キーのトリガー情報を保存
	}JPINFO;												// コントローラーの操作情報										

	static CONTROLLER m_Controller[MAX_CONTROLLER];			// コントローラー情報のポインタ
	static JPINFO m_Stick[MAX_CONTROLLER];					// スティック情報
	static int m_nNumController;							// 使用するコントローラーのカウント
};
#endif