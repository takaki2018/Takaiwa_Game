//=============================================================================
//
// ライフ処理 [life.cpp]
// Author : istuki takaiwa
//
//=============================================================================
#include "life.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLife::m_pTexture = NULL;		// テクスチャ情報のポインタ

//=============================================================================
// CLifeのコンストラクタ
//=============================================================================
CLife::CLife(int nPriority) : CScene(nPriority)
{
	// 変数の初期化
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		m_apScene2D[nCntLife] = NULL;
	}
	m_nLife = MAX_LIFE;
}

//=============================================================================
// CBgのデストラクタ
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CLife::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CLife::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CLife *CLife::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CLife *pLife;
	pLife = new CLife(CScene::PRIORITY_UI);

	// NULLチェック
	if (pLife != NULL)
	{
		// 初期化処理
		pLife->Init(pos, size);

		// オブジェクトタイプの設定
		pLife->SetObjType(OBJTYPE_LIFE);
	}

	return pLife;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLife::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	for (int nCntLife = 0; nCntLife < m_nLife; nCntLife++)
	{
		// メモリの確保
		m_apScene2D[nCntLife] = new CScene2D(CScene::PRIORITY_UI);

		// CScene2Dの初期化処理
		m_apScene2D[nCntLife]->Init(D3DXVECTOR3(pos.x + 40.0f * nCntLife,pos.y,0.0f), size);

		// テクスチャの割り当て
		m_apScene2D[nCntLife]->BindTexture(m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLife::Uninit(void)
{
	for (int nCntLife = 0; nCntLife < m_nLife; nCntLife++)
	{
		// CScene2Dの終了処理
		m_apScene2D[nCntLife]->Uninit();
	}

	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLife::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CLife::Draw(void)
{
	for (int nCntLife = 0; nCntLife < m_nLife; nCntLife++)
	{
		// CScene2Dの描画処理
		m_apScene2D[nCntLife]->Draw();
	}
}

//=============================================================================
// ライフの設定処理
//=============================================================================
void CLife::SetLife(int nlife)
{
	// ライフのセット
	m_nLife = nlife;

	// CScene2Dの終了処理
	m_apScene2D[m_nLife]->Uninit();
}