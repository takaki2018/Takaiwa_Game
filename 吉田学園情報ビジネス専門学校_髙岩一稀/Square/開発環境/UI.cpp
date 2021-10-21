//=============================================================================
//
// UI処理 [UI.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "UI.h"

#include "score.h"
#include "number.h"
#include "life_ui.h"
#include "hiscore.h"
#include "game_logo.h"
#include "UI_bg.h"
#include "bg.h"
#include "count_down.h"
#include "gamedata_keep_object.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CScore *CUI::m_pScore = NULL;			// スコア情報のポインタ
CHiScore *CUI::m_pHiScore = NULL;		// ハイスコア情報のポインタ
CLifeUI *CUI::m_pLife = NULL;				// ライフ情報のポインタ
CUIBg *CUI::m_pLogoBg = NULL;			// ロゴ背景情報
CCountDown *CUI::m_pCountDown = NULL;	// カウントダウン情報のポインタ

//=============================================================================
// CUIのコンストラクタ
//=============================================================================
CUI::CUI()
{

}

//=============================================================================
// CUIのデストラクタ
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUI::Init(CManager::MODE mode)
{
	// ライフのテクスチャロード
	CLifeUI::Load();

	// UI背景のテクスチャロード
	CUIBg::Load();

	// UI背景の生成
	CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 40.0f, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH, 80.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		CUIBg::TEXTURETYPE_UI);

	// ゲーム画面に必要なUIロゴの生成
	CGameLogo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));

	// ライフの生成
	m_pLife = CLifeUI::Create(D3DXVECTOR3(600.0f, 40.0f, 0.0f), D3DXVECTOR2(LIFE_SIZE, LIFE_SIZE));

	// スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR2(20.0f, 40.0f),20.0f,CScore::SCOREPLACEMENT_LEFT);
	
	// ハイスコアの生成
	m_pHiScore = CHiScore::Create(D3DXVECTOR3(1240.0f, 50.0f, 0.0f), D3DXVECTOR2(20.0f, 40.0f), 20.0f,CScore::SCOREPLACEMENT_RIGHT);

	// 背景情報の初期化
	m_pLogoBg =  CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH, 80.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),
		CUIBg::TEXTURETYPE_LOGO);

	// カウントダウンの生成
	m_pCountDown = CCountDown::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(40.0f, 80.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUI::Uninit(void)
{
	// ライフのテクスチャアンロード
	CLifeUI::Unload();

	// UI背景のテクスチャアンロード
	CUIBg::Unload();

	// スコアの保存
	CGameDataKeepObject::SetScore(m_pScore->GetScore());
}

//=============================================================================
// 更新処理
//=============================================================================
void CUI::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CUI::Draw(void)
{

}