//=============================================================================
//
// ゲームロゴ処理 [game_logo.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "game_logo.h"
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "input_keyboard.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CGameLogo::bTransition = false;		// 遷移するかどうか

//=============================================================================
// CGameLogoのコンストラクタ
//=============================================================================
CGameLogo::CGameLogo(int nPriority) : CScene(nPriority)
{
	// 変数の初期化
	for (int nCntTitleLogo = 0; nCntTitleLogo < LOGOTYPE_MAX; nCntTitleLogo++)
	{
		m_apLogo[nCntTitleLogo] = NULL;
	}
}

//=============================================================================
// CGameLogoのデストラクタ
//=============================================================================
CGameLogo::~CGameLogo()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CGameLogo *CGameLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CGameLogo *pGameLogo;
	pGameLogo = new CGameLogo(CScene::PRIORITY_UI);

	// NULLチェック
	if (pGameLogo != NULL)
	{
		// 初期化処理
		pGameLogo->Init(pos, size);

		// オブジェクトタイプの設定
		pGameLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pGameLogo;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGameLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ロゴの初期化処理
	m_apLogo[LOGOTYPE_SCORE] = CLogo::Create(D3DXVECTOR3(80.0f, 20.0f, 0.0f), D3DXVECTOR2(80.0f, 20.0f), CLogo::TEXTURETYPE_SCORE);
	m_apLogo[LOGOTYPE_HIGHSCORE] = CLogo::Create(D3DXVECTOR3(1170.0f, 20.0f, 0.0f), D3DXVECTOR2(160.0f, 20.0f), CLogo::TEXTURETYPE_HIGHSCORE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGameLogo::Uninit(void)
{
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			// ロゴの終了処理
			m_apLogo[nCnttitleLogo]->Uninit();
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGameLogo::Update(void)
{
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			// ロゴの更新処理
			m_apLogo[nCnttitleLogo]->Update();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGameLogo::Draw(void)
{
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			// ロゴの描画処理
			m_apLogo[nCnttitleLogo]->Draw();
		}
	}
}