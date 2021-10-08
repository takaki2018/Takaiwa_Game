//==============================================================================================================
//
// ポーズ (pause.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "pause.h"
#include "game.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "Scene2D.h"
#include "input_joypad.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_PATTERN				(5)					// アニメーションパターンNo.の最大数

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
LPDIRECT3DTEXTURE9 CPause::m_pTexture[CPause::PAUSEUI_MAX] = {};

//=============================================================================
// CBulletのコンストラクタ
//=============================================================================
CPause::CPause(int nPriority):CScene(nPriority)
{
	// 変数の初期化
	for (int nCntPauseUI = 0; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		m_apScene2D[nCntPauseUI] = NULL;
	}
	m_nPauseSelect = (int)PAUSEUI_CONTINUE;
}

//=============================================================================
// CPlayerのデストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CPause::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_menu_bg.png", &m_pTexture[PAUSEUI_MENUBG]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/continue_logo.png", &m_pTexture[PAUSEUI_CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/retry_logo.png", &m_pTexture[PAUSEUI_RETRY]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/quit_logo.png", &m_pTexture[PAUSEUI_QUIT]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CPause::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < PAUSEUI_MAX; nCntTexture++)
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
CPause *CPause::Create(D3DXVECTOR3 pos,D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CPause *pPause;
	pPause = new CPause(CScene::PRIORITY_PAUSE);

	// NULLチェック
	if (pPause != NULL)
	{
		// 初期化処理
		pPause->Init(pos, size);

		// オブジェクトタイプの設定
		pPause->SetObjType(OBJTYPE_PAUSE);
	}

	return pPause;
}

//--------------------------------------------------------------------------------------------------------------
// ポーズの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 変数宣言
	// ポーズ画面に使用するUIの位置
	D3DXVECTOR3 aPos[PAUSEUI_MAX] = { D3DXVECTOR3(pos.x,pos.y,pos.z),
		D3DXVECTOR3(pos.x,PAUSE_MENU_BG_Y,pos.z),
		D3DXVECTOR3(pos.x,LOGO_Y,pos.z),
		D3DXVECTOR3(pos.x,LOGO_Y + 70.0f,pos.z),
		D3DXVECTOR3(pos.x,LOGO_Y + 140.0f,pos.z)};

	// ポーズ画面に使用するUIの位置
	D3DXVECTOR2 aSize[PAUSEUI_MAX] = { D3DXVECTOR2(size.x,size.y),
		D3DXVECTOR2(PAUSE_MENU_BG_WIDTH,PAUSE_MENU_BG_HEIGHT),
		D3DXVECTOR2(LOGO_WIDTH,LOGO_HEIGHT),
		D3DXVECTOR2(LOGO_WIDTH,LOGO_HEIGHT),
		D3DXVECTOR2(LOGO_WIDTH,LOGO_HEIGHT) };

	for (int nCntPauseUI = 0; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		// メモリの確保
		m_apScene2D[nCntPauseUI] = new CScene2D(CScene::PRIORITY_NONE);

		// CScene2Dの初期化処理
		m_apScene2D[nCntPauseUI]->Init(aPos[nCntPauseUI], aSize[nCntPauseUI]);

		// テクスチャの割り当て
		if (m_pTexture[nCntPauseUI] != NULL)
		{
			m_apScene2D[nCntPauseUI]->BindTexture(m_pTexture[nCntPauseUI]);
		}
	}
	// ポーズ画面の背景のみ色を変える
	m_apScene2D[PAUSEUI_NONE]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f));

	// 選択されているUIの保存
	m_nPauseSelect = (int)PAUSEUI_CONTINUE;

	// 各UIの色を設定
	for (int nCntPauseUI = PAUSEUI_CONTINUE; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		if (m_nPauseSelect == nCntPauseUI)
		{
			// 現在選択されているものはα値1.0fに設定
			m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// それ以外はα値0.3fに設定
			m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// ポーズの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void CPause::Uninit(void)
{
	for (int nCntPauseUI = 0; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		// CScene2Dの終了処理
		m_apScene2D[nCntPauseUI]->Uninit();
	}

	// オブジェクト破棄
	Release();
}

//--------------------------------------------------------------------------------------------------------------
// ポーズの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void CPause::Update(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	bool *pTrigger = CManager::GetInputJoypad()->GetStickTrigger(PLAYER_1);

	// フェード情報の取得
	CFade *pFade = CManager::GetFade();

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_UP) || pTrigger[CInputJoypad::CROSSKEYTRIGGER_UP])
		{
			// 上にいくとき番号を減らす
			m_nPauseSelect--;

			if (m_nPauseSelect < PAUSEUI_CONTINUE)
			{
				// コンティニューより小さくなったとき
				m_nPauseSelect = PAUSEUI_QUIT;
			}
			// 効果音
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_DOWN) || pTrigger[CInputJoypad::CROSSKEYTRIGGER_DOWN])
		{
			// 下にいくとき番号を増やす
			m_nPauseSelect++;

			if (m_nPauseSelect > PAUSEUI_QUIT)
			{
				// クイットより大きくなったとき
				m_nPauseSelect = PAUSEUI_CONTINUE;
			}
			// 効果音
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
		}

		// 各UIの色を設定
		for (int nCntPauseUI = PAUSEUI_CONTINUE; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
		{
			if (m_nPauseSelect == nCntPauseUI)
			{
				// 現在選択されているものはα値1.0fに設定
				m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				// それ以外はα値0.3fに設定
				m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			}
		}

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_SHOT_DOWN))
		{
			// 選択されているUIを参照し、どの処理をするか決定
			switch (m_nPauseSelect)
			{
				// コンティニューボタン
			case PAUSEUI_CONTINUE:
				CManager::SetPause(false);										// ポーズを閉じる
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_PAUSE_OUT);	// 効果音
				break;
				// リトライボタン
			case PAUSEUI_RETRY:
				pFade->SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_START);	// 効果音
				break;
				// 終了ボタン
			case PAUSEUI_QUIT:
				pFade->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_START);	// 効果音
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ポーズの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void CPause::Draw(void)
{
	for (int nCntPauseUI = 0; nCntPauseUI < PAUSEUI_MAX; nCntPauseUI++)
	{
		// CScene2Dの描画処理
		m_apScene2D[nCntPauseUI]->Draw();
	}
}