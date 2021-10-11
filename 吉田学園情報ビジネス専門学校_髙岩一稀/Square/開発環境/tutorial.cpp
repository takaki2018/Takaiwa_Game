//-----------------------------------------------------------------
//
// チュートリアル画面 (tutorial.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "tutorial.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "Scene2D.h"
#include "logo.h"
#include "input_joypad.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_MENU_BG_X			(SCREEN_WIDTH / 2)	// ポーズメニューの中心座標X
#define PAUSE_MENU_BG_Y			(370.0f)			// ポーズメニューの中心座標Y
#define PAUSE_MENU_BG_WIDTH		(400.0f)			// ポーズメニューの幅
#define PAUSE_MENU_BG_HEIGHT	(400.0f)			// ポーズメニューの高さ

#define LOGO_X					(SCREEN_WIDTH / 2)	// ロゴの中心座標X(すべて共通)
#define LOGO_Y					(310.0f)			// ロゴの中心座標Y(すべて共通)
#define LOGO_WIDTH				(280.0f)			// ロゴの幅(すべて共通)
#define LOGO_HEIGHT				(70.0f)				// ロゴの高さ(すべて共通)

#define ALUFA_INTERBAL			(10)				// 点滅のインターバル

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture[CTutorial::TEXTURE_MAX] = {}; // チュートリアル画面のテクスチャのポインタ

//=============================================================================
// CTutorialのコンストラクタ
//=============================================================================
CTutorial::CTutorial(int nPriority) :CScene(nPriority)
{
	// 変数の初期化
	for (int nCntTutorialUI = 0; nCntTutorialUI < TUTORIALUI_MAX; nCntTutorialUI++)
	{
		m_apTutorial[nCntTutorialUI] = NULL;
	}
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		m_apCursor[nCntCursor] = NULL;
	}
	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		m_apArrow[nCntArrow] = NULL;
	}
	m_pLogo = NULL;
	m_nCursor = (int)TUTORIALUI_KEYBOARD;
}

//=============================================================================
// CTutorialのデストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CTutorial::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial_keyboard.png", &m_pTexture[TEXTURE_KEYBOARD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial_gamepad.png", &m_pTexture[TEXTURE_GAMEPAD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial_gamescreen.png", &m_pTexture[TEXTURE_GAMESCREEN]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial_game.png", &m_pTexture[TEXTURE_GAME]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/marker.png", &m_pTexture[TEXTURE_CURSOR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/arrow.png", &m_pTexture[TEXTURE_ARROW]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTutorial::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < TEXTURE_MAX; nCntTexture++)
	{
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CTutorial *CTutorial::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CTutorial *pTutorial;
	pTutorial = new CTutorial(CScene::PRIORITY_TUTORIAL);

	// NULLチェック
	if (pTutorial != NULL)
	{
		// 初期化処理
		pTutorial->Init(pos, size);

		// オブジェクトタイプの設定
		pTutorial->SetObjType(OBJTYPE_TUTORIAL);
	}

	return pTutorial;
}

//=============================================================================
// チュートリアルの初期化処理
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 変数宣言
	// ポーズ画面に使用するUIの位置
	D3DXVECTOR3 aPos[TUTORIALUI_MAX] = { D3DXVECTOR3(pos.x,pos.y,pos.z),
		D3DXVECTOR3(pos.x,340.0f,pos.z),
		D3DXVECTOR3(pos.x,340.0f,pos.z),
		D3DXVECTOR3(pos.x,340.0f,pos.z),
		D3DXVECTOR3(pos.x,340.0f,pos.z) };

	// ポーズ画面に使用するUIの位置
	D3DXVECTOR2 aSize[TUTORIALUI_MAX] = { D3DXVECTOR2(size.x,size.y),
		D3DXVECTOR2(960.0f,540.0f),
		D3DXVECTOR2(960.0f,540.0f),
		D3DXVECTOR2(960.0f,540.0f),
		D3DXVECTOR2(960.0f,540.0f) };

	// UIの初期化
	for (int nCntTutorialUI = 0; nCntTutorialUI < TUTORIALUI_MAX; nCntTutorialUI++)
	{
		// メモリの確保
		m_apTutorial[nCntTutorialUI] = new CScene2D(CScene::PRIORITY_NONE);

		// CScene2Dの初期化処理
		m_apTutorial[nCntTutorialUI]->Init(aPos[nCntTutorialUI], aSize[nCntTutorialUI]);

		// テクスチャの割り当て
		if (m_pTexture[nCntTutorialUI] != NULL)
		{
			m_apTutorial[nCntTutorialUI]->BindTexture(m_pTexture[nCntTutorialUI]);
		}
	}
	// チュートリアル画面の後ろを薄くする
	m_apTutorial[TUTORIALUI_NONE]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f));

	// 選択されているUIの保存
	m_nCursor = (int)TUTORIALUI_KEYBOARD;

	// カーソルの初期化
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		// メモリの確保
		m_apCursor[nCntCursor] = new CScene2D(CScene::PRIORITY_NONE);

		// カーソルの初期化処理
		m_apCursor[nCntCursor]->Init(D3DXVECTOR3(550.0f + 60.0f * nCntCursor, 650.0f, 0.0f), D3DXVECTOR2(20.0f, 20.0f));

		// テクスチャの割り当て
		if (m_pTexture[TEXTURE_CURSOR] != NULL)
		{
			m_apCursor[nCntCursor]->BindTexture(m_pTexture[TEXTURE_CURSOR]);
		}

		if (m_nCursor == nCntCursor)
		{
			// 現在選択されているものはα値1.0fに設定
			m_apCursor[nCntCursor]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// それ以外はα値0.3fに設定
			m_apCursor[nCntCursor]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}
	// 矢印の初期化
	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		// メモリの確保
		m_apArrow[nCntArrow] = new CScene2D(CScene::PRIORITY_NONE);

		// 矢印の初期化処理
		m_apArrow[nCntArrow]->Init(D3DXVECTOR3(100.0f + 1080.0f * nCntArrow, 330.0f, 0.0f), D3DXVECTOR2(100.0f,100.0f));

		// テクスチャの割り当て
		if (m_pTexture[TEXTURE_CURSOR] != NULL)
		{
			m_apArrow[nCntArrow]->BindTexture(m_pTexture[TEXTURE_ARROW]);
		}

		if (nCntArrow == 0)
		{
			// テクスチャの向きを反対にする
			m_apArrow[nCntArrow]->SetTex(D3DXVECTOR2(1,1), D3DXVECTOR2(-1, 1));
		}
	}
	// ロゴの初期化
	m_pLogo = CLogo::Create(D3DXVECTOR3(1100.0f,650.0f,0.0f),D3DXVECTOR2(200.0f,50.0f),CLogo::TEXTURETYPE_PRESSENTER);

	return S_OK;
}

//=============================================================================
// チュートリアルの終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// チュートリアルUIの終了処理
	for (int nCntTutorialUI = 0; nCntTutorialUI < TUTORIALUI_MAX; nCntTutorialUI++)
	{
		if (m_apTutorial[nCntTutorialUI] != NULL)
		{
			m_apTutorial[nCntTutorialUI]->Uninit();
		}
	}

	// カーソルの終了処理
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		if (m_apCursor[nCntCursor] != NULL)
		{
			m_apCursor[nCntCursor]->Uninit();
		}
	}

	// 矢印の終了処理
	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (m_apArrow[nCntArrow] != NULL)
		{
			m_apArrow[nCntArrow]->Uninit();
		}
	}

	// ロゴの終了処理
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
	}

	// オブジェクト破棄
	Release();
}

//=============================================================================
// チュートリアルの更新処理
//=============================================================================
void CTutorial::Update(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	bool *pTrigger = CManager::GetInputJoypad()->GetStickTrigger(PLAYER_1);

	// フェード情報の取得
	CFade *pFade = CManager::GetFade();

	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_LEFT) || pTrigger[CInputJoypad::STICK_AND_CROSSKEY_LEFT])
	{
		// 上にいくとき番号を減らす
		m_nCursor--;

		if (m_nCursor < TUTORIALUI_KEYBOARD)
		{
			// コンティニューより小さくなったとき
			m_nCursor = (TUTORIALUI_MAX - 1);
		}
		// 効果音
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
	}
	else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_RIGHT) || pTrigger[CInputJoypad::STICK_AND_CROSSKEY_RIGHT])
	{
		// 下にいくとき番号を増やす
		m_nCursor++;

		if (m_nCursor > (TUTORIALUI_MAX - 1))
		{
			// クイットより大きくなったとき
			m_nCursor = TUTORIALUI_KEYBOARD;
		}
		// 効果音
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
	}

	// 各UIの色を設定
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		if ((m_nCursor - 1) == nCntCursor)
		{
			// 現在選択されているものはα値1.0fに設定
			m_apCursor[nCntCursor]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// それ以外はα値0.3fに設定
			m_apCursor[nCntCursor]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	// カーソルが右端の時ロゴを点滅させる
	if (m_nCursor == TUTORIALUI_MAX - 1)
	{
		if (m_pLogo != NULL)
		{
			m_pLogo->Update();

			m_pLogo->StartBlinking(20);
		}
	}
	// カーソルが最後かつエンターキーを押したときチュートリアル画面を閉じる
	if (m_nCursor == TUTORIALUI_GAME && (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_SHOT_DOWN)))
	{
		// falseにする
		CManager::SetTutorial(false);

		// 終了処理
		Uninit();

		return;
	}
}

//=============================================================================
// チュートリアルの描画処理
//=============================================================================
void CTutorial::Draw(void)
{
	// チュートリアルの描画処理
	m_apTutorial[TUTORIALUI_NONE]->Draw();
	m_apTutorial[m_nCursor]->Draw();
		
	// カーソルの描画
	for (int nCntCursor = 0; nCntCursor < MAX_CURSOR; nCntCursor++)
	{
		if (m_apCursor[nCntCursor] != NULL)
		{
			m_apCursor[nCntCursor]->Draw();
		}
	}

	// カーソルが右端にいるとき表示
	if (m_nCursor == TUTORIALUI_MAX - 1)
	{
		m_pLogo->Draw();
	}

	// 矢印の描画処理
	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (m_apArrow[nCntArrow] != NULL)
		{
			m_apArrow[nCntArrow]->Draw();
		}
	}
}