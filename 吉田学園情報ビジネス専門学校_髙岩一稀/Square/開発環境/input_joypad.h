//=============================================================================
//
// ジョイパッド処理 [input_joypad.h]
// Author : 
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

//-----------------------------------------------------------------------------
// ジョイパッドクラス(派生クラス)
//-----------------------------------------------------------------------------
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
		LSTICKPRESS_UP = 0,	// 上
		LSTICKPRESS_DOWN,		// 下
		LSTICKPRESS_RIGHT,		// 右
		LSTICKPRESS_LEFT,		// 左
		LSTICKPRESS_MAX,
	}LSTICKPRESS;

	typedef enum
	{
		RSTICKPRESS_UP = 0,		// 上
		RSTICKPRESS_DOWN,		// 下
		RSTICKPRESS_RIGHT,		// 右
		RSTICKPRESS_LEFT,		// 左
		RSTICKPRESS_MAX,
	}RSTICKPRESS;

	typedef enum
	{
		STICKTRIGGER_UP = 0,		// 上
		STICKTRIGGER_DOWN,			// 下
		STICKTRIGGER_RIGHT,			// 右
		STICKTRIGGER_LEFT,			// 左
		STICKTRIGGER_MAX,
	}STICKTRIGGER;

	typedef enum
	{
		CROSSKEYPRESS_UP = 0,	// 上
		CROSSKEYPRESS_DOWN,		// 下
		CROSSKEYPRESS_RIGHT,	// 右
		CROSSKEYPRESS_LEFT,		// 左
		CROSSKEYPRESS_MAX,
	}CROSSKEYPRESS;

	typedef enum
	{
		CROSSKEYTRIGGER_UP = 0,		// 上
		CROSSKEYTRIGGER_DOWN,		// 下
		CROSSKEYTRIGGER_RIGHT,		// 右
		CROSSKEYTRIGGER_LEFT,		// 左
		CROSSKEYTRIGGER_MAX,
	}CROSSKEYTRIGGER;

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

	struct CONTROLLER	// コントローラーの情報
	{
		LPDIRECTINPUTDEVICE8 pDevjoypad = NULL;	// ジョイパッドのデバイス情報
		DIJOYSTATE2 State;						// プレス
		DIJOYSTATE2 Trigger;					// トリガー
		DIJOYSTATE2 Release;					// リリース
	};								

	struct JPINFO	// コントローラーの操作情報
	{
		bool bLStickPress[LSTICKPRESS_MAX];				// Lスティックのプレス情報
		bool bRStickPress[RSTICKPRESS_MAX];				// Rスティックのプレス情報
		bool bStickTrigger[STICKTRIGGER_MAX];			// スティックのトリガー情報
		bool bStickOldTrigger[STICKTRIGGER_MAX];		// スティックのトリガー情報を保存
		bool bCrossKeyPress[CROSSKEYPRESS_MAX];			// 十字キーのプレス情報
		bool bCrossKeyTrigger[CROSSKEYTRIGGER_MAX];		// 十字キーのトリガー情報
		bool bCrossKeyOldTrigger[CROSSKEYTRIGGER_MAX];	// 十字キーのトリガー情報を保存
	};											

	static CONTROLLER m_Controller[MAX_CONTROLLER];			// コントローラー情報のポインタ
	static JPINFO m_Stick[MAX_CONTROLLER];					// スティック情報
	static int m_nNumController;							// 使用するコントローラーのカウント
};
#endif