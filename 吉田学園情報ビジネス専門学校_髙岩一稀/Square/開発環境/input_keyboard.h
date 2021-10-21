//=============================================================================
//
// キーボード処理 [input_keyboard.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DATA							(256)												// 最大データ数
#define KEY_LookAtPoint_LEFT				(CInputKeyboard::KEYINFO_LookAtPoint_LEFT)			// 注視点左回転
#define KEY_LookAtPoint_RIGHT				(CInputKeyboard::KEYINFO_LookAtPoint_RIGHT)			// 注視点右回転
#define KEY_Point_LEFT						(CInputKeyboard::KEYINFO_Point_LEFT)				// 視点左回転
#define KEY_Point_RIGHT						(CInputKeyboard::KEYINFO_Point_RIGHT)				// 視点右回転
#define KEY_MOVE_UP							(CInputKeyboard::KEYINFO_UP)						// 上移動
#define KEY_MOVE_DOWN						(CInputKeyboard::KEYINFO_DOWN)						// 下移動
#define KEY_MOVE_RIGHT						(CInputKeyboard::KEYINFO_RIGHT)						// 右移動
#define KEY_MOVE_LEFT						(CInputKeyboard::KEYINFO_LEFT)						// 左移動
#define KEY_MOVE_LookAtPoint_UP				(CInputKeyboard::KEYINFO_LookAtPoint_UP)			// 注視点上移動
#define KEY_MOVE_LookAtPoint_DOWN			(CInputKeyboard::KEYINFO_LookAtPoint_DOWN)			// 注視点下移動
#define KEY_MOVE_Point_UP					(CInputKeyboard::KEYINFO_Point_UP)					// 視点上移動
#define KEY_MOVE_Point_DOWN					(CInputKeyboard::KEYINFO_Point_DOWN)				// 視点下移動

#define KEY_ADD_ALPHA						(CInputKeyboard::KEYINFO_ADD_ALPHA)					// α値増加
#define KEY_DECREASE_ALPHA					(CInputKeyboard::KEYINFO_DECREASE_ALPHA)			// α値減少
#define KEY_ADD_SIZE						(CInputKeyboard::KEYINFO_ADD_SIZE)					// サイズ増加
#define KEY_DECREASE_SIZE					(CInputKeyboard::KEYINFO_DECREASE_SIZE)				// サイズ減少
#define KEY_RESET							(CInputKeyboard::KEYINFO_LCORL)						// リセット

//*****************************************************************************
// キーボードクラス(派生クラス)
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	// キー入力情報
	typedef enum
	{
		KEYINFO_OK = 0,				// 決定
		KEYINFO_UP,					// 上選択
		KEYINFO_DOWN,				// 下選択
		KEYINFO_RIGHT,				// 右移動
		KEYINFO_LEFT,				// 左移動
		KEYINFO_PAUSE,				// ポーズ
		KEYINFO_SHOT_UP,			// 上方向に進む弾の発射
		KEYINFO_SHOT_DOWN,			// 下方向に進む弾の発射
		KEYINFO_SHOT_RIGHT,			// 右方向に進む弾の発射
		KEYINFO_SHOT_LEFT,			// 左方向に進む弾の発射
		KEYINFO_LookAtPoint_LEFT,	// 注視点左回転
		KEYINFO_LookAtPoint_RIGHT,	// 注視点右回転
		KEYINFO_Point_LEFT,			// 視点左回転
		KEYINFO_Point_RIGHT,		// 視点右回転
		KEYINFO_LookAtPoint_UP,		// 注視点上移動
		KEYINFO_LookAtPoint_DOWN,	// 注視点下移動
		KEYINFO_Point_UP,			// 視点上移動
		KEYINFO_Point_DOWN,			// 視点下移動
		KEYINFO_LCORL,				// リセット
		KEYINFO_ADD_ALPHA,			// α値増加
		KEYINFO_DECREASE_ALPHA,		// α値減少
		KEYINFO_ADD_SIZE,			// サイズ増加
		KEYINFO_DECREASE_SIZE,		// サイズ減少
		KEYINFO_MAX,
	}KEYINFO;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetReleace(int nKey);

private:
	BYTE m_aKeyState[MAX_DATA];				// キー情報
	BYTE m_aKeyStateTrigger[MAX_DATA];		// キートリガ-情報
	BYTE m_aKeyStateReleace[MAX_DATA];		// キーリリース情報

	int m_aKeyInfo[KEYINFO_MAX] =
	{
		DIK_RETURN,		// 決定
		DIK_W,			// 上選択
		DIK_S,			// 下選択 
		DIK_D,			// 右移動
		DIK_A,			// 左移動
		DIK_P,			// ポーズ
		DIK_UP,			// 上方向に進む弾の発射
		DIK_DOWN,		// 下方向に進む弾の発射
		DIK_RIGHT,		// 右方向に進む弾の発射
		DIK_LEFT,		// 左方向に進む弾の発射
		DIK_Q,			// 注視点左回転
		DIK_E,			// 注視点右回転
		DIK_Z,			// 視点左回転
		DIK_C,			// 視点右回転
		DIK_T,			// 注視点上移動
		DIK_B,			// 注視点下移動
		DIK_R,			// 視点上移動
		DIK_V,			// 視点下移動
		DIK_LCONTROL,	// 左コントロール
		DIK_0,			// α値増加
		DIK_9,			// α値減少
		DIK_6,			// サイズ増加
		DIK_5,			// サイズ減少
	};
};
#endif