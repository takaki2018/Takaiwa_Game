//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "tile.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[EFFECT_TYPE_MAX] = {};		// テクスチャ情報のポインタ

//=============================================================================
// CEffectのコンストラクタ
//=============================================================================
CEffect::CEffect(int nPriority) : CScene2D(nPriority)
{
	// 変数の初期化
	m_effect.move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_effect.col = D3DXCOLOR(0.220f,0.0f,0.0f,0.0f);
	m_effect.fRemove = 0.0f;
	m_effect.nCntAlpha = 0;
	m_effect.nLife = 0;
	m_effect.nInterval = 0;
}

//=============================================================================
// CEffectのデストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CEffect::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_explosion.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_tile.jpg", &m_pTexture[2]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CEffect::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < EFFECT_TYPE_MAX;nCntTexture++)
	{
		// テクスチャの破棄
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
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,float fRemove, D3DXVECTOR2 size, float fCntAlpha, int nLife, EFFECT_TYPE type, int nInterval)
{
	// ポインタ変数の生成
	CEffect *pEffect;
	pEffect = new CEffect(CScene::PRIORITY_EFFECT);

	// NULLチェック
	if (pEffect != NULL)
	{
		// 初期化処理
		pEffect->Init(pos, move, col, fRemove,size,fCntAlpha,nLife,nInterval);

		// テクスチャ設定
		pEffect->BindTexture(m_pTexture[type]);
	}

	return pEffect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRemove, D3DXVECTOR2 size, float fCntAlpha, int nLife, int nInterval)
{
	// メンバ変数の初期化
	m_effect.move = move;
	m_effect.col = col;
	m_effect.fRemove = fRemove;
	m_effect.nCntAlpha = fCntAlpha;
	m_effect.nLife = nLife;
	m_effect.nInterval = nInterval;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_EFFECT);

	// 頂点座標の設定
	SetPosition(pos);

	// 色の更新
	SetCol(m_effect.col);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	if (m_effect.nInterval <= 0)
	{
		// 中心座標の更新
		pos.x += m_effect.move.x;
		pos.y += m_effect.move.y;

		// 移動量の更新
		m_effect.move.x *= m_effect.fRemove;
		m_effect.move.y *= m_effect.fRemove;

		// α値の加算
		m_effect.col.a -= m_effect.nCntAlpha;

		// 寿命の更新
		m_effect.nLife--;

		// 色の更新
		SetCol(m_effect.col);

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
					pTile->EffectReflection(&pos, &m_effect.move, size);
					break;
				}
			}
			// 次の情報へ
			pScene = pSceneNext;
		}

		// 頂点座標の設定
		SetPosition(pos);

		// エフェクトの削除
		if (m_effect.nLife <= 0 || m_effect.col.a < 0.0f)
		{
			Uninit();

			return;
		}
	}
	else
	{
		// カウントを減らす
		m_effect.nInterval--;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}