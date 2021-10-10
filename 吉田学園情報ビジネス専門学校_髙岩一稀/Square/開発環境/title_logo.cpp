//=============================================================================
//
// タイトルロゴ処理 [title_logo.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "title_logo.h"
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "input_keyboard.h"
#include "ui_bg.h"
#include "title.h"
#include "sound.h"
#include "tutorial.h"
#include "fade.h"
#include "input_joypad.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CTitleLogo::bTransition = false;		// 遷移するかどうか

//=============================================================================
// CTitleLogoのコンストラクタ
//=============================================================================
CTitleLogo::CTitleLogo(int nPriority) : CScene(nPriority)
{
	// 変数の初期化
	for (int nCntTitleLogo = 0; nCntTitleLogo < LOGOTYPE_MAX; nCntTitleLogo++)
	{
		m_apLogo[nCntTitleLogo] = NULL;
	}
	m_nSelectLogo = LOGOTYPE_PLAY;
	bTransition = false;
}

//=============================================================================
// CTitleLogoのデストラクタ
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CTitleLogo *pTitleLogo;
	pTitleLogo = new CTitleLogo(CScene::PRIORITY_UI);

	// NULLチェック
	if (pTitleLogo != NULL)
	{
		// 初期化処理
		pTitleLogo->Init(pos, size);

		// オブジェクトタイプの設定
		pTitleLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pTitleLogo;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ロゴ背景情報を取得
	CUIBg **apLogoBg = CTitle::GetLogoBg();

	// ロゴの初期化処理
	m_apLogo[LOGOTYPE_TITLE] = CLogo::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), D3DXVECTOR2(300.0f, 80.0f), CLogo::TEXTURETYPE_TITLELOGO);
	m_apLogo[LOGOTYPE_PLAY] = CLogo::Create(apLogoBg[LOGOTYPE_PLAY - 1]->GetPosition(), D3DXVECTOR2(95.0f,30.0f), CLogo::TEXTURETYPE_PLAY);
	m_apLogo[LOGOTYPE_TUTORIAL] = CLogo::Create(apLogoBg[LOGOTYPE_TUTORIAL - 1]->GetPosition(), D3DXVECTOR2(200.0f, 30.0f), CLogo::TEXTURETYPE_TUTORIAL);

	m_nSelectLogo = LOGOTYPE_PLAY;

	// 各ロゴの色を設定
	for (int nCntLogo = LOGOTYPE_PLAY; nCntLogo <= LOGOTYPE_TUTORIAL; nCntLogo++)
	{
		if (m_nSelectLogo == nCntLogo)
		{
			// 現在選択されているものはα値1.0fに設定
			m_apLogo[nCntLogo]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			apLogoBg[nCntLogo - 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// それ以外はα値0.3fに設定
			m_apLogo[nCntLogo]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			apLogoBg[nCntLogo - 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitleLogo::Uninit(void)
{
	// ロゴの終了処理
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			m_apLogo[nCnttitleLogo]->Uninit();
		}
	}

	// 破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitleLogo::Update(void)
{
	// ロゴ背景情報を取得
	CUIBg **apLogoBg = CTitle::GetLogoBg();

	// ロゴの更新処理
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			m_apLogo[nCnttitleLogo]->Update();
		}
	}

	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	bool *pTrigger = CManager::GetInputJoypad()->GetStickTrigger(PLAYER_1);

	// フェード情報の取得
	CFade *pFade = CManager::GetFade();

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_UP) || pTrigger[CInputJoypad::STICK_AND_CROSSKEY_UP])
		{
			// 上にいくとき番号を減らす
			m_nSelectLogo--;

			if (m_nSelectLogo < LOGOTYPE_PLAY)
			{
				// コンティニューより小さくなったとき
				m_nSelectLogo = LOGOTYPE_TUTORIAL;
			}
			// 効果音
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_DOWN) || pTrigger[CInputJoypad::STICK_AND_CROSSKEY_DOWN])
		{
			// 下にいくとき番号を増やす
			m_nSelectLogo++;

			if (m_nSelectLogo > LOGOTYPE_TUTORIAL)
			{
				// クイットより大きくなったとき
				m_nSelectLogo = LOGOTYPE_PLAY;
			}
			// 効果音
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
		}

		// 各ロゴの色を設定
		for (int nCntLogo = LOGOTYPE_PLAY; nCntLogo <= LOGOTYPE_TUTORIAL; nCntLogo++)
		{
			if (m_nSelectLogo == nCntLogo)
			{
				// 現在選択されているものはα値1.0fに設定
				m_apLogo[nCntLogo]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				apLogoBg[nCntLogo - 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				// それ以外はα値0.3fに設定
				m_apLogo[nCntLogo]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
				apLogoBg[nCntLogo - 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			}
		}

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_SHOT_DOWN))
		{
			// 選択されているUIを参照し、どの処理をするか決定
			switch (m_nSelectLogo)
			{
				// Playロゴ
			case LOGOTYPE_PLAY:
				bTransition = true;
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_TRANSITION);		// 効果音
				break;

				// Tutorialロゴ
			case LOGOTYPE_TUTORIAL:
				// チュートリアル画面の生成処理
				CTutorial::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
				CManager::SetTutorial(true);
				break;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitleLogo::Draw(void)
{
	// ロゴの描画処理
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			m_apLogo[nCnttitleLogo]->Draw();
		}
	}
}