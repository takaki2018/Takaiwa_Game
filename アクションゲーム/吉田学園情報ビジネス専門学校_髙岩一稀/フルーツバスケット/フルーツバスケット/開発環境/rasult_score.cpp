//-----------------------------------------------------------------
//
// リザルトスコア (score.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_score.h"
#include "selectmode.h"
#include "bear.h"
#include "gauge.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_PLAYER			(4)						// プレイヤーの最大数
#define MAX_TEXTURE			(2)						// テクスチャの最大数
#define MAX_SCORE			(2)						// UIに表示するスコアの桁数
#define RESULTSCORE_X		(400.0f)				// 1番目のスコアの中心座標(X)
#define RESULTSCORE_Y		(500.0f)				// 1番目のスコアの中心座標(Y)

#define RESULTSCORE_WIDTH	(30.0f)					// スコアの幅
#define RESULTSCORE_HEIGHT	(60.0f)					// スコアの高さ
#define WIDTH_X				(25.0f)					// スコアを配置する間隔
#define PlAYER_WIDTH		(180.0f)				// プレイヤーを配置する間隔

#define MAX_PATTERN			(10)					// アニメーションパターンNo.の最大数
#define TEXTURE_X			(10)					// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y			(1)						// アニメーションパターンNo.の縦の最大数

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;	// スコアのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;		// スコアの頂点バッファのポインタ
D3DXVECTOR3 g_posResultScore[MAX_PLAYER];				// スコアの中心座標
int g_nResultScore[MAX_PLAYER];							// スコアを保存する
int g_nDisplayResultScore;								// 表示する数を保存

//-----------------------------------------------------------------
// リザルトスコアの初期化処理
//-----------------------------------------------------------------
HRESULT InitResultScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// シングルモードの時処理
	case PLAYMODE_SINGLE:
		g_nDisplayResultScore = 1;
		break;
	// マルチモードの時処理
	case PLAYMODE_MULTI:
		g_nDisplayResultScore = MAX_PLAYER;
		break;
	}	

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureResultScore);

	// 変数の初期化
	for (int nCntScore = 0; nCntScore < MAX_PLAYER; nCntScore++)
	{
		g_posResultScore[nCntScore] = D3DXVECTOR3(RESULTSCORE_X + PlAYER_WIDTH * nCntScore, RESULTSCORE_Y, 0.0f);
		g_nResultScore[nCntScore] = 0;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE * MAX_PLAYER, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nDisplayScore = 0; nDisplayScore < MAX_PLAYER; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x - (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y + (RESULTSCORE_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x - (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y - (RESULTSCORE_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x + (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y + (RESULTSCORE_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x + (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y - (RESULTSCORE_HEIGHT / 2), 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
			pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

			// スコアの情報を次にうつす
			pVtx += 4;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// リザルトスコアの終了処理
//-----------------------------------------------------------------
void UninitResultScore(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}

	// テクスチャの開放
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

}

//-----------------------------------------------------------------
// リザルトスコアの更新処理
//-----------------------------------------------------------------
void UpdateResultScore(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayResultScore; nDisplayScore++)
	{
		// 変数宣言
		float fmove = GetMoveGauge(nDisplayScore);

		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x - (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y + (RESULTSCORE_HEIGHT / 2) - fmove, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x - (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y - (RESULTSCORE_HEIGHT / 2) - fmove, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x + (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y + (RESULTSCORE_HEIGHT / 2) - fmove, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posResultScore[nDisplayScore].x + (RESULTSCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posResultScore[nDisplayScore].y - (RESULTSCORE_HEIGHT / 2) - fmove, 0.0f);

			// 次の情報
			pVtx += 4;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();

}

//-----------------------------------------------------------------
// リザルトスコアの描画処理
//-----------------------------------------------------------------
void DrawResultScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	for (int nCntResultScore = 0; nCntResultScore < MAX_SCORE * g_nDisplayResultScore; nCntResultScore++)
	{
		pDevice->SetTexture(0, g_pTextureResultScore);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultScore * 4, 2);
	}
}

//-----------------------------------------------------------------
// リザルトスコアの加算処理
//-----------------------------------------------------------------
void AddResultScore(int nCntPlayer, int nValue)
{
	// 変数宣言
	int aNumber[MAX_SCORE];			// スコアを格納
	g_nResultScore[nCntPlayer] += nValue;

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目のスコアか
	pVtx += nCntPlayer * 4 * MAX_SCORE;

	// スコアを格納
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{// 桁を一つずつ格納
		aNumber[nCntScore] = (g_nResultScore[nCntPlayer] / (int)powf(10.0f, (float)(MAX_SCORE - (nCntScore + 1))));

		// テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 0.0f);

		// スコアの情報を次にうつす
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}