//-----------------------------------------------------------------
//
// 敵 (enemy.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "object.h"
#include "shockwave.h"
#include "item.h"
#include "setparticle.h"
#include "score.h"
#include "score_enemy.h"
#include "sound.h"
#include "selectmode.h"
#include "timer.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MOVE_ENEMY			(0.5f)				// 敵の移動量
#define SET_ENEMY			(g_nSetCounter)			// 敵の配置する間隔

#define MAX_TYPE_ENEMY		(3)					// 敵の種類
#define BULLET_COUNT		(200)				// 弾を撃つカウント

#define PARTICLE_ANGLE		(10)				// パーティクルの飛散角度
#define PARTICLE_MOVE		(0.1f)				// パーティクルの移動量

#define ENEMY_SCORE_WHITE	(50)				// 敵(白)のスコア
#define ENEMY_SCORE_RED		(100)				// 敵(赤)のスコア
#define ENEMY_SCORE_BLUE	(200)				// 敵(青)のスコア

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// 敵の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEnemy[MAX_TYPE_ENEMY] = {};	// 敵のテクスチャのポインタ
ENEMY g_aEnemy[MAX_ENEMY];									// 敵の情報
D3DXVECTOR3 g_posEnemy;										// 敵の頂点座標
int g_nCnt = 0;												// カウント
D3DXCOLOR g_EnemyCol;										// 敵の色
D3DXVECTOR3 g_posobject;									// オブジェクトの座標
bool g_HitObject;											// オブジェクトに当たったかどうか
bool g_bItemEffect;											// アイテムを使っているか
int g_nCntEnemy;											// 何番目か保存
int g_nCntColor;											// 点滅のときに使用
float g_NB;													// ノックバック値
int g_nCombo;												// コンボ加算値
int g_nSetCounter;											// 敵を配置するカウント
int g_nInCounter;											// 敵がサークルの中に入ってきた数

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
	g_EnemyCol = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	g_HitObject = false;
	g_nCnt = 0;
	g_nCntEnemy = NULL;
	g_nCntColor = 0;
	g_NB = 0.0f;
	g_nCombo = 10;
	g_nSetCounter = 105;
	g_nInCounter = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rat_white.png", &g_pTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rat_red.png", &g_pTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rat_blue.png", &g_pTextureEnemy[2]);

	// 敵の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = ENEMYTYPE_WHITE;
		g_aEnemy[nCntEnemy].col = g_EnemyCol;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].fAngle = 0.0f;
		g_aEnemy[nCntEnemy].nCounterState = NULL;
		g_aEnemy[nCntEnemy].nLife = 1;
		g_aEnemy[nCntEnemy].nCntFlash = 0;
		g_aEnemy[nCntEnemy].nFlashCounter = 0;
		g_aEnemy[nCntEnemy].bUse = false;
	}

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

	// 頂点座標の設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 頂点座標の設定
		SetVertexEnemy(nCntEnemy);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_EnemyCol;
		pVtx[1].col = g_EnemyCol;
		pVtx[2].col = g_EnemyCol;
		pVtx[3].col = g_EnemyCol;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 弾の情報を次にうつす
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
	for (int nCntTexture = 0; nCntTexture < MAX_TYPE_ENEMY; nCntTexture++)
	{
		if (g_pTextureEnemy[nCntTexture] != NULL)
		{
			g_pTextureEnemy[nCntTexture]->Release();
			g_pTextureEnemy[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// 敵の更新処理
//-----------------------------------------------------------------
void UpdateEnemy(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	PLAYER *pPlayer;

	// 変数の受け渡し
	pEnemy = &g_aEnemy[0];
	pPlayer = GetPlayer();
	g_posobject = GetposObject();
	g_bItemEffect = GetbUseItem();
	g_NB = ShockwaveMove();

	// 自機の状態
	if (pPlayer->state == PLAYERSTATE_HIT)
	{// 攻撃を受けたときコンボの初期化
		g_nCombo = 10;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 敵の更新
	if (g_HitObject == false)
	{// オブジェクトに当たっていないとき
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{// 敵を使用しているとき
				// 変数宣言
				// 敵とオブジェクトの距離
				float fLength = sqrtf((pEnemy->pos.x - g_posobject.x) * (pEnemy->pos.x - g_posobject.x)
					+ (pEnemy->pos.y - g_posobject.y) * (pEnemy->pos.y - g_posobject.y));

				if (g_bItemEffect == true && pEnemy->state == ENEMYSTATE_NORMAL )
				{// アイテムを使っているとき
					pEnemy->state = ENEMYSTATE_ITEMEFFECT;
					pEnemy->nCounterState = 60;
				}
				// タイプごとに更新
				switch (pEnemy->nType)
				{
				case ENEMYTYPE_WHITE:			// 白
					EnemyInfoWhite(nCntEnemy);
					break;

				case ENEMYTYPE_RED:				// 赤
					EnemyInfoRed(nCntEnemy);
					break;

				case ENEMYTYPE_BLUE:			// 青
					EnemyInfoBlue(nCntEnemy);
					break;
				}
				// 敵状態の判別
				EnemyState(nCntEnemy);

				// 敵とオブジェクト当たり判定
				if (pEnemy->pos.y - 10.0f <= g_posobject.y + 10.0f &&
					pEnemy->pos.y + 10.0f >= g_posobject.y - 10.0f &&
					pEnemy->pos.x - 10.0f <= g_posobject.x + 10.0f &&
					pEnemy->pos.x + 10.0f >= g_posobject.x - 10.0f)
				{// 当たったときtrueに変換	
					g_HitObject = true;

					// 当たった敵が何番目か保存
					g_nCntEnemy = nCntEnemy;
					break;
				}
				// 半径250.0に来たとき警告を出す
				if (fLength <= 250.0f && pEnemy->nCntFlash != 0)
				{// 警告用衝撃破を出すか
					if (pEnemy->nFlashCounter == 0)
					{
						// 警告用の衝撃波を配置
						SetShockwave(g_posobject, D3DXVECTOR3(280.0f, 280.0f, 0.0f), D3DXCOLOR(0.8f, 0.01f, 0.01f, 0.5f),
							2.0f, 0.0f, 1.0f, 0.01f);

						// カウントを減らす
						pEnemy->nCntFlash--;
						
						// カウントを戻す
						pEnemy->nFlashCounter = 30;

						// ボーナスの増加
						g_nInCounter++;

						// サウンド
						PlaySound(SOUND_LABEL_SE_ALARM);
					}
					// カウントを減らす
					pEnemy->nFlashCounter--;
				}
				else if (fLength > 250.0f && pEnemy->nCntFlash == 0)
				{// カウントを戻す
					pEnemy->nCntFlash = 2;
				}
			}
		}
		// 敵をランダムに配置
		SetEnemyRandam();
	}
	else if (g_HitObject == true)
	{
		// カウンターアップ
		g_nCntColor++;
		pEnemy += g_nCntEnemy;

		// 点滅
		if (g_nCntColor == 10)
		{
			pEnemy->col.a = 0.0f;
		}
		else if (g_nCntColor == 20)
		{
			pEnemy->col.a = 1.0f;
			g_nCntColor = 0;
		}
		// 頂点座標の設定
		SetVertexEnemy(g_nCntEnemy);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
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

	// 敵の描画
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCntEnemy].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// 敵(白)の情報
//-----------------------------------------------------------------
void EnemyInfoWhite(int nCntEnemy)
{
	// 移動量の更新
	g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x - g_aEnemy[nCntEnemy].move.x * g_NB;
	g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y - g_aEnemy[nCntEnemy].move.y * g_NB;

	// 頂点座標の設定
	SetVertexEnemy(nCntEnemy);

	// 敵の軌道
	SetMoveOrbit(g_aEnemy[nCntEnemy].pos,
		D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			cosf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			0.0f),
		D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.5f),
		SETPARTICLE_MOVE_ENEMY);
}

//-----------------------------------------------------------------
// 敵(赤)の情報
//-----------------------------------------------------------------
void EnemyInfoRed(int nCntEnemy)
{
	// 移動量の更新
	g_aEnemy[nCntEnemy].pos.x += (g_aEnemy[nCntEnemy].move.x - g_aEnemy[nCntEnemy].move.x * g_NB) * 2.0f;
	g_aEnemy[nCntEnemy].pos.y += (g_aEnemy[nCntEnemy].move.y - g_aEnemy[nCntEnemy].move.y * g_NB) * 2.0f;

	// 頂点座標の設定
	SetVertexEnemy(nCntEnemy);

	// 敵の軌道
	SetMoveOrbit(g_aEnemy[nCntEnemy].pos,
		D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			cosf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			0.0f),
		D3DXCOLOR(0.9f, 0.3f, 0.1f, 0.5f),
		SETPARTICLE_MOVE_ENEMY);
}

//-----------------------------------------------------------------
// 敵(青)の情報
//-----------------------------------------------------------------
void EnemyInfoBlue(int nCntEnemy)
{
	// 構造体のポインタ変数
	PLAYER *pPlayer;

	// 変数宣言
	float fAngleEnemy;	// 敵とプレイヤーまでの角度
	int nBulletColor = BULLETCOLOR_RED;

	// 変数の受け渡し
	pPlayer = GetPlayer();

	// 移動量の更新
	g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x - g_aEnemy[nCntEnemy].move.x * g_NB;
	g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y - g_aEnemy[nCntEnemy].move.y * g_NB;

	// 頂点座標の設定
	SetVertexEnemy(nCntEnemy);

	// 敵の軌道
	SetMoveOrbit(g_aEnemy[nCntEnemy].pos,
		D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			cosf(g_aEnemy[nCntEnemy].fAngle) * (float)((rand() % PARTICLE_ANGLE + 1) * PARTICLE_MOVE),
			0.0f),
		D3DXCOLOR(0.1f, 0.3f, 0.9f, 0.5f),
		SETPARTICLE_MOVE_ENEMY);

	// カウントの減少
	g_aEnemy[nCntEnemy].nCntBullet--;

	// 弾の発射
	if (g_aEnemy[nCntEnemy].nCntBullet == 0)
	{// 角度の確定
		fAngleEnemy = atan2f(g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x, g_aEnemy[nCntEnemy].pos.y - pPlayer->pos.y);

		// 弾の設定
		SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * 5.0f, cosf(D3DX_PI - fAngleEnemy) * 5.0f, 0.0f),
			nBulletColor, 100, BULLETTYPE_ENEMY);

		// カウントの初期化
		g_aEnemy[nCntEnemy].nCntBullet = BULLET_COUNT;
	}
}

//-----------------------------------------------------------------
// 敵の状態
//-----------------------------------------------------------------
void EnemyState(int nCntEnemy)
{
	// 敵の状態
	switch (g_aEnemy[nCntEnemy].state)
	{// ダメージを受けたとき
	case ENEMYSTATE_ANYHIT:
		// 状態カウンタの減算
		g_aEnemy[nCntEnemy].nCounterState--;

		if (g_aEnemy[nCntEnemy].nCounterState <= 0)
		{// 状態カウンタが0以下になったとき状態を通常に変更
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

			// 色を元に戻す
			g_aEnemy[nCntEnemy].col = g_EnemyCol;
		}
		break;
		// アイテムの効果を受けているとき
	case ENEMYSTATE_ITEMEFFECT:
		// 状態カウンタの減算
		g_aEnemy[nCntEnemy].nCounterState--;

		if (g_aEnemy[nCntEnemy].nCounterState <= 0)
		{// 状態カウンタが0以下になったとき状態を通常に変更
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

			// アイテムを使っていない状態にする
			SetItem(false);
		}
		break;
	}
}

//-----------------------------------------------------------------
// 敵をランダムに配置
//-----------------------------------------------------------------
void SetEnemyRandam(void)
{
	// 変数宣言
	D3DXVECTOR3 posObject;
	float fAngleEnemy;	// 敵とオブジェクトまでの角度

	// 変数の初期化
	posObject = GetposObject();

	// エンドレスモードのとき敵を配置する間隔をずらす
	if (GetSelectMode() == SELECTMODE_ENDLESS)
	{
		if (GetCntSetEnemy() < g_nSetCounter)
		{// 配置する間隔がずれたときカウントを初期化
			g_nCnt = 0;
		}
		// 敵の出る間隔
		g_nSetCounter = GetCntSetEnemy();
	}

	// カウンターアップ
	g_nCnt++;				// 敵の移動量指定

	// 敵の配置
	if (g_nCnt == SET_ENEMY)
	{
		g_posEnemy = D3DXVECTOR3((float)(rand() % SCREEN_WIDTH), (float)(-ENEMY_Y), 0.0f);
		fAngleEnemy = atan2f(g_posEnemy.x - posObject.x, g_posEnemy.y - posObject.y );
		SetEnemy(g_posEnemy, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * MOVE_ENEMY, cosf(D3DX_PI - fAngleEnemy) * MOVE_ENEMY,0.0f) ,fAngleEnemy, ENEMYTYPE_WHITE);
	}
	else if (g_nCnt == SET_ENEMY * 2)
	{
		g_posEnemy = D3DXVECTOR3((float)(ENEMY_X + SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT), 0.0f);
		fAngleEnemy = atan2f(g_posEnemy.x - posObject.x, g_posEnemy.y - posObject.y);
		SetEnemy(g_posEnemy, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * MOVE_ENEMY, cosf(D3DX_PI - fAngleEnemy) * MOVE_ENEMY, 0.0f), fAngleEnemy, (ENEMYTYPE)(rand()%3));
	}
	else if (g_nCnt == SET_ENEMY * 3)
	{
		g_posEnemy = D3DXVECTOR3((float)(rand() % SCREEN_WIDTH), (float)(ENEMY_Y + SCREEN_HEIGHT), 0.0f);
		fAngleEnemy = atan2f(g_posEnemy.x - posObject.x, g_posEnemy.y - posObject.y);
		SetEnemy(g_posEnemy, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * MOVE_ENEMY, cosf(D3DX_PI - fAngleEnemy) * MOVE_ENEMY, 0.0f), fAngleEnemy, ENEMYTYPE_WHITE);
	}
	else if (g_nCnt == SET_ENEMY * 4)
	{
		g_posEnemy = D3DXVECTOR3((float)-ENEMY_X, (float)(rand() % SCREEN_HEIGHT), 0.0f);
		fAngleEnemy = atan2f(g_posEnemy.x - posObject.x, g_posEnemy.y - posObject.y);
		SetEnemy(g_posEnemy, D3DXVECTOR3(sinf(D3DX_PI + fAngleEnemy) * MOVE_ENEMY, cosf(D3DX_PI - fAngleEnemy) * MOVE_ENEMY, 0.0f), fAngleEnemy, (ENEMYTYPE)(rand()%3));
		g_nCnt = 0;
	}
}

//-----------------------------------------------------------------
// 敵の設定
//-----------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move,float angle ,ENEMYTYPE nType)
{
	// 構造体のポインタ変数
	ENEMY *pEnemy;

	// 変数の受け渡し
	pEnemy = &g_aEnemy[0];

	// 敵の設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			// 位置設定
			pEnemy->pos = pos;

			// 移動量
			pEnemy->move = move;

			// 頂点座標の設定
			SetVertexEnemy(nCntEnemy);

			// オブジェクトまでの角度
			pEnemy->fAngle = angle;

			// タイプの設定
			pEnemy->nType = nType;

			// 弾を撃つカウントの設定
			if (nType == ENEMYTYPE_BLUE)
			{
				pEnemy->nCntBullet = BULLET_COUNT;
			}

			// 警告の回数
			pEnemy->nCntFlash = 2;

			// 警告のインターバル
			pEnemy->nFlashCounter = 0;

			// trueで返す
			pEnemy->bUse = true;

			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
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

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_X, g_aEnemy[nIdx].pos.y + ENEMY_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_X, g_aEnemy[nIdx].pos.y - ENEMY_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_X, g_aEnemy[nIdx].pos.y + ENEMY_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_X, g_aEnemy[nIdx].pos.y - ENEMY_Y, 0.0f);

	// 色の更新
	pVtx[0].col = g_aEnemy[nIdx].col;
	pVtx[1].col = g_aEnemy[nIdx].col;
	pVtx[2].col = g_aEnemy[nIdx].col;
	pVtx[3].col = g_aEnemy[nIdx].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------
// 敵に当たった時
//-----------------------------------------------------------------
bool HitEnemy(int nIdx, int nCntBullet)
{
	// 構造体のポインタ変数
	ENEMY *pEnemy;
	BULLET *pBullet;

	// 変数の受け渡し
	pEnemy = &g_aEnemy[0];
	pBullet = GetBullet();

	// 何番目の情報か
	pEnemy += nIdx;
	pBullet += nCntBullet;

	// タイプの識別
	if (pEnemy->nType == pBullet->nColType && pBullet->type == BULLETTYPE_PLAYER)
	{// 敵の色の判別
		if (pEnemy->nType == ENEMYTYPE_WHITE)
		{// 白
			// スコア
			AddScore(ENEMY_SCORE_WHITE * g_nCombo);
		}
		else if (pEnemy->nType == ENEMYTYPE_RED)
		{// 赤
			// スコア
			AddScore(ENEMY_SCORE_RED * g_nCombo);
		}
		else if (pEnemy->nType == ENEMYTYPE_BLUE)
		{// 青
			// スコア
			AddScore(ENEMY_SCORE_BLUE * g_nCombo);
		}
		// 敵を未使用にする
		pEnemy->bUse = false;

		// 爆発パーティクル
		SetBom(pEnemy->pos,SETPARTICLE_HIT_ENEMY);

		// 効果音(爆発)
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		// コンボ加算
		g_nCombo += 1;

		// trueで返す
		return true;
	}
	else
	{// 敵に違う弾が当たったとき状態を変更　
		pEnemy->state = ENEMYSTATE_ANYHIT;

		// 状態カウンタを5に設定
		pEnemy->nCounterState = 5;

		// 色の変更
		pEnemy->col = D3DCOLOR_RGBA(100, 100, 100, 255);

		// 爆発パーティクル
		SetBom(pEnemy->pos, SETPARTICLE_ANYHIT_ENEMY);

		if (pBullet->type == BULLETTYPE_PLAYER)
		{// コンボ値の初期化
			g_nCombo = 10;
		}
		// falseで返す
		return false;
	}
}

//-----------------------------------------------------------------
// 敵情報の取得
//-----------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//-----------------------------------------------------------------
// オブジェクトの当たり判定の取得
//-----------------------------------------------------------------
bool GetHitObject(void)
{
	return g_HitObject;
}

//-----------------------------------------------------------------
// 範囲内に来た数
//-----------------------------------------------------------------
int GetEnemyIn(void)
{
	return g_nInCounter;
}