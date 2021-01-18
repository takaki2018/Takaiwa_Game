//-----------------------------------------------------------------
//
// 弾 (bullet.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bullet.h"
#include "input.h"
#include "gamepad.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "gauge.h"
#include "setparticle.h"
#include "score.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define BULLET_SIZE		(5)			// 弾の大きさ

#define MAX_BULLET		(256)		// 弾の最大値

#define MOVE_BULLET		(8.0f)		// 弾の移動量
#define BULLET_SPASE	(10.0f)		// 弾の間隔

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 弾のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			// 弾の頂点バッファのポインタ
BULLET g_aBullet[MAX_BULLET];						// 弾の情報
int g_nColorCnt;

//-----------------------------------------------------------------
// 弾の初期化処理
//-----------------------------------------------------------------
HRESULT InitBullet(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_nColorCnt = BULLETCOLOR_WHITE;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet);

	// 弾情報の初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aBullet[nCntBullet].nColType = NULL;
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].bUse = false;	
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_BULLET, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// 頂点座標の設定
		SetVertexBullet(nCntBullet);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		
		// 弾の情報を次にうつす
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 弾の終了処理
//-----------------------------------------------------------------
void UninitBullet(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	// テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
}

//-----------------------------------------------------------------
// 弾の更新処理
//-----------------------------------------------------------------
void UpdateBullet(void)
{
	// 変数宣言
	BULLET *pBullet;	// 弾情報の構造体のポインタ
	ENEMY *pEnemy;		// 敵情報の構造体のポインタ
	PLAYER *pPlayer;	// プレイヤー情報の構造体のポインタ
	bool bRemove;		// ゲージの減少量の情報

	// 変数の受け渡し
	pBullet = &g_aBullet[0];
	pEnemy = GetEnemy();
	pPlayer = GetPlayer();
	bRemove = GetRemove();

	// 弾の色の切り替え
	if (GetKeyboardTrigger(KEYINFO_COLOR_CHANGE) == true || GetJoypadTrigger(JPINFO_COLOR_CHANGE) == true)
	{
		g_nColorCnt = (g_nColorCnt + 1) % BULLETCOLOR_MAX;
	}

	// 弾の発射
	if (pPlayer->state != PLAYERSTATE_HIT)
	{// 自機の状態が敵に攻撃を受けていないとき
		if (bRemove == false)
		{// ゲージが完全に減少していないとき
			if (GetKeyboardTrigger(KEYINFO_SHOOT) == true || GetJoypadTrigger(JPINFO_SHOOT) == true)
			{// 弾の設定
			 // 効果音
				PlaySound(SOUND_LABEL_SE_SHOOT);

				// 一発目
				SetBullet(D3DXVECTOR3(pPlayer->pos.x + (BULLET_SPASE * sinf(D3DX_PI / 2 - pPlayer->rot.z)), pPlayer->pos.y - (BULLET_SPASE * cosf(D3DX_PI / 2 - pPlayer->rot.z)), 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI - pPlayer->rot.z) * -MOVE_BULLET, cosf(D3DX_PI - pPlayer->rot.z) * MOVE_BULLET, 0.0f),
					g_nColorCnt,
					60,
					BULLETTYPE_PLAYER);

				// 二発目
				SetBullet(D3DXVECTOR3(pPlayer->pos.x - (BULLET_SPASE * sinf(D3DX_PI / 2 - pPlayer->rot.z)), pPlayer->pos.y + (BULLET_SPASE * cosf(D3DX_PI / 2 - pPlayer->rot.z)), 0.0f),
					D3DXVECTOR3(sinf(D3DX_PI - pPlayer->rot.z) * -MOVE_BULLET, cosf(D3DX_PI - pPlayer->rot.z) * MOVE_BULLET, 0.0f),
					g_nColorCnt,
					60,
					BULLETTYPE_PLAYER);
			}
		}
	}
	// 弾の更新
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{// 位置更新
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.y += pBullet->move.y;

			// 弾の軌道
			if (pBullet->type == BULLETTYPE_PLAYER)
			{// 自機の弾の時
				SetBulletOrbit(pBullet->pos, pBullet->col, SETPARTICLE_PLAYER_BULLET);
			}
			else if (pBullet->type == BULLETTYPE_ENEMY)
			{// 敵の弾の時
				SetBulletOrbit(pBullet->pos, pBullet->col, SETPARTICLE_ENEMY_BULLET);
			}

			// 敵との当たり判定
			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
			{
				// 使用してるか判定
				if (pEnemy->bUse == true)
				{
					if (pBullet->type == BULLETTYPE_PLAYER)
					{
						if (pEnemy->pos.y - ENEMY_Y <= pBullet->pos.y &&
							pEnemy->pos.y + ENEMY_Y >= pBullet->pos.y &&
							pEnemy->pos.x - ENEMY_X <= pBullet->pos.x &&
							pEnemy->pos.x + ENEMY_X >= pBullet->pos.x)
						{// 敵に当たったとき
							HitEnemy(nCntEnemy, nCntBullet);

							// 弾を未使用の状態にする
							pBullet->bUse = false;
						}
					}
					else if (pBullet->type == BULLETTYPE_ENEMY)
					{// 敵の弾のとき
						if (pPlayer->state == PLAYERSTATE_NORMAL)
						{// 自機に当たったとき
							if (pPlayer->pos.y - ENEMY_Y <= pBullet->pos.y &&
								pPlayer->pos.y + ENEMY_Y >= pBullet->pos.y &&
								pPlayer->pos.x - ENEMY_X <= pBullet->pos.x &&
								pPlayer->pos.x + ENEMY_X >= pBullet->pos.x)
							{// 自機に当たったとき
								HitPlayer(nCntBullet);

								// 弾を未使用の状態にする
								pBullet->bUse = false;
							}
						}
						if (pEnemy->nType != ENEMYTYPE_BLUE)
						{// 青以外のとき
							if (pEnemy->pos.y - ENEMY_Y <= pBullet->pos.y &&
								pEnemy->pos.y + ENEMY_Y >= pBullet->pos.y &&
								pEnemy->pos.x - ENEMY_X <= pBullet->pos.x &&
								pEnemy->pos.x + ENEMY_X >= pBullet->pos.x)
							{// 敵に当たったとき
								HitEnemy(nCntEnemy, nCntBullet);

								// 弾を未使用の状態にする
								pBullet->bUse = false;
							}
						}
					}
				}
			}
			// 敵情報のリセット
			pEnemy = GetEnemy();

			// 頂点座標の設定
			SetVertexBullet(nCntBullet);

			// 画面外チェック
			if (pBullet->pos.y <= 0)
			{// 弾の状態をfalseにする
				pBullet->bUse = false;
			}
			if (pBullet->pos.y >= SCREEN_HEIGHT)
			{// 弾の状態をfalseにする
				pBullet->bUse = false;
			}
			if (pBullet->pos.x <= 0)
			{// 弾の状態をfalseにする
				pBullet->bUse = false;
			}
			if (pBullet->pos.x >= SCREEN_WIDTH)
			{// 弾の状態をfalseにする
				pBullet->bUse = false;
			}
		}
	}
}

//-----------------------------------------------------------------
// 弾の描画処理
//-----------------------------------------------------------------
void DrawBullet(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 弾の描画
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{// 弾が使われているとき処理
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// 弾の設定
//-----------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,int nBulletColor,int nLife,BULLETTYPE type)
{
	// 構造体のポインタ変数
	BULLET *pBullet;

	// 変数の受け渡し
	pBullet = &g_aBullet[0];

	// 弾の設定
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{// 位置設定
			pBullet->pos = pos;

			// 弾のタイプ
			pBullet->nColType = nBulletColor;

			// 色の設定
			if (pBullet->nColType == BULLETCOLOR_WHITE)
			{// 白に設定
				pBullet->col = D3DXCOLOR(0.9f, 0.9f, 0.9f,0.9f);
			}
			else if (pBullet->nColType == BULLETCOLOR_RED)
			{// 赤に設定
				pBullet->col = D3DXCOLOR(0.9f,0.3f,0.1f,0.9f);
			}
			else if (pBullet->nColType == BULLETCOLOR_BLUE)
			{// 青に設定
				pBullet->col = D3DXCOLOR(0.1f, 0.3f,0.9f, 0.9f);
			}
			// 頂点座標の設定
			SetVertexBullet(nCntBullet);

			// 移動量を設定
			pBullet->move = move;

			// プレイヤーと敵どちらが使っているか
			pBullet->type = type;

			// trueで返す
			pBullet->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// 弾の頂点座標
//-----------------------------------------------------------------
void SetVertexBullet(int nIdx)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の情報を次にうつす
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 0.0f);

	// 色の設定
	pVtx[0].col = g_aBullet[nIdx].col;
	pVtx[1].col = g_aBullet[nIdx].col;
	pVtx[2].col = g_aBullet[nIdx].col;
	pVtx[3].col = g_aBullet[nIdx].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//-----------------------------------------------------------------
// 弾の情報の取得
//-----------------------------------------------------------------
BULLET *GetBullet(void)
{
	return &g_aBullet[0];
}

//-----------------------------------------------------------------
// 現在の弾の色
//-----------------------------------------------------------------
int GetBulletcol(void)
{
	return g_nColorCnt;
}