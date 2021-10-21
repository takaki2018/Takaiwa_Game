//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "AI.h"
#include "bullet.h"
#include "setparticle.h"
#include "tile.h"
#include "explosion.h"
#include "game.h"
#include "UI.h"
#include "score.h"
#include "spawn_enemy.h"
#include "sound.h"
#include "model.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMYTYPE_NAX] = {}; // テクスチャ情報のポインタ

//=============================================================================
// CEnemyのコンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority) : CScene2D(nPriority)
{
	// メンバ変数の初期化
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_enemyType = ENEMYTYPE_NORMAL;
}

//=============================================================================
// CEnemyのデストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy.png", &m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CEnemy::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < MAX_ENEMYTYPE; nCntTexture++)
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
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,float fMoveAI, EnemyType enemyType)
{
	// ポインタ変数の生成
	CEnemy *pEnemy;
	pEnemy = new CEnemy(CScene::PRIORITY_ENEMY);

	// NULLチェック
	if (pEnemy != NULL)
	{
		// 初期化処理
		pEnemy->Init(pos, size,fMoveAI,enemyType);

		// テクスチャ設定
		pEnemy->BindTexture(m_pTexture[0]);

		// オブジェクトの種類設定
		pEnemy->SetObjType(CScene::OBJTYPE_ENEMY);
	}

	return pEnemy;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, EnemyType enemyType)
{
	// 変数の初期化
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_enemyType = enemyType;
	m_fMoveAI = fMoveAI;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// 位置の保存
	SetPosOld(pos);

	// オブジェクト情報の取得
	CScene *pScene= GetScene(CScene::PRIORITY_PLAYER);

	// NULLチェック
	while (pScene)
	{
		// 次の情報を取得
		CScene *pSceneNext = pScene->GetNext();

		// オブジェクトの種類の取得
		CScene::OBJTYPE objType = pScene->GetObjType();

		// オブジェクトの種類がEnemyだったら処理
		if (objType == CScene::OBJTYPE_PLAYER)
		{
			// 敵インスタンス情報の取得
			CPlayer *pPlayer = (CPlayer*)pScene;

			// 位置の取得
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

			// 追跡
			m_move = CAi::Seek(pos, posPlayer,m_fMoveAI);

			// 移動
			pos += m_move;
			
			// 座標の設定
			SetPosition(pos);

			// プレイヤーが無敵状態のときプレイヤーとの当たり判定を付与
			if (pPlayer->GetPlayerState() == CPlayer::PLAYERSTATE_INVINCIBLE)
			{
				CollisionPlayer(pPlayer);
			}
		}
		// 次の情報へ
		pScene = pSceneNext;
	}
	// 弾との当たり判定処理
	if (CollisionBullet())
	{
		return;
	}

	// タイルとのあたり判定
	pos = CollitionTile(pos, size);
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 敵破棄の前
//=============================================================================
void CEnemy::HitEnemy(void)
{
	// レンダラー情報の取得
	CRenderer *pRender = CManager::GetRenderer();

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// 爆発の生成
	CExplosion::Create(pos,
		D3DXVECTOR2(30.0f, 30.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		35.0f,
		0.13f,
		5,
		0.0f,
		CExplosion::EXPLOSIONTYPE_ENEMY,
		CExplosion::EXPLOSION_TEXTURETYPE_NORMAL);

	// 衝撃波の生成
	CExplosion::Create(pos,
		D3DXVECTOR2(30.0f, 30.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
		10.0f,
		0.0f,
		0,
		0.02f,
		CExplosion::EXPLOSIONTYPE_WAVE,
		CExplosion::EXPLOSION_TEXTURETYPE_WAVE);

	// 爆発パーティクル
	CParticle::SetExplosion(pos,
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		size,
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		0.02f,
		D3DXVECTOR2(5, 5));

	// スコア情報の取得
	CScore *pScore = CGame::GetUI()->GetScore();

	// スコアの加算
	pScore->AddScore(100);

	// 敵を倒した数を更新
	int nNumRemainingEnemy = CSpawnEnemy::GetRemainingEnemy();
	CSpawnEnemy::SetRemainingEnemy(nNumRemainingEnemy - 1);

	// モデルの角度移動量を加算
	CGame::GetModel()->AddMoveRotate(D3DXVECTOR3(0.01f, 0.0f, 0.01f));

	// 効果音
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HITENEMY);

	// フェードバックエフェクトの許可
	pRender->SetFadeBackEffect();

	// 敵の破棄
	Uninit();
}

//=============================================================================
// タイルとの当たり判定
//=============================================================================
D3DXVECTOR3 CEnemy::CollitionTile(D3DXVECTOR3 pos, D3DXVECTOR2 size)
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
			case CTile::TILETYPE_WHITE:
				// タイルとの当たり判定
				pTile->FillTileTypeWhite(pos, size);
				break;
			}
		}
		// 次の情報へ
		pScene = pSceneNext;
	}
	
	return pos;
}

//=============================================================================
// 弾との当たり判定処理
//=============================================================================
bool CEnemy::CollisionBullet(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// オブジェクト情報の取得
	CScene *pScene = GetScene(CScene::PRIORITY_BULLET);

	// 敵との当たり判定
	while (pScene)
	{
		// 次の情報を取得
		CScene *pSceneNext = pScene->GetNext();

		// オブジェクトの種類の取得
		CScene::OBJTYPE objType = pScene->GetObjType();

		// オブジェクトの種類がEnemyだったら処理
		if (objType == CScene::OBJTYPE_BULLET)
		{
			// 敵インスタンス情報の取得
			CBullet *pBullet = (CBullet*)pScene;

			if (pBullet->GetBulletType() == CBullet::BULLETTYPE_PLEYER)
			{
				// 位置の取得
				D3DXVECTOR3 posBullet = pBullet->GetPosition();

				// サイズの取得
				D3DXVECTOR2 sizeBullet = pBullet->GetSize();

				// 矩形の当たり判定
				if (pos.x + size.x >= posBullet.x - sizeBullet.x / 2.0f && pos.x - size.x <= posBullet.x + sizeBullet.x / 2.0f &&
					pos.y - size.y <= posBullet.y + sizeBullet.y / 2.0f && pos.y + size.y >= posBullet.y - sizeBullet.y / 2.0f)
				{
					// 弾の破棄
					pBullet->Uninit();

					// 敵の破棄
					HitEnemy();

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
// プレイヤーとの当たり判定処理
//=============================================================================
bool CEnemy::CollisionPlayer(CPlayer *pPlayer)
{
	// 当たったかどうか
	bool bCollision = false;

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// 前回の位置の取得
	D3DXVECTOR3 posOld = GetPosOld();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// プレイヤーの位置取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// プレイヤーの前回の位置の取得
	D3DXVECTOR3 posOldPlayer = pPlayer->GetPosOld();

	// プレイヤーのサイズの取得
	D3DXVECTOR2 sizePlayer = D3DXVECTOR2(pPlayer->GetSize().x / 2.0f, pPlayer->GetSize().y / 2.0f);

	if ((pos.x - size.x) < (posPlayer.x + sizePlayer.x) &&
		(pos.x + size.x) > posPlayer.x - sizePlayer.x)
	{// プレイヤーがブロックの中にいるとき
		if (posOld.y + size.y <= posPlayer.y - sizePlayer.y)
		{// プレイヤーがブロックの上にいるとき
			if (pos.y + size.y > (posPlayer.y - sizePlayer.y))
			{// めり込んだら処理
				pos.y = posPlayer.y - sizePlayer.y - size.y;
				m_move.y = 0.0f;

				bCollision = true;
			}
		}
		else if ((posOld.y - size.y) >= (posPlayer.y + sizePlayer.y))
		{// プレイヤーがブロックの下にいるとき
			if ((pos.y - size.y) < (posPlayer.y + sizePlayer.y))
			{// めり込んだらブロックの下の座標にする
				pos.y = posPlayer.y + sizePlayer.y + size.y;
				m_move.y = 0.0f;

				bCollision = true;
			}
		}
	}
	if ((pos.y + size.y) > posPlayer.y - sizePlayer.y && 
		(pos.y - size.y) < (posPlayer.y + sizePlayer.y))
	{// プレイヤーがブロックのY幅にいるとき
		if ((posOld.x + size.x) <= posPlayer.x - sizePlayer.x)
		{// プレイヤーがブロックの左側にいるとき
			if ((pos.x + size.x) > posPlayer.x - sizePlayer.x)
			{// めり込んだらブロックの左の座標にする
				pos.x = posPlayer.x - sizePlayer.x - size.x;
				m_move.x = 0.0f;

				bCollision = true;
			}
		}
		else if ((posOld.x - size.x) >= (posPlayer.x + sizePlayer.x))
		{// プレイヤーがブロックの右側にいるとき
			if ((pos.x - size.x) < (posPlayer.x + sizePlayer.x))
			{// めり込んだらブロックの右の座標にする
				pos.x = posPlayer.x + sizePlayer.x + size.x;
				m_move.x = 0.0f;

				bCollision = true;
			}
		}
	}
	// 位置を設定
	SetPosition(pos);

	return bCollision;
}