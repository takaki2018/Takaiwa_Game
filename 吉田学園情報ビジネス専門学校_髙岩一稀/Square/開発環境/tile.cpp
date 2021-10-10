//=============================================================================
//
// タイル処理 [tile.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "tile.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "AI.h"
#include "bullet.h"
#include "enemy.h"
#include "setparticle.h"
#include "tile_revival.h"
#include "stage.h"
#include "count_down.h"
#include "game.h"
#include "UI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAAX_CORNER		(4)				// 角の数

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTile::m_pTexture = NULL;		// テクスチャ情報のポインタ

//=============================================================================
// CTileのコンストラクタ
//=============================================================================
CTile::CTile(int nPriority) : CScene2D(nPriority)
{
	// メンバ変数の初期化
	m_tileType = TILETYPE_NONE;
	m_pTileRevival = NULL;
	m_nNum = -1;
	m_nCntRevival = 0;
}

//=============================================================================
// CTileのデストラクタ
//=============================================================================
CTile::~CTile()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CTile::Load(void)
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
void CTile::Unload(void)
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
CTile *CTile::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,TILETYPE type, int nNum)
{
	// ポインタ変数の生成
	CTile *pTile;
	pTile = new CTile(CScene::PRIORITY_TILE);

	// NULLチェック
	if (pTile != NULL)
	{
		// 初期化処理
		pTile->Init(pos, size,type,nNum);

		// テクスチャ設定
		pTile->BindTexture(m_pTexture);
	}

	return pTile;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTile::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size,TILETYPE type, int nNum)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_TILE);

	// 種類によって色を変える
	FillTile(type);

	// 何番目のタイルか保存
	m_nNum = nNum;

	// インターバルの初期化
	m_nCntRevival = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTile::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTile::Update(void)
{
	// カウントダウン情報の取得
	CCountDown *pCountDown = CGame::GetUI()->GetCountDown();

	// カウントダウンが0ならタイルを復活させる
	if (pCountDown->GetCountDown() == 1 && m_tileType == TILETYPE_CHANGE_BLACK)
	{
		if (CStage::GetRevival(m_nNum) != true)
		{
			// 位置の取得
			D3DXVECTOR3 pos = GetPosition();

			// タイル復活エフェクトの生成
			CTileRevival::Create(pos, D3DXVECTOR2(1.0f, 1.0f), CTileRevival::TILETYPE_BLACK, m_nNum);

			// 復活している状態にする
			CStage::SetRevival(true, m_nNum);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTile::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 当たったのが黒いタイルの場合
//=============================================================================
void CTile::FillTileTypeBlack(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 位置の取得
	D3DXVECTOR3 posTile = GetPosition();

	// サイズの取得
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// 黒タイルとの当たり判定
	if (pos.x + size.x >= posTile.x - sizeTile.x && pos.x - size.x <= posTile.x + sizeTile.x &&
		pos.y - size.y <= posTile.y + sizeTile.y && pos.y + size.y >= posTile.y - sizeTile.y)
	{
		// タイルを白色にする
		FillTile(TILETYPE_WHITE);
	}
}

//=============================================================================
// 当たったのが白いタイルの場合
//=============================================================================
void CTile::FillTileTypeWhite(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 位置の取得
	D3DXVECTOR3 posTile = GetPosition();

	// サイズの取得
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// 黒タイルとの当たり判定
	if (pos.x + size.x >= posTile.x - sizeTile.x && pos.x - size.x <= posTile.x + sizeTile.x &&
		pos.y - size.y <= posTile.y + sizeTile.y && pos.y + size.y >= posTile.y - sizeTile.y)
	{
		if (m_nCntRevival < 0)
		{
			// タイルが黒に変わるとき
			FillTile(TILETYPE_CHANGE_BLACK);

			// タイル復活エフェクトの生成
			CTileRevival::Create(posTile, D3DXVECTOR2(1.0f, 1.0f), CTileRevival::TILETYPE_BLACK, m_nNum);

			// 復活している状態にする
			CStage::SetRevival(true, m_nNum);

			// プレイヤーがいるかどうか
			CollisionPlayer();
		}
		else
		{
			m_nCntRevival--;
		}
	}
}

//=============================================================================
// 当たったのが黒に変わるタイルの場合
//=============================================================================
void CTile::FillTileTypeChangeBlack(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 位置の取得
	D3DXVECTOR3 posTile = GetPosition();

	// サイズの取得
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// 黒タイルとの当たり判定
	if (pos.x + size.x >= posTile.x - sizeTile.x && pos.x - size.x <= posTile.x + sizeTile.x &&
		pos.y - size.y <= posTile.y + sizeTile.y && pos.y + size.y >= posTile.y - sizeTile.y)
	{
		// タイルを白色にする
		FillTile(TILETYPE_WHITE);

		// 復活してない状態にする
		CStage::SetRevival(false, m_nNum);

		m_nCntRevival = 15;
	}
}

//=============================================================================
// 当たったのが透明のタイルの場合
//=============================================================================
void CTile::EffectReflection(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR2 size)
{
	// 位置の取得
	D3DXVECTOR3 posTile = GetPosition();

	// サイズの取得
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// タイルの当たり判定
	if (pPos->x > posTile.x - sizeTile.x && pPos->x < posTile.x + sizeTile.x)
	{
		if (pPos->y > posTile.y - sizeTile.y && pPos->y < posTile.y + sizeTile.y + size.y)
		{
			pMove->y *= -1.0f;
			pPos->y = posTile.y + sizeTile.y + size.y;
		}
		else if (pPos->y > posTile.y - sizeTile.y - size.y && pPos->y < posTile.y + sizeTile.y)
		{
			pMove->y *= -1.0f;
			pPos->y = posTile.y - sizeTile.y - size.y;
		}
	}
	if (pPos->y > posTile.y - sizeTile.y && pPos->y < posTile.y + sizeTile.y)
	{
		if (pPos->x > posTile.x - sizeTile.x && pPos->x < posTile.x + sizeTile.x + size.x)
		{
			pMove->x *= -1.0f;
			pPos->x = posTile.x + sizeTile.x + size.x;
		}
		else if (pPos->x > posTile.x - sizeTile.x - size.x && pPos->x < posTile.x + sizeTile.x)
		{
			pMove->x *= -1.0f;
			pPos->x = posTile.x - sizeTile.x - size.x;
		}
	}
}

//=============================================================================
// 当たったのが透明のタイルの場合
//=============================================================================
bool CTile::CollisionObject(D3DXVECTOR3 *pos, D3DXVECTOR3 posOld,D3DXVECTOR3 *move ,D3DXVECTOR2 size)
{
	// 変数宣言
	bool bHitTile = false;

	// 位置の取得
	D3DXVECTOR3 posTile = GetPosition();

	// サイズの取得
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// タイルの当たり判定
	if (pos->x > posTile.x - sizeTile.x && pos->x < posTile.x + sizeTile.x)
	{
		if (pos->y > posTile.y - sizeTile.y && pos->y < posTile.y + sizeTile.y + size.y)
		{
			move->y = 0.0f;
			pos->y = posTile.y + sizeTile.y + size.y;
			bHitTile = true;
		}
		else if (pos->y > posTile.y - sizeTile.y - size.y && pos->y < posTile.y + sizeTile.y)
		{
			move->y = 0.0f;
			pos->y = posTile.y - sizeTile.y - size.y;
			bHitTile = true;
		}
	}
	if (pos->y > posTile.y - sizeTile.y && pos->y < posTile.y + sizeTile.y)
	{
		if (pos->x > posTile.x - sizeTile.x && pos->x < posTile.x + sizeTile.x + size.x)
		{
			move->x = 0.0f;
			pos->x = posTile.x + sizeTile.x + size.x;
			bHitTile = true;
		}
		else if(pos->x > posTile.x - sizeTile.x - size.x && pos->x < posTile.x + sizeTile.x)
		{
			move->x = 0.0f;
			pos->x = posTile.x - sizeTile.x - size.x;
			bHitTile = true;
		}
	}

	// タイルの四つ角の位置
	D3DXVECTOR3 aPos[MAAX_CORNER] = { D3DXVECTOR3(posTile.x + sizeTile.x,posTile.y + sizeTile.y,0.0f) ,
		D3DXVECTOR3(posTile.x + sizeTile.x,posTile.y - sizeTile.y,0.0f) ,
		D3DXVECTOR3(posTile.x - sizeTile.x,posTile.y + sizeTile.y,0.0f), 
		D3DXVECTOR3(posTile.x - sizeTile.x,posTile.y - sizeTile.y,0.0f) };

	for (int nCnt = 0; nCnt < MAAX_CORNER; nCnt++)
	{
		// 中心とタイルの角との距離を測る
		D3DXVECTOR3 vec = *pos - aPos[nCnt];
		float fLength = sqrtf(vec.x * vec.x + vec.y * vec.y);

		if (fLength < size.x)
		{
			// 円の半径が比較オブジェクトのサイズより小さい場合
			float fRate = 1.0f - (fLength / size.x);

			float x = vec.x * fRate;
			float y = vec.y * fRate;

			pos->x += x;
			pos->y += y;
		}
	}

	return bHitTile;
}

//=============================================================================
// 弾がタイルに当たったとき
//=============================================================================
void CTile::FillTileforBullet(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR3 posOld, D3DXCOLOR colEffect, TILETYPE tileType, CBullet *pBullet)
{
	// 位置の取得
	D3DXVECTOR3 posTile = GetPosition();

	// サイズの取得
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// 当たったかどうか
	bool bHitTile = false;

	// タイルの当たり判定
	if ((pos.x - size.x) < (posTile.x + sizeTile.x) && (pos.x + size.x) > posTile.x - sizeTile.x)
	{// オブジェクトがタイルの中にいるとき
		if (posOld.y + size.y <= posTile.y - sizeTile.y)
		{// オブジェクトがタイルの上にいるとき
			if (pos.y + size.y > (posTile.y - sizeTile.y))
			{
				CParticle::SetFillTile(posTile,
					D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					sizeTile,
					colEffect,
					CParticle::BULLETMOVE_UP,
					D3DXVECTOR2(3, 3));

				bHitTile = true;
			}
		}
		else if ((posOld.y - size.y) >= (posTile.y + sizeTile.y))
		{// プレイヤーがブロックのposTile
			if ((pos.y - size.y) < (posTile.y + sizeTile.y))
			{
				CParticle::SetFillTile(posTile,
					D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					sizeTile,
					colEffect,
					CParticle::BULLETMOVE_DOWN,
					D3DXVECTOR2(3, 3));

				bHitTile = true;
			}
		}
	}
	if ((pos.y + size.y) > posTile.y - sizeTile.y && (pos.y - size.y) < (posTile.y + sizeTile.y))
	{// プレイヤーがブロックのY幅にいるとき
		if ((posOld.x + size.x) <= posTile.x - sizeTile.x)
		{// プレイヤーがブロックの左側にいるとき
			if ((pos.x + size.x) > posTile.x - sizeTile.x)
			{
				CParticle::SetFillTile(posTile,
					D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					sizeTile,
					colEffect,
					CParticle::BULLETMOVE_LEFT,
					D3DXVECTOR2(3, 3));

				bHitTile = true;
			}
		}
		else if ((posOld.x - size.x) >= (posTile.x + sizeTile.x))
		{// プレイヤーがブロックの右側にいるとき
			if ((pos.x - size.x) < (posTile.x + sizeTile.x))
			{
				CParticle::SetFillTile(posTile,
					D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					sizeTile,
					colEffect,
					CParticle::BULLETMOVE_RIGHT,
					D3DXVECTOR2(3, 3));

				bHitTile = true;
			}
		}
	}
	if (bHitTile == true)
	{
		// タイルを塗りつぶす
		FillTile(tileType);
	}
}

//=============================================================================
// 爆発がタイルに当たったとき
//=============================================================================
void CTile::CircleCollision(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE tileType)
{

}

//=============================================================================
// プレイヤーとのあたり判定
//=============================================================================
void CTile::CollisionPlayer(void)
{
	// 位置の取得
	D3DXVECTOR3 posTile = GetPosition();

	// サイズの取得
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// オブジェクト情報の取得
	CScene *pScene = CScene::GetScene(PRIORITY_PLAYER);

	// 敵との当たり判定
	while (pScene)
	{
		// 次の情報を取得
		CScene *pSceneNext = pScene->GetNext();

		// オブジェクトの種類の取得
		CScene::OBJTYPE objType = pScene->GetObjType();

		// オブジェクトの種類がEnemyだったら処理
		if (objType == CScene::OBJTYPE_PLAYER)
		{
			// プレイヤーインスタンス情報の取得
			CPlayer *pPlayer = (CPlayer*)pScene;

			// 位置とサイズを取得
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXVECTOR2 sizePlayer = D3DXVECTOR2(pPlayer->GetSize().x / 2.0f,pPlayer->GetSize().y / 2.0f);

			// プレイヤーとの当たり判定
			if (posPlayer.x + sizePlayer.x >= posTile.x - sizeTile.x && posPlayer.x - sizePlayer.x <= posTile.x + sizeTile.x &&
				posPlayer.y - sizePlayer.y <= posTile.y + sizeTile.y && posPlayer.y + sizePlayer.y >= posTile.y - sizeTile.y)
			{
				if (pPlayer->GetPlayerState() == CPlayer::PLAYERSTATE_NORMAL)
				{
					// 当たったことを伝える
					pPlayer->HitPlayer();
				}
				else if (pPlayer->GetPlayerState() == CPlayer::PLAYERSTATE_INVINCIBLE)
				{
					// タイルを白にする
					FillTile(TILETYPE_WHITE);
				}

				break;
			}
		}
		pScene = pSceneNext;
	}
}

//=============================================================================
// 敵とのあたり判定
//=============================================================================
void CTile::CollisionEnemy(CBullet *pBullet)
{
	// 位置の取得
	D3DXVECTOR3 posTile = GetPosition();

	// サイズの取得
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// オブジェクト情報の取得
	CScene *pScene = CScene::GetScene(PRIORITY_ENEMY);

	// 敵との当たり判定
	while (pScene)
	{
		// 次の情報を取得
		CScene *pSceneNext = pScene->GetNext();

		// オブジェクトの種類の取得
		CScene::OBJTYPE objType = pScene->GetObjType();

		// オブジェクトの種類がEnemyだったら処理
		if (objType == CScene::OBJTYPE_ENEMY)
		{
			// プレイヤーインスタンス情報の取得
			CEnemy *pEnemy = (CEnemy*)pScene;

			// 位置とサイズを取得
			D3DXVECTOR3 posEnemy = pEnemy->GetPosition();
			D3DXVECTOR2 sizeEnemy = pEnemy->GetSize();

			// プレイヤーとの当たり判定
			if (posEnemy.x + sizeEnemy.x >= posTile.x - sizeTile.x && posEnemy.x - sizeEnemy.x <= posTile.x + sizeTile.x &&
				posEnemy.y - sizeEnemy.y <= posTile.y + sizeTile.y && posEnemy.y + sizeEnemy.y >= posTile.y - sizeTile.y)
			{
				// 当たったことを伝える
				pEnemy->HitEnemy();

				// 弾も消す
				pBullet->Uninit();

				break;
			}
		}
		pScene = pSceneNext;
	}
}

//=============================================================================
// タイルの塗りつぶし処理
//=============================================================================
void CTile::FillTile(TILETYPE tileType)
{
	// タイルの種類によって処理を区別
	switch (tileType)
	{
		// 黒
	case TILETYPE_BLACK:
		// テクスチャ座標の変更
		SetTex(D3DXVECTOR2(0.5f, 1.0f), D3DXVECTOR2(2, 1));
		break;

		// 白
	case TILETYPE_WHITE:
		// テクスチャ座標の変更
		SetTex(D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(2, 1));
		break;

		// 透明
	case TILETYPE_INVISIBLE:
		// 色の変更
		SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		break;

		// 黒に変わるとき
	case TILETYPE_CHANGE_BLACK:
		// テクスチャ座標の変更
		SetTex(D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(2, 1));
		break;

		// 白に変わるとき
	case TILETYPE_CHANGE_WHITE:
		// テクスチャ座標の変更
		SetTex(D3DXVECTOR2(0.5f, 1.0f), D3DXVECTOR2(2, 1));
		break;

	}
	// タイプ更新
	m_tileType = tileType;
}