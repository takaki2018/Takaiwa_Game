//-------------------------------------------------------------------
//
// 敵 (enemy.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "enemy.h"
#include "player.h"
#include "block.h"
#include "score.h"
#include "point.h"
#include "gamepad.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_ENEMY			(256)					// フルーツの最大値
#define MAX_ENEMY_TYPE		(2)						// フルーツの種類

#define ENEMY_SIZE_X		(30.0f)					// フルーツのサイズ(幅)
#define ENEMY_SIZE_Y		(30.0f)					// フルーツのサイズ(高さ)

#define ADD_GRAVITY			(0.1f)					// 重力
#define ADD_ROT				(0.1f)					// 回転量
#define BOUND				(1.80f)					// バウンド値

#define SETENEMY_INTERVAL	(200)					// 敵を出現させる間隔

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// 敵のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEnemy[MAX_ENEMY_TYPE] = {};	// 敵の頂点バッファのポインタ
ENEMY g_aEnemy[MAX_ENEMY];									// 敵の情報
float g_fEnemyLength;										// 敵の対角線
float g_fEnemyAngle;										// 敵の角度
int g_SetCntEnemy;											// 敵の出現カウント

//-----------------------------------------------------------------
// 敵の初期化処理
//-----------------------------------------------------------------
HRESULT InitEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].posOld = g_aEnemy[nCnt].pos;
		g_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].nCntBound = 0;
		g_aEnemy[nCnt].fWidth = ENEMY_SIZE_X;
		g_aEnemy[nCnt].fHeight = ENEMY_SIZE_Y;
		g_aEnemy[nCnt].nType = 0;
		g_aEnemy[nCnt].bUse = false;
	}

	// 変数の初期化
	g_fEnemyLength = sqrtf(ENEMY_SIZE_X * ENEMY_SIZE_X + ENEMY_SIZE_Y * ENEMY_SIZE_Y);
	g_fEnemyAngle = atan2f(ENEMY_SIZE_X, ENEMY_SIZE_Y);
	g_SetCntEnemy = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/caterpillar001.png", &g_pTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/caterpillar002.png", &g_pTextureEnemy[1]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_ENEMY, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// 頂点座標の設定
		SetVertexEnemy(nCnt);

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

		// 次の情報に更新
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 敵の終了処理
//-----------------------------------------------------------------
void UninitEnemy(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_ENEMY_TYPE; nCnt++)
	{
		if (g_pTextureEnemy[nCnt] != NULL)
		{
			g_pTextureEnemy[nCnt]->Release();
			g_pTextureEnemy[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// 敵の更新処理
//-----------------------------------------------------------------
void UpdateEnemy(void)
{
	// 構造体のポインタ変数
	ENEMY *pEnemy = &g_aEnemy[0];
	BLOCK *pBlock = GetBlock();

	g_SetCntEnemy++;
	if (0 == g_SetCntEnemy % SETENEMY_INTERVAL)
	{// 虫の配置
		SetEnemy(D3DXVECTOR3((float)(rand() % 1000 + 100), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 + 3), 0.0f, 0.0f),
			rand() % MAX_ENEMY_TYPE);
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// 位置の保存
			pEnemy->posOld = pEnemy->pos;

			// 移動量の更新
			pEnemy->move.y += ADD_GRAVITY;

			// 位置の更新
			pEnemy->pos += pEnemy->move;

			// ブロックとの当たり判定
			bool bLand = CollisionBlock(&pEnemy->pos, &pEnemy->posOld, &pEnemy->move, 
				pEnemy->fWidth * 0.9f, pEnemy->fHeight * 0.9f,COLLISIONINFO_ENEMY);

			// ブロックの上に乗ったとき
			if (bLand == true)
			{
				pEnemy->move.y -= pEnemy->move.y * BOUND;
			}

			// 回転
			if (pEnemy->posOld.x < pEnemy->pos.x)
			{// 左回転
				pEnemy->rot.z -= ADD_ROT;
				if (pEnemy->rot.z < D3DX_PI)
				{
					pEnemy->rot.z -= D3DX_PI * 2.0f;
				}
			}
			else if (pEnemy->posOld.x > pEnemy->pos.x)
			{// 右回転
				pEnemy->rot.z += ADD_ROT;
				if (pEnemy->rot.z > D3DX_PI)
				{
					pEnemy->rot.z -= D3DX_PI * 2.0f;
				}
			}
			// 敵の当たり判定
			HitEnemy(nCntEnemy);

			// 画面外に出ないようにする
			if (pEnemy->pos.y + pEnemy->fHeight > SCREEN_HEIGHT)
			{// 一番下に到達したとき処理
				pEnemy->pos.y = SCREEN_HEIGHT - pEnemy->fHeight;
				pEnemy->move.y -= pEnemy->move.y * BOUND;
			}
			if (pEnemy->pos.x + pEnemy->fWidth <= 130.0f)
			{// 一番左に到達したとき処理
				pEnemy->move.x *= -1;
			}
			if (pEnemy->pos.x - pEnemy->fWidth >= SCREEN_WIDTH - 130.0f)
			{// 一番右に到達したとき処理
				pEnemy->move.x *= -1;
			}
			// 穴に行ったとき
			if (pEnemy->pos.y - pEnemy->fHeight >= 580.0f)
			{
				if (pEnemy->pos.x + pEnemy->fWidth < 180.0f)
				{// 左側に入ったとき
					pEnemy->bUse = false;
				}
				else if (pEnemy->pos.x - pEnemy->fWidth > 1070.0f)
				{// 右側に入ったとき
					pEnemy->bUse = false;
				}
			}

			// 頂点座標の更新
			SetVertexEnemy(nCntEnemy);
		}
	}
}

//-----------------------------------------------------------------
// 敵の描画処理
//-----------------------------------------------------------------
void DrawEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// バウンドの描画
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCnt].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// 敵の当たり判定
//-----------------------------------------------------------------
void HitEnemy(int nCntEnemy)
{
	// 構造体のポインタ変数
	ENEMY *pEnemy = GetEnemy();
	PLAYER *pPlayer = GetPlayer();

	// 変数宣言
	int nPlayer = GetnCntController();

	// 何番目か
	pEnemy += nCntEnemy;

	// 敵との当たり判定
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++, pPlayer++)
	{
		if ((pPlayer->pos.y + pPlayer->fHeight) >(pEnemy->pos.y - pEnemy->fHeight * 0.7f) &&
			(pPlayer->pos.y - pPlayer->fHeight) < (pEnemy->pos.y + pEnemy->fHeight * 0.7f) &&
			(pPlayer->pos.x + pPlayer->fWidth * 0.7f) > (pEnemy->pos.x - pEnemy->fWidth * 0.7f) &&
			(pPlayer->pos.x - pPlayer->fWidth * 0.7f) < (pEnemy->pos.x + pEnemy->fWidth * 0.7f))
		{// 獲得ポイントを表示する
			SetPoint(pPlayer->pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), POINTTYPE_DECREASE_FIVE);

			// スコアの加算
			AddScore(nCntPlayer, -5);

			// サウンド
			PlaySound(SOUND_LABEL_SE_ENEMY);

			// 使用状態をfalseにする
			pEnemy->bUse = false;
		}
		
	}
}

//-----------------------------------------------------------------
// 敵の設定
//-----------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int type)
{
	// 構造体のポインタ変数
	ENEMY *pEnemy = &g_aEnemy[0];

	// 弾の設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{// 位置設定
			pEnemy->pos = pos;

			// 移動量の設定
			pEnemy->move = move;

			// タイプの確定
			pEnemy->nType = type;

			// trueで返す
			pEnemy->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// 敵の頂点座標
//-----------------------------------------------------------------
void SetVertexEnemy(int nIdx)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の情報を次にうつす
	pVtx += nIdx * 4;

	// 頂点座標の更新
	pVtx[0].pos.x = g_aEnemy[nIdx].pos.x + sinf(g_aEnemy[nIdx].rot.z + (D3DX_PI * 0.00f - g_fEnemyAngle)) * g_fEnemyLength;
	pVtx[0].pos.y = g_aEnemy[nIdx].pos.y + cosf(g_aEnemy[nIdx].rot.z + (D3DX_PI * 0.00f - g_fEnemyAngle)) * g_fEnemyLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aEnemy[nIdx].pos.x + sinf(g_aEnemy[nIdx].rot.z + (g_fEnemyAngle + D3DX_PI * 1.00f)) * g_fEnemyLength;
	pVtx[1].pos.y = g_aEnemy[nIdx].pos.y + cosf(g_aEnemy[nIdx].rot.z + (g_fEnemyAngle + D3DX_PI * 1.00f)) * g_fEnemyLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aEnemy[nIdx].pos.x + sinf(g_aEnemy[nIdx].rot.z + (g_fEnemyAngle + D3DX_PI * 0.00f)) * g_fEnemyLength;
	pVtx[2].pos.y = g_aEnemy[nIdx].pos.y + cosf(g_aEnemy[nIdx].rot.z + (g_fEnemyAngle + D3DX_PI * 0.00f)) * g_fEnemyLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aEnemy[nIdx].pos.x + sinf(g_aEnemy[nIdx].rot.z + (D3DX_PI * 1.00f - g_fEnemyAngle)) * g_fEnemyLength;
	pVtx[3].pos.y = g_aEnemy[nIdx].pos.y + cosf(g_aEnemy[nIdx].rot.z + (D3DX_PI * 1.00f - g_fEnemyAngle)) * g_fEnemyLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------
// 敵情報の取得
//-----------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}