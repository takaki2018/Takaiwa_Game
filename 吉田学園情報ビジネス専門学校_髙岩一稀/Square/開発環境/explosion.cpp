//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "score.h"
#include "game.h"
#include "UI.h"
#include "tile.h"
#include "spawn_enemy.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture[EXPLOSION_TEXTURETYPE_MAX] = {};	// テクスチャ情報のポインタ

//=============================================================================
// CExplosionのコンストラクタ
//=============================================================================
CExplosion::CExplosion(int nPriority) : CScene2D(nPriority)
{
	// メンバ変数の初期化
	m_explosion.sizeMove = 0.0f;
	m_explosion.nCnt = 0;
	m_explosion.nCntSwitch = 0;
	m_explosion.explosionType = EXPLOSIONTYPE_NONE;
	m_explosion.bSwitch = false;
}

//=============================================================================
// CPlayerのデストラクタ
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CExplosion::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_explosion.png", &m_pTexture[EXPLOSION_TEXTURETYPE_NORMAL]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_explosion_wave.png", &m_pTexture[EXPLOSION_TEXTURETYPE_WAVE]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CExplosion::Unload(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < EXPLOSION_TEXTURETYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, float sizeMove, float fInertia,int nCntSwitch, float fMoveAlpha, ExplosionType explosionType, Explosion_TextureType textureType)
{
	// ポインタ変数の生成
	CExplosion *pExplosion;
	pExplosion = new CExplosion(CScene::PRIORITY_EFFECT);

	// NULLチェック
	if (pExplosion != NULL)
	{
		// 初期化処理
		pExplosion->Init(pos, size,col,sizeMove, fInertia,nCntSwitch, fMoveAlpha,explosionType);

		// オブジェクトタイプの設定
		pExplosion->SetObjType(OBJTYPE_EXPLOSION);

		// テクスチャ設定
		pExplosion->BindTexture(m_pTexture[textureType]);
	}

	return pExplosion;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, float sizeMove, float fInertia,int nCntSwitch, float fMoveAlpha, ExplosionType explosionType)
{
	// メンバ変数の初期化
	m_explosion.sizeMove = sizeMove;
	m_explosion.fInertia = fInertia;
	m_explosion.nCnt = 0;
	m_explosion.nCntSwitch = nCntSwitch;
	m_explosion.explosionType = explosionType;
	m_explosion.bSwitch = false;
	m_explosion.fMoveAlpha = fMoveAlpha;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// 色の設定
	CScene2D::SetCol(col);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CExplosion::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CExplosion::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();

	// タイプによって処理を分ける
	if (m_explosion.explosionType == EXPLOSIONTYPE_WAVE)
	{
		// 衝撃波処理
		WaveExplosion();
	}
	else
	{
		// 通常処理
		NormalExplosion();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CExplosion::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 通常爆発処理
//=============================================================================
void CExplosion::NormalExplosion(void)
{
	// サイズの取得
	D3DXVECTOR2 size = GetSize();

	// カウント更新
	m_explosion.nCnt++;

	// サイズが増加しているとき
	if (m_explosion.bSwitch == false)
	{
		// 切り返しカウントと同じになったらサイズ増加量に-1.0fを乗算する
		if (m_explosion.nCnt == m_explosion.nCntSwitch)
		{
			m_explosion.sizeMove *= -1.0f;

			// 切り替えたことを伝える
			m_explosion.bSwitch = true;
		}
	}

	// 爆発がプレイヤー以外のだったとき
	if (m_explosion.explosionType != EXPLOSIONTYPE_PLAYER)
	{
		if (HitEnemy())
		{
			return;
		}
	}
	// タイルとのあたり判定
	CollosionTile();

	// サイズが0いかになったら削除
	if (size.x < 0.0f)
	{
		Uninit();

		return;
	}
	// 色情報の取得
	D3DXCOLOR col = GetCol();

	// α値の移動
	col.a -= m_explosion.fMoveAlpha;;

	// 色の設定
	SetCol(col);

	// α値が0.0f以下の時削除
	if (col.a <= 0.0f)
	{
		Uninit();

		return;
	}

	// 慣性
	m_explosion.sizeMove += (0.0f - m_explosion.sizeMove) * m_explosion.fInertia;

	// サイズの更新
	size.x += m_explosion.sizeMove;
	size.y += m_explosion.sizeMove;

	// サイズの設定
	SetSize(size);
}

//=============================================================================
// 衝撃波処理
//=============================================================================
void CExplosion::WaveExplosion(void)
{
	// サイズの取得
	D3DXVECTOR2 size = GetSize();

	// 色情報の取得
	D3DXCOLOR col = GetCol();

	// サイズの更新
	size.x += m_explosion.sizeMove;
	size.y += m_explosion.sizeMove;

	// α値の移動
	col.a -= m_explosion.fMoveAlpha;;

	// サイズの設定
	SetSize(size);

	// 色の設定
	SetCol(col);

	// α値が0.0f以下の時削除
	if (col.a <= 0.0f)
	{
		Uninit();
	}
}

//=============================================================================
// 敵との当たり判定処理
//=============================================================================
bool CExplosion::HitEnemy(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// オブジェクト情報の取得
	CScene *pScene = GetScene(CScene::PRIORITY_ENEMY);

		// NULLチェック
	while (pScene)
	{
		// 次の情報を取得
		CScene *pSceneNext = pScene->GetNext();

		if (!pScene->GetDeath())
		{
			// オブジェクトの種類の取得
			CScene::OBJTYPE objType = pScene->GetObjType();

			// オブジェクトの種類がEnemyだったら処理
			if (objType == CScene::OBJTYPE_ENEMY)
			{
				// 敵インスタンス情報の取得
				CEnemy *pEnemy = (CEnemy*)pScene;

				// 位置の取得
				D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

				// サイズの取得
				D3DXVECTOR2 sizeEnemy = D3DXVECTOR2(pEnemy->GetSize().x / 2.0f, pEnemy->GetSize().y / 2.0f);

				// 矩形の当たり判定
				if (pos.x + size.x >= posEnemy.x - sizeEnemy.x && pos.x - size.x <= posEnemy.x + sizeEnemy.x &&
					pos.y - size.y <= posEnemy.y + sizeEnemy.y && pos.y + size.y >= posEnemy.y - sizeEnemy.y)
				{
					// 敵の破棄
					pEnemy->HitEnemy();

					return true;
				}
			}
		}
		// 次の情報へ
		pScene = pSceneNext;
	}
	return false;
}

//=============================================================================
// タイルとの当たり判定処理
//=============================================================================
void CExplosion::CollosionTile(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

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
			case CTile::TILETYPE_BLACK:
				if (m_explosion.explosionType != EXPLOSIONTYPE_PLAYER)
				{
					// タイルとの当たり判定
					pTile->FillTileTypeBlack(pos, size);
				}
				break;

			case CTile::TILETYPE_WHITE:
				if (m_explosion.explosionType == EXPLOSIONTYPE_PLAYER)
				{
					pTile->FillTileTypeWhite(pos, size);
				}
				break;

			case CTile::TILETYPE_CHANGE_BLACK:
				if (m_explosion.explosionType != EXPLOSIONTYPE_PLAYER)
				{
					// タイルとの当たり判定
					pTile->FillTileTypeChangeBlack(pos, size);
				}
				break;
			}
		}
		// 次の情報へ
		pScene = pSceneNext;
	}
}