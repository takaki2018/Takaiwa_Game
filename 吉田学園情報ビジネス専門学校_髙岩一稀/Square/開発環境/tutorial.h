//=============================================================================
//
// ロード画面ヘッダー (tutorial.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CURSOR	(TUTORIALUI_MAX - 1)		// 最大カーソル数
#define MAX_ARROW	(2)							// ページ捲り用矢印

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;			// ポリゴン2Dクラス
class CLogo;			// ロゴクラス

//*****************************************************************************
// チュートリアル画面クラス(派生クラス)
//*****************************************************************************
class CTutorial : public CScene
{
public:
	// チュートリアルで使用するテクスチャの種類　
	typedef enum
	{
		TEXTURE_NONE = 0,	// チュートリアル背景
		TEXTURE_KEYBOARD,	// キーボード操作
		TEXTURE_GAMEPAD,	// ゲームパッド操作
		TEXTURE_GAMESCREEN,	// ゲーム画面
		TEXTURE_GAME,		// ゲーム内容
		TEXTURE_CURSOR,		// カーソル
		TEXTURE_ARROW,		// 矢印
		TEXTURE_MAX,
	}TutorialTexture;

	// チュートリアルで使用するUIの種類　
	typedef enum
	{
		TUTORIALUI_NONE = 0,	// チュートリアル背景
		TUTORIALUI_KEYBOARD,	// キーボード操作
		TUTORIALUI_GAMEPAD,		// ゲームパッド操作
		TUTORIALUI_GAMESCREEN,	// ゲーム画面
		TUTORIALUI_GAME,		// ゲーム内容
		TUTORIALUI_MAX,
	}TutorialUI;

	CTutorial(int nPriolity);
	~CTutorial();

	static HRESULT Load(void);
	static void Unload(void);
	static CTutorial *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURE_MAX];	// チュートリアル画面のテクスチャのポインタ
	CScene2D *m_apTutorial[TUTORIALUI_MAX];				// チュートリアル情報のポインタ
	CScene2D *m_apCursor[MAX_CURSOR];					// カーソル情報のポインタ
	CScene2D *m_apArrow[MAX_ARROW];						// ページ捲り用矢印情報のポインタ
	CLogo *m_pLogo;										// ロゴのポインタ
	int m_nCursor;										// カーソルの位置
};

#endif
