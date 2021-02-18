//-------------------------------------------------------------------
//
// クマ (bear.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "bear.h"
#include "player.h"
#include "block.h"
#include "timer.h"
#include "score.h"
#include "point.h"
#include "title.h"
#include "input.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define BEAR_SIZE_X			(120.0f)				// クマのサイズ(幅)
#define BEAR_SIZE_Y			(85.0f)					// クマのサイズ(高さ)

#define ADD_GRAVITY			(1.0f)					// 重力
#define JAMP_COUNT			(20)					// ジャンプまでのカウント
#define TRUN_COUNT			(150)					// 向きを変えるまでのカウント
#define JUMP_BEAR			(18.0f)					// クマのジャンプ量
#define SHAKE_SCREEN		(10.0f)					// 画面揺れの量
#define SHAKE_COUNTER		(5)						// 画面揺れのカウンタ
#define SHAKE_MULTIPLAY		(0.95f)					// 画面揺れの量に掛け合わせる数

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBear = NULL;	// クマのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBear = NULL;		// クマの頂点バッファのポインタ
BEAR g_Bear;									// クマの情報
float g_fmove;								

//-----------------------------------------------------------------
// クマの初期化処理
//-----------------------------------------------------------------
HRESULT InitBear(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の初期化
	pDevice = GetDevice();
	g_fmove = 0.0f;

	// 変数の初期化
	g_Bear.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Bear.posOld = g_Bear.pos;
	g_Bear.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Bear.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Bear.fWidth = BEAR_SIZE_X / 2.0f;
	g_Bear.fHeight = BEAR_SIZE_Y / 2.0f;
	g_Bear.fTexU = 1.0f;
	g_Bear.fTexV = 1.0f;
	g_Bear.nAppTime = rand()%10 + 10;
	g_Bear.nJampCnt = 0;
	g_Bear.nTurnCnt = 0;
	g_Bear.bJump = false;
	g_Bear.bLand = false;
	g_Bear.bEvent = false;
	g_Bear.bAppear = false;
	g_Bear.bUse = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bear.png", &g_pTextureBear);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBear,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBear->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetVertexBear();

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

	// 頂点バッファをアンロックする
	g_pVtxBuffBear->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// クマの終了処理
//-----------------------------------------------------------------
void UninitBear(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBear != NULL)
	{
		g_pVtxBuffBear->Release();
		g_pVtxBuffBear = NULL;
	}

	// テクスチャの開放
	if (g_pTextureBear != NULL)
	{
		g_pTextureBear->Release();
		g_pTextureBear = NULL;
	}
}

//-----------------------------------------------------------------
// クマの更新処理
//-----------------------------------------------------------------
void UpdateBear(void)
{
	// 変数宣言
	MODE mode = GetMode();

	switch (mode)
	{// タイトル画面のとき
	case MODE_TITLE:
		TitleBear();
		break;

		// ゲーム画面のとき
	case MODE_GAME:
		GameBear();
		break;
	}
}

//-----------------------------------------------------------------
// クマの描画処理
//-----------------------------------------------------------------
void DrawBear(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBear, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// バウンドの描画
	if (g_Bear.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureBear);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-----------------------------------------------------------------
// タイトル画面のクマ
//-----------------------------------------------------------------
void TitleBear(void)
{
	// 変数宣言
	bool bSkipTitle = GetSkipTitle();

	// クマを設置
	if (g_Bear.bUse == false && g_Bear.bAppear == false)
	{
		SetBear(D3DXVECTOR3(1280.0f, 640.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f));

		// 登場したことを伝える
		g_Bear.bAppear = true;
	}

	if (g_Bear.bUse == true && g_Bear.bAppear == true && bSkipTitle == false)
	{
		// クマの動き
		ActionBear();
	}
	else if (bSkipTitle == true)
	{// 画面揺れを解除
		g_fmove = 0.0f;
	}

	// 頂点座標の更新
	SetVertexBear();
}

//-----------------------------------------------------------------
// ゲーム画面のクマ
//-----------------------------------------------------------------
void GameBear(void)
{
	// 変数宣言
	int nTimer = GetTimer();

	if (nTimer == g_Bear.nAppTime)
	{
		SetBear(D3DXVECTOR3(1280.0f, 450.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	}

	if (g_Bear.bUse == true)
	{
		// クマの動き
		ActionBear();

		// クマの当たり判定
		HitBear();

		// 頂点座標の更新
		SetVertexBear();
	}
}

//-----------------------------------------------------------------
// クマの動き
//-----------------------------------------------------------------
void ActionBear(void)
{
	// 位置を保存
	g_Bear.posOld = g_Bear.pos;

	// 位置の更新
	g_Bear.pos += g_Bear.move;

	// 重力
	g_Bear.move.y += ADD_GRAVITY;

	// ブロックとの当たり判定
	g_Bear.bLand = CollisionBlock(&g_Bear.pos, &g_Bear.posOld, &g_Bear.move, g_Bear.fWidth, g_Bear.fHeight, COLLISIONINFO_ENEMY);

	if (g_Bear.pos.x <= 1130.0f && g_Bear.bEvent == false && g_Bear.bJump == false)
	{// クマが所定の位置まで来たとき動きを止めてジャンプまでのカウントを更新
		g_Bear.nJampCnt++;
		g_Bear.move.x = 0.0f;

		if (g_Bear.nJampCnt == JAMP_COUNT)
		{// ジャンプ
			g_Bear.move.y = -JUMP_BEAR;
			g_Bear.bJump = true;
		}
	}
	else if (g_Bear.bJump == true && g_Bear.bLand == true)
	{// ブロックに乗っていてジャンプしているとき処理
		g_Bear.bJump = false;
		g_Bear.bEvent = true;

		// 画面揺れの値を設定
		g_fmove = SHAKE_SCREEN;
	}
	else if (g_Bear.bEvent == true)
	{// イベントが来たときカウントを更新
		g_Bear.nTurnCnt++;

		if (2 == g_Bear.nTurnCnt % 5)
		{
			g_fmove *= -SHAKE_MULTIPLAY;
		}
		else if (4 == g_Bear.nTurnCnt % 5)
		{
			g_fmove *= -SHAKE_MULTIPLAY;
		}

		if (g_Bear.nTurnCnt == TRUN_COUNT)
		{
			g_Bear.move.x = 1.0f;			// 移動量の更新
			g_Bear.fTexU = -1.0f;			// テクスチャ座標Uの更新
			g_Bear.bEvent = false;			// イベントを終える

											// 画面揺れの値を設定
			g_fmove = 0.0f;
		}
	}
}

//-----------------------------------------------------------------
// クマの当たり判定
//-----------------------------------------------------------------
void HitBear(void)
{
	// 構造体のポインタ変数
	PLAYER *pPlayer = GetPlayer();

	// 変数宣言
	int nPlayer = GetPlayerCnt();

	// クマとの当たり判定
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->state != PLAYERSTATE_FLASHING)
		{
			if ((pPlayer->pos.y + pPlayer->fHeight) > (g_Bear.pos.y - g_Bear.fHeight) &&
				(pPlayer->pos.y - pPlayer->fHeight) < (g_Bear.pos.y + g_Bear.fHeight) &&
				(pPlayer->pos.x + pPlayer->fWidth * 0.7f) > (g_Bear.pos.x - g_Bear.fWidth) &&
				(pPlayer->pos.x - pPlayer->fWidth * 0.7f) < (g_Bear.pos.x + g_Bear.fWidth))
			{// 変数宣言
				float fAngle = atan2f(pPlayer->pos.x - g_Bear.pos.x, pPlayer->pos.y - g_Bear.pos.y);

				// 獲得ポイントを表示する
				SetPoint(pPlayer->pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), POINTTYPE_DECREASE_ONE);

				// スコアの加算
				AddScore(nCntPlayer, -1);

				// プレイヤーの当たり判定
				HitPlayer(nCntPlayer, fAngle);
			}
		}
	}
}

//-----------------------------------------------------------------
// クマの設定
//-----------------------------------------------------------------
void SetBear(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	// 構造体のポインタ変数
	BEAR *pBear = &g_Bear;

	// 弾の設定
	if (pBear->bUse == false)
	{// 位置設定
		pBear->pos = pos;

		// 移動量の設定
		pBear->move = move;

		// trueで返す
		pBear->bUse = true;
	}
}

//-----------------------------------------------------------------
// 敵の頂点座標
//-----------------------------------------------------------------
void SetVertexBear(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBear->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_Bear.pos.x - g_Bear.fWidth, g_Bear.pos.y + g_Bear.fHeight + g_fmove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Bear.pos.x - g_Bear.fWidth, g_Bear.pos.y - g_Bear.fHeight + g_fmove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Bear.pos.x + g_Bear.fWidth, g_Bear.pos.y + g_Bear.fHeight + g_fmove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Bear.pos.x + g_Bear.fWidth, g_Bear.pos.y - g_Bear.fHeight + g_fmove, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_Bear.fTexV);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_Bear.fTexU, g_Bear.fTexV);
	pVtx[3].tex = D3DXVECTOR2(g_Bear.fTexU, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBear->Unlock();
}

//-----------------------------------------------------------------
// 画面揺れ情報の取得
//-----------------------------------------------------------------
float GetShaking(void)
{
	return g_fmove;
}