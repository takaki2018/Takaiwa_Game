//=============================================================================
//
// UI背景処理 [UI_bg.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "UI_bg.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CUIBg::m_pTexture[TEXTURETYPE_MAX] = {};		// テクスチャ情報のポインタ

//=============================================================================
// CUIBgのコンストラクタ
//=============================================================================
CUIBg::CUIBg(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CUIBgのデストラクタ
//=============================================================================
CUIBg::~CUIBg()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CUIBg::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_bg2.jpg", &m_pTexture[TEXTURETYPE_UI]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_bg.jpg", &m_pTexture[TEXTURETYPE_LOGO]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CUIBg::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < TEXTURETYPE_MAX; nCntTexture++)
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
CUIBg *CUIBg::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber)
{
	// ポインタ変数の生成
	CUIBg *pUIBg;
	pUIBg = new CUIBg(CScene::PRIORITY_UI);

	// NULLチェック
	if (pUIBg != NULL)
	{
		// 初期化処理
		pUIBg->Init(pos, size,col);

		// オブジェクトタイプの設定
		pUIBg->SetObjType(OBJTYPE_BG);

		// テクスチャ設定
		pUIBg->BindTexture(m_pTexture[nTextureNumber]);
	}

	return pUIBg;
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUIBg::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col)
{
	// CScoreの初期化処理
	CScene2D::Init(pos, size);

	// 色の設定
	SetCol(col);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUIBg::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUIBg::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CUIBg::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}