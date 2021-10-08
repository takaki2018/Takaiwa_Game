//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : 
//
//=============================================================================
#include "bullet.h"
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "collision.h"
#include "life.h"
#include "setparticle.h"
#include "game.h"
#include "tile.h"
#include "UI.h"
#include "setparticle.h"
#include "spawn_enemy.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
// CBulletのコンストラクタ
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{
	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bulletType = BULETTTYPE_NONE;
}

//=============================================================================
// CPlayerのデストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CBullet::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CBullet::Unload(void)
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
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, BULLETTYPE type, BULLETDIRECTION direction)
{
	// ポインタ変数の生成
	CBullet *pBullet;
	pBullet = new CBullet(CScene::PRIORITY_BULLET);

	// NULLチェック
	if (pBullet != NULL)
	{
		// 初期化処理
		pBullet->Init(pos, move, size, type,direction);

		// テクスチャ設定
		pBullet->BindTexture(m_pTexture);

		// オブジェクトの種類設定
		pBullet->SetObjType(CScene::OBJTYPE_BULLET);
	}

	return pBullet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, BULLETTYPE type, BULLETDIRECTION direction)
{
	// メンバ変数の初期化
	m_move = move;
	m_bulletType = type;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// テクスチャ座標の設定
	switch (m_bulletType)
	{
	case BULLETTYPE_PLEYER:
		SetTex(D3DXVECTOR2(0.5f,1.0f), D3DXVECTOR2(2.0f, 1.0f));
		break;

	case BULLETTYPE_ENEMY:
		SetTex(D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(2.0f, 1.0f));
		break;
	}

	// 向きの設定
	switch (direction)
	{
	case BULLETDIRECTION_DOWN:
		SetRotateVertex(D3DX_PI);
		break;

	case BULLETDIRECTION_RIGHT:
		SetRotateVertex((D3DX_PI / 2.0f) * 3.0f);
		break;

	case BULLETDIRECTION_LEFT:
		SetRotateVertex(D3DX_PI / 2.0f);
		break;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = GetSize();

	// 位置の保存
	SetOldPosition(pos);

	// 弾の移動処理
	Move();

	// タイルとの当たり判定
	CollisionTile();
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 弾の移動処理
//=============================================================================
void CBullet::Move(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// 座標の設定
	pos += m_move;

	// 座標の設定
	CScene2D::SetPosition(pos);
}

//=============================================================================
// タイルとの当たり判定
//=============================================================================
void CBullet::CollisionTile(void)
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

			// タイルの種類を取得
			CTile::TILETYPE tileType = pTile->GetTileType();

			switch (tileType)
			{// 黒タイルの場合
			case CTile::TILETYPE_BLACK:
				if (m_bulletType == BULLETTYPE_PLEYER)
				{
					pTile->FillTileforBullet(pos, size, m_posOld, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), CTile::TILETYPE_WHITE,this);
				}
				break;
				// 白タイルの場合
			case CTile::TILETYPE_WHITE:
				if (m_bulletType == BULLETTYPE_ENEMY)
				{
					pTile->FillTileTypeWhite(pos, size);
				}
				break;
				// 透明タイルの場合
			case CTile::TILETYPE_INVISIBLE:
				if (pTile->CollisionObject(&pos, m_posOld, &m_move, size))
				{
					// 当たったら削除
					Uninit();
					return;
				}
				break;
				// 黒に変わるタイルだった場合
			case CTile::TILETYPE_CHANGE_BLACK:
				if (m_bulletType == BULLETTYPE_PLEYER)
				{
					pTile->FillTileTypeChangeBlack(pos, size);
					pTile->FillTileforBullet(pos, size, m_posOld, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), CTile::TILETYPE_WHITE, this);
				}
				break;

			default:
				break;
			}
		}
		// 次の情報へ
		pScene = pSceneNext;;
	}
}