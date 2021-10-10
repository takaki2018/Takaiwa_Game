//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "player.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "sound.h"
#include "collision.h"
#include "tile.h"
#include "enemy.h"
#include "explosion.h"
#include "life.h"
#include "UI.h"
#include "stage.h"
#include "setparticle.h"
#include "model.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE		(1.6f)		// 移動量
#define INERTIA			(0.2f)		// 慣性

#define EXPANTION_COUNT	(50)		// 拡大カウント
#define SHRINK_COUNT	(100)		// 縮小カウント
#define MOVE_LENGTH		(0.5f)		// ポリゴンサイズの移動量

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;	// テクスチャ情報のポインタ

//=============================================================================
// CPlayerのコンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene2D(nPriority)
{
	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntDisp = 0;
	m_nCntState = 0;
	m_playerState = PLAYERSTATE_NORMAL;
}

//=============================================================================
// CPlayerのデストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CPlayer::Load(const char *TextureName)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, TextureName, &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CPlayer::Unload(void)
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
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);

	// NULLチェック
	if (pPlayer != NULL)
	{
		// 初期化処理
		pPlayer->Init(pos, size);

		// オブジェクトの種類設定
		pPlayer->SetObjType(OBJTYPE_PLAYER);

		// テクスチャ設定
		pPlayer->BindTexture(m_pTexture);
	}

	return pPlayer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos,size);

	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntDisp = 0;
	m_nCntState = 0;
	m_playerState = PLAYERSTATE_NORMAL;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// ライフの取得
	int nLife = CUI::GetLife()->GetLife();

	if (nLife > 0)
	{
		// 状態によって処理を分ける
		switch (m_playerState)
		{
			// 通常
		case PLAYERSTATE_NORMAL:
			StateNormal();
			break;

			// ヒット
		case PLAYERSTATE_HIT:
			StateHit();
			break;

			// 無敵
		case PLAYERSTATE_INVINCIBLE:
			StateInvincible();
			break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::Move(void)
{
	// CInputKeyboardインスタンスの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// CInputKeyboardインスタンスの取得
	CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

	// ゲームパッド情報の取得
	DIJOYSTATE2 Controller = pInputJoypad->GetControllerPless(PLAYER_1);

	// 位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = GetSize();

	// 移動処理
	if (pInputKeyboard->GetPress(pInputKeyboard->KEYINFO_UP))
	{
		// 上
		m_move.y -= PLAYER_MOVE;
	}
	if (pInputKeyboard->GetPress(pInputKeyboard->KEYINFO_DOWN))
	{
		// 下
		m_move.y += PLAYER_MOVE;
	}
	if (pInputKeyboard->GetPress(pInputKeyboard->KEYINFO_LEFT))
	{
		// 左
		m_move.x -= PLAYER_MOVE;
	}
	if (pInputKeyboard->GetPress(pInputKeyboard->KEYINFO_RIGHT))
	{
		// 右
		m_move.x += PLAYER_MOVE;
	}

	if (Controller.lX != 0 || Controller.lY != 0)
	{// スティックの入力角度
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY);

		m_move.x += PLAYER_MOVE * sinf(fAngle);			// X軸移動量の決定
		m_move.y += PLAYER_MOVE * cosf(fAngle);			// Z軸移動量の決定
	}

	// 慣性処理
	m_move.x += (0.0f - m_move.x) * INERTIA;
	m_move.y += (0.0f - m_move.y) * INERTIA;
}

//=============================================================================
// 弾発射処理
//=============================================================================
void CPlayer::Shot(void)
{
	// CInputKeyboardインスタンスの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// CInputKeyboardインスタンスの取得
	CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// 弾のサイズ
	D3DXVECTOR2 size = D3DXVECTOR2(BULLET_SIZE / 2.0f, BULLET_SIZE);

	// 変数宣言	
	bool bShopt = false;														// 弾を発射したかどうか
	D3DXVECTOR3 moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 弾の移動量
	CBullet::BULLETDIRECTION bulletdirection = CBullet::BULLETDIRECTION_UP;		// 弾を撃つ方向

	if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::JPINFO_SHOT_UP) || pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_SHOT_UP))
	{// 上方向
		moveBullet = D3DXVECTOR3(0.0f, -BULLET_MOVE, 0.0f);
		bulletdirection = CBullet::BULLETDIRECTION_UP;
		bShopt = true;
	}
	else if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::JPINFO_SHOT_DOWN) || pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_SHOT_DOWN))
	{// 下方向
		moveBullet = D3DXVECTOR3(0.0f, BULLET_MOVE, 0.0f);
		bulletdirection = CBullet::BULLETDIRECTION_DOWN;
		bShopt = true;
	}
	else if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::JPINFO_SHOT_RIGHT) || pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_SHOT_RIGHT))
	{// 右方向
		moveBullet = D3DXVECTOR3(BULLET_MOVE, 0.0f, 0.0f);
		bulletdirection = CBullet::BULLETDIRECTION_RIGHT;
		bShopt = true;
	}
	else if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::JPINFO_SHOT_LEFT) || pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_SHOT_LEFT))
	{// 左方向
		moveBullet = D3DXVECTOR3(-BULLET_MOVE, 0.0f, 0.0f);
		bulletdirection = CBullet::BULLETDIRECTION_LEFT;
		bShopt = true;
	}

	// 弾が撃たれていたら
	if (bShopt == true)
	{
		// モデルの角度移動量を加算
		//CGame::GetModel()->AddMoveRotate(D3DXVECTOR3(0.0f, 0.05f, 0.0f));

		// 弾を撃つ
		CBullet::Create(pos,moveBullet, size, CBullet::BULLETTYPE_PLEYER, bulletdirection);
		
		// 効果音
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SHOOT);
	}
}

//=============================================================================
// タイルとの当たり判定
//=============================================================================
D3DXVECTOR3 CPlayer::CollitionTile(D3DXVECTOR3 pos,D3DXVECTOR2 size)
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
				pTile->CollisionObject(&pos, m_posOld, &m_move, size);
				break;

			case CTile::TILETYPE_BLACK:
				pTile->CollisionObject(&pos, m_posOld, &m_move, size);
				break;

			default:
				break;
			}
		}
		// 次の情報へ
		pScene = pSceneNext;
	}

	return pos;
}

//=============================================================================
// 敵との当たり判定処理
//=============================================================================
bool CPlayer::CollitionEnemy(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// オブジェクト情報の取得
	CScene *pScene = GetScene(CScene::PRIORITY_ENEMY);

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
			// 敵インスタンス情報の取得
			CEnemy *pEnemy = (CEnemy*)pScene;

			// 位置の取得
			D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

			// サイズの取得
			D3DXVECTOR2 sizeEnemy = pEnemy->GetSize();

			// 矩形の当たり判定
			if (pos.x + size.x >= posEnemy.x - size.x && pos.x - size.x <= posEnemy.x + size.x &&
				pos.y - size.y <= posEnemy.y + size.y && pos.y + size.y >= posEnemy.y - size.y)
			{
				// 当たったとき処理
				if (m_playerState == PLAYERSTATE_NORMAL)
				{
					HitPlayer();
				}

				return true;
			}
		}
		// 次の情報へ
		pScene = pSceneNext;
	}
	return false;
}

//=============================================================================
// 弾との当たり判定処理
//=============================================================================
bool CPlayer::CollitionBullet(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);;

	// オブジェクト情報の取得
	CScene *pScene = GetScene(PRIORITY_BULLET);

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

			if (pBullet->GetBulletType() == CBullet::BULLETTYPE_ENEMY)
			{
				// 位置の取得
				D3DXVECTOR3 posPlayer = pBullet->GetPosition();

				// サイズの取得
				D3DXVECTOR2 sizePlayer = pBullet->GetSize();

				// 矩形の当たり判定
				if (pos.x + size.x >= posPlayer.x - size.x && pos.x - size.x <= posPlayer.x + size.x &&
					pos.y - size.y <= posPlayer.y + size.y && pos.y + size.y >= posPlayer.y - size.y)
				{
					// 弾の破棄
					pBullet->Uninit();

					if (m_playerState == PLAYERSTATE_NORMAL)
					{
						// 当たったとき処理
						HitPlayer();
					}

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
// プレイヤーがオブジェクトに当たったとき
//=============================================================================
void CPlayer::HitPlayer(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// 状態を変える
	m_playerState = PLAYERSTATE_HIT;

	// 状態カウンタと表示カウンタの初期化
	m_nCntState = 0;
	m_nCntDisp = 60;

	// 効果音
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT_PLAYER);

	// 爆発の生成
	CExplosion::Create(pos,
		D3DXVECTOR2(30.0f,30.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		35.0f,
		0.13f,
		5,
		0.0f,
		CExplosion::EXPLOSIONTYPE_PLAYER,
		CExplosion::EXPLOSION_TEXTURETYPE_NORMAL);

	// 爆発パーティクル
	CParticle::SetExplosion(pos,
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		size,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.01f,
		D3DXVECTOR2(5, 5));

	// 色の変更
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// ライフの減少
	int nLife = CUI::GetLife()->GetLife();
	CUI::GetLife()->SetLife(nLife - 1);

	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// 共通処理
//=============================================================================
void CPlayer::CommonProcess(void)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// 位置の保存
	SetPosOld(pos);

	// CScene2Dの更新処理
	CScene2D::Update();

	// 弾の発射処理
	Shot();

	// 移動処理
	Move();

	// 座標の設定
	pos += m_move;

	// タイルとの当たり判定処理
	pos = CollitionTile(pos, size);

	// 敵との当たり判定
	CollitionEnemy();

	// 弾との当たり判定
	CollitionBullet();

	// 座標の設定
	CScene2D::SetPosition(pos);
}

//=============================================================================
// 通常状態のとき
//=============================================================================
void CPlayer::StateNormal(void)
{
	// どの状態でも共通な処理
	CommonProcess();
}

//=============================================================================
// オブジェクトに当たった状態のとき
//=============================================================================
void CPlayer::StateHit(void)
{
	// 状態カウンタの更新
	m_nCntState++;

	// 状態カウンタが表示カウンタと同じ値になったら状態更新
	if (m_nCntState % m_nCntDisp == 0)
	{
		// サイズの取得
		D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

		// 状態を無敵にする
		m_playerState = PLAYERSTATE_INVINCIBLE;

		// 状態カウンタと表示カウンタを初期化
		m_nCntState = 0;
		m_nCntDisp = 120;
	}
	else if (m_nCntState % (m_nCntDisp - 1) == 0)
	{
		// 位置をステージの中央にする
		// タイル情報を取得
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, CStage::GetNumPlayerSpawn());

		// 位置情報取得
		CTile *pTile = (CTile*)pScene;

		// 位置の変更
		SetPosition(pTile->GetPosition());

		// 爆発の生成
		CExplosion::Create(pTile->GetPosition(),
			D3DXVECTOR2(100.0f, 100.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.0f),
			40.0f,
			0.1f,
			5,
			0.0f,
			CExplosion::EXPLOSIONTYPE_NONE,
			CExplosion::EXPLOSION_TEXTURETYPE_NORMAL);
	}
}

//=============================================================================
// 無敵状態のとき
//=============================================================================
void CPlayer::StateInvincible(void)
{
	// 状態カウンタの更新
	m_nCntState++;

	// わかりやすく点滅させる
	if (m_nCntState % 20 == 10)
	{
		// 半透明にする
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else if (m_nCntState % 20 == 0)
	{
		// 不透明にする
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// 位置をステージの中央にする
		// タイル情報を取得
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, CStage::GetNumPlayerSpawn());

		// 位置情報取得
		CTile *pTile = (CTile*)pScene;

		// 衝撃波の生成
		CExplosion::Create(pTile->GetPosition() ,
			D3DXVECTOR2(30.0f, 30.0f),
			D3DXCOLOR(0.8f, 0.5f, 0.2f, 0.8f),
			5.0f,
			0.0f,
			0,
			0.01f,
			CExplosion::EXPLOSIONTYPE_WAVE,
			CExplosion::EXPLOSION_TEXTURETYPE_NORMAL);
	}

	// 状態カウントと表示カウントが同じになったとき状態の変更
	if (m_nCntState % m_nCntDisp == 0)
	{
		// 状態を通常にする
		m_playerState = PLAYERSTATE_NORMAL;

		// 不透明にする
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// どの状態でも共通な処理
	CommonProcess();
}