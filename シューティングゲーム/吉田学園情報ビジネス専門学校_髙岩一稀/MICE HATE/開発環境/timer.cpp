//-----------------------------------------------------------------
//
// タイマー (timer.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "timer.h"
#include "selectmode.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define SCORE_X			(565.0f)										// 左X座標
#define RIGHT_X			(615.0f)										// 右X座標
#define SCORE_Y			(10.0f)											// 上Y座標
#define LOW_Y			(90.0f)											// 下Y座標
#define WIDTH_X			(45.0f)											// スコアの幅

#define MAX_PATTERN     (10)											// アニメーションパターンNo.の最大数
#define TEXTURE_X		(10)											// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)												// アニメーションパターンNo.の縦の最大数

#define MAX_TIMER		(3)												// スコアテクスチャの最大値
#define TIME			(50)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;	// タイマーのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;		// タイマーの頂点バッファのポインタ
int g_nTimer;									// タイマー
int g_nTimerCnt;								// タイマーカウント
int g_aNumber[3];								// 残り時間を格納
D3DXCOLOR g_TimerCol;							// タイマーの色
int g_nSelectMode;
int g_nCntSetEnemy;

//-----------------------------------------------------------------
// タイマーの初期化処理
//-----------------------------------------------------------------
HRESULT InitTimer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();
	g_nSelectMode = GetSelectMode();
	g_nCntSetEnemy = 105;

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTimer);

	// 変数の初期化
	switch (g_nSelectMode)
	{// タイマー
	case SELECTMODE_NORMAL:
		g_nTimer = TIME;
		break;
		// タイマー
	case SELECTMODE_ENDLESS:
		g_nTimer = 0;
		break;
	}								
	g_nTimerCnt = 60;								// タイマーカウント
	g_TimerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// タイマーの色

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TIMER, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(SCORE_X + WIDTH_X * nCntTimer, LOW_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCORE_X + WIDTH_X * nCntTimer, SCORE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RIGHT_X + WIDTH_X * nCntTimer, LOW_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RIGHT_X + WIDTH_X * nCntTimer, SCORE_Y, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_TimerCol;
		pVtx[1].col = g_TimerCol;
		pVtx[2].col = g_TimerCol;
		pVtx[3].col = g_TimerCol;

		// 残り時間の計算
		g_aNumber[nCntTimer] = (g_nTimer / (int)powf(10.0f, (float)(MAX_TIMER - (nCntTimer + 1))));

		// テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTimer] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTimer] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTimer] * 0.1f, 0.0f);

		// スコアの情報を次にうつす
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// タイマーの終了処理
//-----------------------------------------------------------------
void UninitTimer(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}

	// テクスチャの開放
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}
}

//-----------------------------------------------------------------
// タイマーの更新処理
//-----------------------------------------------------------------
void UpdateTimer(void)
{
	// モードで識別
	switch (g_nSelectMode)
	{// ノーマルモード
	case SELECTMODE_NORMAL:
		// タイマーカウントの減少
		g_nTimerCnt--;

		if (g_nTimerCnt == 0 && g_nTimer != 0)
		{
			// 残り時間を減らす
			g_nTimer -= 1;

			// 初期化
			g_nTimerCnt = 60;

			if (g_nTimer == 0)
			{
				g_nTimerCnt = 10;
			}
		}
		if (g_nTimerCnt == 0 && g_nTimer == 0)
		{// タイマーを点滅させる
			g_TimerCol.a = 0.0f;
			g_nTimerCnt = 20;
		}
		else if (g_nTimerCnt == 10 && g_nTimer == 0)
		{
			g_TimerCol.a = 1.0f;
		}
		break;
		// エンドレスモード
	case SELECTMODE_ENDLESS:
		// タイマーカウントの減少
		g_nTimerCnt--;

		if (g_nTimerCnt == 0)
		{
			// 時間のカウント
			g_nTimer++;

			// 初期化
			g_nTimerCnt = 60;

			// 敵を配置する間隔を変更
			if (g_nTimer <= 300)
			{
				if (0 == g_nTimer % 50)
				{// サウンド
					PlaySound(SOUND_LABEL_SE_OK);
					g_nCntSetEnemy -= 10;
				}
			}
		}
		break;

	default:
		break;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// 残り時間を格納
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{// 残り時間の計算
		g_aNumber[nCntTimer] = (g_nTimer / (int)powf(10.0f, (float)(MAX_TIMER - (nCntTimer + 1))));

		// テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTimer] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTimer] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTimer] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTimer] * 0.1f, 0.0f);

		// 頂点カラーの更新
		pVtx[0].col = g_TimerCol;
		pVtx[1].col = g_TimerCol;
		pVtx[2].col = g_TimerCol;
		pVtx[3].col = g_TimerCol;

		// スコアの情報を次にうつす
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//-----------------------------------------------------------------
// タイマーの描画処理
//-----------------------------------------------------------------
void DrawTimer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{// スコアの表示
		pDevice->SetTexture(0, g_pTextureTimer);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimer * 4, 2);
	}
}

//-----------------------------------------------------------------
// タイマー情報の取得
//-----------------------------------------------------------------
int GetTimer(void)
{
	return g_nTimer;
}

//-----------------------------------------------------------------
// 敵を配置する間隔
//-----------------------------------------------------------------
int GetCntSetEnemy(void)
{
	return g_nCntSetEnemy;
}