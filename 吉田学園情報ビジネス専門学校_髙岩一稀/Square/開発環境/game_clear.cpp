//=============================================================================
//
// ゲームクリア処理 [game_clear.cpp]
// Author : 
//
//=============================================================================
#include "game_clear.h"
#include "logo.h"
#include "ui_bg.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CGameClear::m_bTransition = false;
int CGameClear::m_nCntInterval = 0;
CGameClear::GameTransition CGameClear::m_gameTransition = CGameClear::GAMETRANSITION_CLEAR;

//=============================================================================
// CGameClearのコンストラクタ
//=============================================================================
CGameClear::CGameClear(int nPriority) : CScene(nPriority)
{
	// メンバ変数の初期化
	m_bTransition = false;
	m_nCntInterval = 0;
	m_pLogo = NULL;
}

//=============================================================================
// CGameClearのデストラクタ
//=============================================================================
CGameClear::~CGameClear()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CGameClear *CGameClear::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, GameTransition gameTransition)
{
	// ポインタ変数の生成
	CGameClear *pGameClear;
	pGameClear = new CGameClear(CScene::PRIORITY_UI);

	// NULLチェック
	if (pGameClear != NULL)
	{
		// 遷移条件の保存
		m_gameTransition = gameTransition;

		// 初期化処理
		pGameClear->Init(pos, size);

		// オブジェクトタイプの設定
		pGameClear->SetObjType(OBJTYPE_GAMECLEAR);
	}

	return pGameClear;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGameClear::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ロゴ情報の初期化
	switch (m_gameTransition)
	{
	case GAMETRANSITION_CLEAR:
		m_pLogo = CLogo::Create(pos, D3DXVECTOR2(400.0f, 70.0f), CLogo::TEXTURETYPE_GAMECLEAR);
		break;

	case GAMETRANSITION_OVER:
		m_pLogo = CLogo::Create(pos, D3DXVECTOR2(400.0f, 70.0f), CLogo::TEXTURETYPE_GAMEOVER);
		break;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGameClear::Uninit(void)
{
	// ロゴ情報の終了処理
	m_pLogo->Uninit();

	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGameClear::Update(void)
{
	// ロゴ情報の更新処理
	m_pLogo->Update();

	// カウントの増加
	m_nCntInterval++;

	// カウントがある程度進んだら遷移を許可する
	if (m_nCntInterval % 120 == 0)
	{
		m_bTransition = true;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGameClear::Draw(void)
{
	// ロゴ情報の描画処理
	m_pLogo->Draw();
}