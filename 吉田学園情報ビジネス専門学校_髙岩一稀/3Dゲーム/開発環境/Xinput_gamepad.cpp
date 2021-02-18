//-----------------------------------------------------------------
//
// Xinput (Xinput_gamepad.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "Xinput_gamepad.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
XINPUT_STATE g_state;					// Xinputコントローラーの状態
XinputGamepad g_XinputGamepad;			// ゲームパッド情報

//-----------------------------------------------------------------
// ゲームパッド情報の読み込み
//-----------------------------------------------------------------
void LoadGamepad(void)
{
	// 変数の初期化
	g_XinputGamepad.bPressA = false;
	g_XinputGamepad.bPressLStick = false;
	g_XinputGamepad.fAngle = 0.0f;

	// 情報の初期化
	ZeroMemory(&g_state, sizeof(XINPUT_STATE));

	// Simply get the state of the controller from XInput.
	DWORD dwResult = XInputGetState(0, &g_state);

	if (dwResult == ERROR_SUCCESS)
	{
		// ボタンがおされているかどうか
		XinputButtons();

		// スティック処理
		XinputSticks();
	}
	else
	{
		// ゲームパッドの更新処理
		UpdateGamepad();
	}
}

//-----------------------------------------------------------------
// ボタン処理
//-----------------------------------------------------------------
void XinputButtons(void)
{
	if (g_state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{// Aボタンが押されたらtrueに設定
		g_XinputGamepad.bPressA = true;
	}
}

//-----------------------------------------------------------------
// スティック処理
//-----------------------------------------------------------------
void XinputSticks(void)
{
	// デッドゾーンの設定
	if ((g_state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		g_state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(g_state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			g_state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		g_state.Gamepad.sThumbLX = 0;
		g_state.Gamepad.sThumbLY = 0;
	}

	// JOYPADのスティック入力
	if (g_state.Gamepad.sThumbLX != 0 || g_state.Gamepad.sThumbLY != 0)
	{// スティックの入力角度
		g_XinputGamepad.fAngle = atan2f((float)g_state.Gamepad.sThumbLX, (float)g_state.Gamepad.sThumbLY * -1);
	
		// スティックを傾けたことを伝える
		g_XinputGamepad.bPressLStick = true;
	}
}

//-----------------------------------------------------------------
// ボタンが押されたかどうか取得
//-----------------------------------------------------------------
XinputGamepad *GetXinputGamepad(void)
{
	return &g_XinputGamepad;
}