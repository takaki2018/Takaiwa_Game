//=============================================================================
//
// タイル復活処理 [tile_revival.cpp]
// Author : 
//
//=============================================================================
#include "tile_revival.h"
#include "manager.h"
#include "renderer.h"
#include "stage.h"
#include "scene.h"
#include "tile.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAAX_CORNER		(4)				// 角の数

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTileRevival::m_pTexture = NULL;

//=============================================================================
// CTileRevivalのコンストラクタ
//=============================================================================
CTileRevival::CTileRevival(int nPriority) : CScene2D(nPriority)
{
	// メンバ変数の初期化
	m_tileType = TILETYPE_NONE;
}

//=============================================================================
// CTileRevivalのデストラクタ
//=============================================================================
CTileRevival::~CTileRevival()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CTileRevival::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tile.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTileRevival::Unload(void)
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
CTileRevival *CTileRevival::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type, int nNum)
{
	// ポインタ変数の生成
	CTileRevival *pTileRevival;
	pTileRevival = new CTileRevival(CScene::PRIORITY_TILEREVIVAL);

	// NULLチェック
	if (pTileRevival != NULL)
	{
		// 初期化処理
		pTileRevival->Init(pos, size, type,nNum);

		// テクスチャ設定
		pTileRevival->BindTexture(m_pTexture);
	}

	return pTileRevival;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTileRevival::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type, int nNum)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_TILE);

	// メンバ変数の初期化
	m_tileType = type;

	if (m_tileType == TILETYPE_BLACK)
	{
		// テクスチャ座標の変更
		SetTex(D3DXVECTOR2(0.5f, 1.0f), D3DXVECTOR2(2, 1));
	}

	// 何番目のタイルか保存
	m_nNum = nNum;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTileRevival::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTileRevival::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();

	// サイズの取得
	D3DXVECTOR2 size = GetSize();

	// 角度の取得
	D3DXVECTOR3 rot = GetRotate();

	// サイズを加算
	size.x += 1.5f;
	size.y += 1.5f;

	// 角度を加算
	rot.z += 0.05f;

	// サイズを設定
	SetSize(size);

	// 角度を設定
	SetRotate(rot);

	if (size.x >= TILE_SIZE || size.y >= TILE_SIZE)
	{
		// 復活してない状態にする
		CStage::SetRevival(false, m_nNum);

		// タイルを黒くする
		CTile *pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, m_nNum);
		pTile->FillTile(CTile::TILETYPE_BLACK);
	}

	// 復活状態の取得
	if (CStage::GetRevival(m_nNum) == false)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTileRevival::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}