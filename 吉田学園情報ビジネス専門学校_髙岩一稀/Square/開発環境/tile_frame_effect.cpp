//=============================================================================
//
// タイルフレームエフェクト処理 [tile_frame_effect.cpp]
// Author : 
//
//=============================================================================
#include "tile_frame_effect.h"
#include "manager.h"
#include "renderer.h"
#include "tile.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTileFrameEffect::m_pTexture = NULL;

//=============================================================================
// CTileFrameEffectのコンストラクタ
//=============================================================================
CTileFrameEffect::CTileFrameEffect(int nPriority) : CScene2D(nPriority)
{
	// メンバ変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// CTileFrameEffectのデストラクタ
//=============================================================================
CTileFrameEffect::~CTileFrameEffect()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CTileFrameEffect::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_tile_frame.jpg", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTileFrameEffect::Unload(void)
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
CTileFrameEffect *CTileFrameEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR3 move)
{
	// ポインタ変数の生成
	CTileFrameEffect *pTileFrameEffect;
	pTileFrameEffect = new CTileFrameEffect(CScene::PRIORITY_EFFECT);

	// NULLチェック
	if (pTileFrameEffect != NULL)
	{
		// 初期化処理
		pTileFrameEffect->Init(pos, size, move);

		// テクスチャ設定
		pTileFrameEffect->BindTexture(m_pTexture);
	}

	return pTileFrameEffect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTileFrameEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR3 move)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_EFFECT);

	// メンバ変数の初期化
	m_move = move;

	// 色の取得
	D3DXCOLOR col = GetCol();

	// α値の設定
	col.a = 0.0f;

	// 色の設定
	SetCol(col);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTileFrameEffect::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTileFrameEffect::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();

	// 位置を取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = GetSize();

	// 色の取得
	D3DXCOLOR col = GetCol();

	// α値の増加
	col.a += 0.1f;

	if (col.a >= 0.6f)
	{
		col.a = 0.6f;
	}
	// 位置の保存
	m_posOld = pos;

	// 移動させる
	pos += m_move;

	// タイルとのあたり判定
	if (CollitionTile(pos, size))
	{
		return;
	}
	// 位置の設定
	SetPosition(pos);

	// 色の設定
	SetCol(col);
}

//=============================================================================
// 描画処理
//=============================================================================
void CTileFrameEffect::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// タイルとの当たり判定
//=============================================================================
bool CTileFrameEffect::CollitionTile(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// オブジェクト情報の取得
	CScene *pScene = GetScene(CScene::PRIORITY_TILE);

	// 敵との当たり判定
	while (pScene)
	{
		// 次の情報を取得
		CScene *pSceneNext = pScene->GetNext();

		// オブジェクトの種類の取得
		CScene::OBJTYPE objType = pScene->GetObjType();

		// オブジェクトの種類がEnemyだったら処理
		if (objType == CScene::OBJTYPE_TILE)
		{
			// 敵インスタンス情報の取得
			CTile *pTile = (CTile*)pScene;

			switch (pTile->GetTileType())
			{
			case CTile::TILETYPE_INVISIBLE:
				// タイルとの当たり判定
				if (pTile->CollisionObject(&pos, m_posOld, &m_move, size))
				{
					// 消す
					Uninit();

					return true;
				}
				break;

			default:
				break;
			}
		}
		// 次の情報へ
		pScene = pSceneNext;
	}
	return false;
}