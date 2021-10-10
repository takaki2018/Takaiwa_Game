//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

#include <stdlib.h>
#include <time.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GET_KEYBOARD		(CManager::GetInputKeyboard())		// キー情報の取得

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;			// レンダラクラス
class CInputKeyboard;		// キーボードクラス
class CInputJoypad;			// ジョイパッドクラス
class CSound;				// サウンドクラス
class CPlayer;				// プレイヤークラス
class CScore;				// スコアクラス
class CLife;				// ライフクラス
class CTitle;				// タイトルクラス
class CTutorial;			// チュートリアルクラス
class CGame;				// ゲームクラス
class CResult;				// リザルトクラス
class CFade;				// フェードクラス
class CGameDataKeepObject;	// ゲームデータ保存用オブジェクトクラス
class CCamera;				// カメラクラス
class CLight;				// ライトクラス

//*****************************************************************************
// マネージャークラス(基本クラス)
//*****************************************************************************
class CManager
{
public:
	// モードの種類
	typedef enum
	{
		MODE_TITLE = 0,		// タイトル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadTexture(void);
	void UnloadTexture(void);
	static void SetMode(MODE mode);

	static MODE GetMode(void) { return m_mode; }
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
	static CFade *GetFade(void) { return m_pFade; }
	static CGameDataKeepObject *GetGameData(void) { return m_pGemeData; }
	static CSound *GetSound(void) { return m_pSound; }
	static bool GetPause(void) { return m_bPause; }
	static void SetPause(void);
	static void SetPause(bool bPause) { m_bPause = bPause; }
	static bool GetTutorial(void) { return m_bTutorial; }
	static void SetTutorial(bool bTutorial) { m_bTutorial = bTutorial; }
	static CCamera *GetCamera(void) { return m_pCamera; }

private:
	static CRenderer *m_pRenderer;				// レンダラ情報のポインタ
	static CInputKeyboard *m_pInputKeyboard;	// キーボード情報のポインタ
	static CInputJoypad *m_pInputJoypad;		// ジョイパッド情報のポインタ
	static CSound *m_pSound;					// サウンド情報のポインタ
	static CTitle *m_pTitle;					// タイトル情報のポインタ
	static CTutorial *m_pTutorial;				// チュートリアル情報のポインタ
	static CGame *m_pGame;						// ゲーム情報のポインタ
	static CResult *m_pResult;					// リザルト情報のポインタ
	static MODE m_mode;							// モード情報
	static CFade *m_pFade;						// フェード情報のポインタ
	static bool m_bPause;						// ポーズするかどうか
	static bool m_bTutorial;					// チュートリアル画面かどうか
	static CGameDataKeepObject *m_pGemeData;	// ゲームデータ保存用オブジェクト情報のポインタ
	static CCamera *m_pCamera;					// カメラ情報のポインタ
	static CLight *m_pLight;					// ライト情報のポインタ
};
#endif