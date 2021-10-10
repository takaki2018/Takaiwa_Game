//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "title.h"
#include "player.h"
#include "manager.h"
#include "input_keyboard.h"
#include "fade.h"
#include "logo.h"
#include "title_logo.h"
#include "bg.h"
#include "UI_bg.h"
#include "sound.h"
#include "fade.h"
#include "model.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CTitle::m_bPressEnter = false;				// エンターを押したかどうか
CUIBg *CTitle::m_apLogoBg[MAX_LOGOBG] = {};		// ロゴ背景情報
												
//=============================================================================
// CTitleのコンストラクタ
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// CTitleのデストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// UI背景テクスチャのロード
	CUIBg::Load();

	// 背景の生成
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f),
		CBg::TEXTURETYPE_GAME);

	// UI背景の生成
	CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 370.0f, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH, 320.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		CUIBg::TEXTURETYPE_UI);

	// ロゴ背景の初期化
	for (int nCntLogoBg = 0; nCntLogoBg < MAX_LOGOBG; nCntLogoBg++)
	{
		m_apLogoBg[nCntLogoBg] = CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 370.0f + nCntLogoBg * 45.0f, 0.0f),
			D3DXVECTOR2(1280.0f, 35.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CUIBg::TEXTURETYPE_LOGO);
	}

	// ロゴの生成
	CTitleLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 290.0f, 0.0f), D3DXVECTOR2(600.0f, 100.0f));

	// BGM
	CManager::GetSound()->Play(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// サウンド停止
	CManager::GetSound()->Stop();

	// UI背景テクスチャのアンロード
	CUIBg::Unload();

	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	// 画面遷移
	if (CTitleLogo::GetTransition() && CFade::GetFade() == CFade::FADE_NONE)
	{
		// ゲームに遷移
		CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{

}