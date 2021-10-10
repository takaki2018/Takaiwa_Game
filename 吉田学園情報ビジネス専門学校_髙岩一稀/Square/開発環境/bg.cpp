//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "bg.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBg::m_pTexture[TEXTURETYPE_MAX] = {}; // テクスチャ情報のポインタ

//=============================================================================
// CBgのコンストラクタ
//=============================================================================
CBg::CBg(int nPriority) : CScene2D(nPriority)
{
	// 変数の初期化
	m_nTextureNumber = 0;
}

//=============================================================================
// CBgのデストラクタ
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CBg::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tile.jpg", &m_pTexture[TEXTURETYPE_TITLE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stage_bg.jpg", &m_pTexture[TEXTURETYPE_GAME]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CBg::Unload(void)
{
	// テクスチャの破棄
	for (int nCntBg = 0; nCntBg < TEXTURETYPE_MAX; nCntBg++)
	{
		if (m_pTexture[nCntBg] != NULL)
		{
			m_pTexture[nCntBg]->Release();
			m_pTexture[nCntBg] = NULL;
		}			  
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber)
{
	// ポインタ変数の生成
	CBg *pBg;
	pBg = new CBg(CScene::PRIORITY_BG);

	// NULLチェック
	if (pBg != NULL)
	{
		// 初期化処理
		pBg->Init(pos, size,col,nTextureNumber);

		// オブジェクトタイプの設定
		pBg->SetObjType(OBJTYPE_BG);
	}

	return pBg;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// テクスチャ番号の保存
	m_nTextureNumber = nTextureNumber;

	// 色の指定
	SetCol(col);

	// テクスチャの割り当て
	BindTexture(m_pTexture[m_nTextureNumber]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBg::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBg::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CBg::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}