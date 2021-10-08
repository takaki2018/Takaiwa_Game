//=============================================================================
//
// リザルトロゴ処理 [result_logo.cpp]
// Author : 
//
//=============================================================================
#include "result_logo.h"
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "input_keyboard.h"
#include "ui_bg.h"
#include "title.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CLogo *CResultLogo::m_apLogo[LOGOTYPE_MAX] = {};

//=============================================================================
// CResultLogoのコンストラクタ
//=============================================================================
CResultLogo::CResultLogo(int nPriority) : CScene(nPriority)
{
	// 変数の初期化
	for (int nCntResultLogo = 0; nCntResultLogo < LOGOTYPE_MAX; nCntResultLogo++)
	{
		m_apLogo[nCntResultLogo] = NULL;
	}
}

//=============================================================================
// CResultLogoのデストラクタ
//=============================================================================
CResultLogo::~CResultLogo()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CResultLogo *CResultLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CResultLogo *pResultLogo;
	pResultLogo = new CResultLogo(CScene::PRIORITY_UI);

	// NULLチェック
	if (pResultLogo != NULL)
	{
		// 初期化処理
		pResultLogo->Init(pos, size);

		// オブジェクトタイプの設定
		pResultLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pResultLogo;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResultLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ロゴの初期化処理
	m_apLogo[LOGOTYPE_RESULT] = CLogo::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), 
		D3DXVECTOR2(220.0f, 60.0f), 
		CLogo::TEXTURETYPE_RESULTLOGO);

	m_apLogo[LOGOTYPE_ARCADEMODE] = CLogo::Create(D3DXVECTOR3(pos.x, pos.y + 60.0f, 0.0f),
		D3DXVECTOR2(300.0f, 40.0f),
		CLogo::TEXTURETYPE_ARCADEMODE);

	m_apLogo[LOGOTYPE_YOURSCORE] = CLogo::Create(D3DXVECTOR3(400.0f, pos.y + 120.0f, 0.0f),
		D3DXVECTOR2(250.0f, 40.0f),
		CLogo::TEXTURETYPE_YOURSCORE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResultLogo::Uninit(void)
{
	// ロゴの終了処理
	for (int nCntResultLogo = 0; nCntResultLogo < LOGOTYPE_MAX; nCntResultLogo++)
	{
		if (m_apLogo[nCntResultLogo] != NULL)
		{
			m_apLogo[nCntResultLogo]->Uninit();
		}
	}

	// 破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResultLogo::Update(void)
{
	// ロゴの更新処理
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			m_apLogo[nCnttitleLogo]->Update();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResultLogo::Draw(void)
{
	// ロゴの描画処理
	for (int nCntResultLogo = 0; nCntResultLogo < LOGOTYPE_MAX; nCntResultLogo++)
	{
		if (m_apLogo[nCntResultLogo] != NULL)
		{
			m_apLogo[nCntResultLogo]->Draw();
		}
	}
}