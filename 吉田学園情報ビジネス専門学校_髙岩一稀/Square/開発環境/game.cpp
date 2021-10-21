//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "game.h"

#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "tile.h"
#include "effect.h"
#include "stage.h"
#include "input_keyboard.h"
#include "manager.h"
#include "fade.h"
#include "UI.h"
#include "pause.h"
#include "spawn_enemyUI.h"
#include "spawn_enemy.h"
#include "game_clear.h"
#include "UI_bg.h"
#include "life_ui.h"
#include "sound.h"
#include "model.h"
#include "tile_revival.h"
#include "tile_frame_effect.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPlayer *CGame::m_pPlayer = NULL;				// プレイヤー情報のポインタ
CUI *CGame::m_pUI = NULL;						// UI情報のポインタ
CSpawnEnemy *CGame::m_pSpawnEnemy = NULL;		// 敵発生情報のポインタ
CGameClear *CGame::m_pGameClear = NULL;			// ゲームクリア情報のポインタ
CModel *CGame::m_pModel = NULL;					// モデル情報のポインタ
int CGame::m_nCntTileFrameEffect = 0;			// タイルフレームエフェクトを出すカウンター

//=============================================================================
// CGameのコンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// CManagerのデストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// プレイヤーのテクスチャロード
	CPlayer::Load("data/TEXTURE/player.png");

	// 弾のテクスチャロード
	CBullet::Load();

	// 爆発のテクスチャロード
	CExplosion::Load();

	// 敵のテクスチャロード
	CEnemy::Load();

	// エフェクトのテクスチャロード
	CEffect::Load();

	// ポーズのテクスチャロード
	CPause::Load();

	// 敵発生UIのテクスチャロード
	CSpawnEnemyUI::Load();

	// タイル復活エフェクトのテクスチャロード
	CTileRevival::Load();

	// タイルフレームエフェクトのテクスチャロード
	CTileFrameEffect::Load();

	// 背景の生成
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
		CBg::TEXTURETYPE_GAME);

	// タイルの生成
	CStage::LoadStageData(CStage::STAGETYPE_BOX);

	// UIの初期化処理
	m_pUI = new CUI;

	if (m_pUI != NULL)
	{
		m_pUI->Init(CManager::MODE_GAME);
	}

	// 敵発生の初期化処理
	m_pSpawnEnemy = new CSpawnEnemy(CScene::OBJTYPE_ENEMY);

	if (m_pSpawnEnemy != NULL)
	{
		m_pSpawnEnemy->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	// タイル情報を取得
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, CStage::GetNumPlayerSpawn());

	// 位置情報取得
	CTile *pTile = (CTile*)pScene;

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(pTile->GetPosition(), D3DXVECTOR2(35.0f, 35.0f));
	
	// ポーズの生成
	CPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// モデルの生成
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f, -2.0f, 5.0f), D3DXVECTOR2(0.0f, 0.0f), "data/MODEL/block_black.x");

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// サウンド停止
	CManager::GetSound()->Stop();

	// プレイヤーのテクスチャアンロード
	CPlayer::Unload();

	// 弾のテクスチャアンロード
	CBullet::Unload();

	// 爆発のテクスチャアンロド
	CExplosion::Unload();

	// 敵のテクスチャアンロード
	CEnemy::Unload();

	// エフェクトのテクスチャアンロード
	CEffect::Unload();

	// ポーズのテクスチャアンロード
	CPause::Unload();

	// 敵発生UIのテクスチャアンロード
	CSpawnEnemyUI::Unload();

	// タイル復活エフェクトのテクスチャアンロード
	CTileRevival::Unload();

	// タイルフレームエフェクトのテクスチャアンロード
	CTileFrameEffect::Unload();

	// UIの終了処理
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		m_pUI = NULL;
	}

	// m_pGameClearの終了処理
	if (m_pGameClear != NULL)
	{
		m_pGameClear->Uninit();
		m_pGameClear = NULL;
	}

	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	// 敵を倒した数を取得
	int nCntRemainingEnemy = CSpawnEnemy::GetRemainingEnemy();

	// ライフの取得
	int nLife = CUI::GetLife()->GetLife();

	if (nCntRemainingEnemy == 0 || nLife == 0)
	{
		if (m_pGameClear != NULL)
		{
			// 遷移してよいか
			bool bTransition = CGameClear::GetTransition();

			// 画面遷移(Enterキーを押すか敵を全滅させたら)
			if (bTransition == true)
			{
				CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
			}
		}
		else if (m_pGameClear == NULL)
		{
			// UI背景情報の初期化
			CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 
				D3DXVECTOR2(SCREEN_WIDTH, 80.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 
				CUIBg::TEXTURETYPE_LOGO);

			if (nCntRemainingEnemy == 0)
			{
				// ゲームクリアインスタンスの生成
				m_pGameClear = CGameClear::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, 80.0f),CGameClear::GAMETRANSITION_CLEAR);
			}
			else if (nLife == 0)
			{
				// ゲームクリアインスタンスの生成
				m_pGameClear = CGameClear::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, 80.0f), CGameClear::GAMETRANSITION_OVER);
			}
		}
	}

	// タイルフレームエフェクトの生成
	TileFrameEffectCreate();
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// タイルフレームエフェクトの生成処理
//=============================================================================
void CGame::TileFrameEffectCreate(void)
{
	// カウンター更新
	m_nCntTileFrameEffect++;

	// タイルフレームエフェクトの生成
	if (m_nCntTileFrameEffect % 30 == 0)
	{
		// 端の番号の取得
		CStage::TileEdge tileEdge = CStage::GetTileEdge();
		CStage::NumTileEdge nNumTileEdge = CStage::GetNumTileEdge();
		int nNumTile;

		// タイル情報の取得
		CTile *pTile;

		D3DXVECTOR3 posTile;
		D3DXVECTOR2 sizeTile;

		int nCnt = rand() % 4;

		// 2方向で分ける
		switch (nCnt)
		{
		case 0:
			nNumTile = rand() % nNumTileEdge.nNumTileEdgeLeft;
			pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, tileEdge.nTileEdgeLeft[nNumTile]);
			// NULLチェック
			if (pTile != NULL)
			{
				// タイルが透明以外のとき生成
				if (pTile->GetTileType() != CTile::TILETYPE_INVISIBLE)
				{
					// 位置とサイズの取得
					posTile = pTile->GetPosition();
					sizeTile = pTile->GetSize();

					// タイルフレームエフェクトの生成
					CTileFrameEffect::Create(D3DXVECTOR3(posTile.x + sizeTile.x / 2.0f, posTile.y - sizeTile.y / 2.0f, posTile.z),
						D3DXVECTOR2(20.0f, 5.0f),
						D3DXVECTOR3(25.0f, 0.0f, 0.0f));
				}
			}
			break;

		case 1:
			nNumTile = rand() % nNumTileEdge.nNumTileEdgeRight;
			pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, tileEdge.nTileEdgeRight[nNumTile]);
			// NULLチェック
			if (pTile != NULL)
			{
				// タイルが透明以外のとき生成
				if (pTile->GetTileType() != CTile::TILETYPE_INVISIBLE)
				{
					// 位置とサイズの取得
					posTile = pTile->GetPosition();
					sizeTile = pTile->GetSize();

					// タイルフレームエフェクトの生成
					CTileFrameEffect::Create(D3DXVECTOR3(posTile.x - sizeTile.x / 2.0f, posTile.y - sizeTile.y / 2.0f, posTile.z),
						D3DXVECTOR2(20.0f, 5.0f),
						D3DXVECTOR3(-25.0f, 0.0f, 0.0f));
				}
			}
			break;

		case 2:
			nNumTile = rand() % nNumTileEdge.nNumTileEdgeTop;
			pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, tileEdge.nTileEdgeTop[nNumTile]);
			// NULLチェック
			if (pTile != NULL)
			{
				// タイルが透明以外のとき生成
				if (pTile->GetTileType() != CTile::TILETYPE_INVISIBLE)
				{
					// 位置とサイズの取得
					posTile = pTile->GetPosition();
					sizeTile = pTile->GetSize();

					// タイルフレームエフェクトの生成
					CTileFrameEffect::Create(D3DXVECTOR3(posTile.x - sizeTile.x / 2.0f, posTile.y + sizeTile.y / 2.0f, posTile.z),
						D3DXVECTOR2(5.0f, 20.0f),
						D3DXVECTOR3(0.0f, 25.0f, 0.0f));
				}
			}
			break;
		case 3:
			nNumTile = rand() % nNumTileEdge.nNumTileEdgeBottom;
			pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, tileEdge.nTileEdgeBottom[nNumTile]);
			// NULLチェック
			if (pTile != NULL)
			{
				// タイルが透明以外のとき生成
				if (pTile->GetTileType() != CTile::TILETYPE_INVISIBLE)
				{
					// 位置とサイズの取得
					posTile = pTile->GetPosition();
					sizeTile = pTile->GetSize();

					// タイルフレームエフェクトの生成
					CTileFrameEffect::Create(D3DXVECTOR3(posTile.x + sizeTile.x / 2.0f, posTile.y - sizeTile.y / 2.0f, posTile.z),
						D3DXVECTOR2(5.0f, 20.0f),
						D3DXVECTOR3(0.0f, -25.0f, 0.0f));
				}
			}
			break;
		}
	}
}

//=============================================================================
// プレイヤー情報の取得
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// スコア情報の取得
//=============================================================================
CUI *CGame::GetUI(void)
{
	return m_pUI;
}