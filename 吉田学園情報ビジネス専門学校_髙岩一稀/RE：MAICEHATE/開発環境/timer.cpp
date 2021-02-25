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
#define TIMER_POS_X				(595.0f)		// マルチモードのタイマーの中心座標(X)
#define TIMER_POS_Y				(50.0f)			// マルチモードのタイマーの中心座標(Y)

#define TIMER_WIDTH				(50.0f)			// タイマーの幅
#define TIMER_HEIGHT			(80.0f)			// タイマーの高さ
#define TIMER_SPACE				(45.0f)			// タイマーの数字を配置する間隔

#define MAX_PATTERN				(10)			// アニメーションパターンNo.の最大数
#define TEXTURE_X				(10)			// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y				(1)				// アニメーションパターンNo.の縦の最大数

#define MAX_TIMER				(3)				// スコアテクスチャの最大値
#define TIME					(180)			// タイムの設定
#define TIMECOUNT_INTERBAL		(60)			// タイムカウントのインターバル

#define FLASH_INTERBAL_TIMER	(20)			// タイマーの点滅インターバル
#define CLEAR_NUMBER_TIMER		(10)			// タイマーの透明番号
#define LESS_CLEAR_NUMBER_TIMER	(0)				// タイマーの不透明番号

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;	// タイマーのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;		// タイマーの頂点バッファのポインタ
D3DXVECTOR3 g_posTimer;							// タイマーの中心座標
int g_nTimer;									// タイマー
int g_nTimerCnt;								// タイマーカウント
int g_nCntFlashTimer;							// タイマーの点滅カウンタ
int g_aNumber[MAX_TIMER];						// 残り時間を格納
D3DXCOLOR g_TimerCol;							// タイマーの色

//-----------------------------------------------------------------
// タイマーの初期化処理
//-----------------------------------------------------------------
HRESULT InitTimer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTimer);

	// 変数の初期化
	g_posTimer = D3DXVECTOR3(TIMER_POS_X, TIMER_POS_Y, 0.0f);
	g_nTimer = TIME;								
	g_nTimerCnt = TIMECOUNT_INTERBAL;				
	g_nCntFlashTimer = 0;
	g_TimerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	

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
		pVtx[0].pos = D3DXVECTOR3(g_posTimer.x - (TIMER_WIDTH / 2) + TIMER_SPACE * nCntTimer, g_posTimer.y + (TIMER_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTimer.x - (TIMER_WIDTH / 2) + TIMER_SPACE * nCntTimer, g_posTimer.y - (TIMER_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTimer.x + (TIMER_WIDTH / 2) + TIMER_SPACE * nCntTimer, g_posTimer.y + (TIMER_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTimer.x + (TIMER_WIDTH / 2) + TIMER_SPACE * nCntTimer, g_posTimer.y - (TIMER_HEIGHT / 2), 0.0f);

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
	if (g_nTimer != 0)
	{// タイマーが0より大きいとき
		// タイマーカウント処理
		CountTimer();
	}
	else if (g_nTimer == 0)
	{// タイマーが0のとき
		// タイムアップ時のタイマー点滅処理
		FlashTmer();
	}
	
	// テクスチャUVの設定
	SetTexUVTimer();

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
// タイマーのカウント処理
//-----------------------------------------------------------------
void CountTimer(void)
{
	// タイマーカウントの減少
	g_nTimerCnt--;

	if (g_nTimerCnt == 0)
	{
		// 残り時間を減らす
		g_nTimer -= 1;

		// 初期化
		g_nTimerCnt = TIMECOUNT_INTERBAL;
	}
}

//-----------------------------------------------------------------
// タイムアップ時のタイマー点滅処理
//-----------------------------------------------------------------
void FlashTmer(void)
{
	if (CLEAR_NUMBER_TIMER == g_nCntFlashTimer % FLASH_INTERBAL_TIMER)
	{// タイマーを点滅させる
		g_TimerCol.a = 0.0f;
	}
	else if (LESS_CLEAR_NUMBER_TIMER == g_nCntFlashTimer % FLASH_INTERBAL_TIMER)
	{
		g_TimerCol.a = 1.0f;
	}
	// 点滅カウンタのカウント
	g_nCntFlashTimer++;
}

//-----------------------------------------------------------------
// テクスチャUVの設定
//-----------------------------------------------------------------
void SetTexUVTimer(void)
{
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
// タイマー情報の取得
//-----------------------------------------------------------------
int GetTimer(void)
{
	return g_nTimer;
}