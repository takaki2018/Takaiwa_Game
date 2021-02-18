//-----------------------------------------------------------------
//
// スコア (score.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "score.h"
#include "bear.h"
#include "selectmode.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_PLAYER		(4)						// プレイヤーの最大数
#define MAX_TEXTURE		(2)						// テクスチャの最大数
#define MAX_SCORE		(2)						// UIに表示するスコアの桁数

#define SCORE_X_SINGLE	(410.0f)				// 1番目のスコアの中心座標(X)シングルモード
#define SCORE_X_MULTI	(160.0f)				// 1番目のスコアの中心座標(X)マルチモード

#define SCORE_TWO_X		(410.0f)				// 2番目のスコアの中心座標(X)
#define SCORE_TUIRD_X	(920.0f)				// 3番目のスコアの中心座標(X)
#define SCORE_FOURTH_X	(1170.0f)				// 4番目のスコアの中心座標(X)
#define SCORE_Y			(70.0f)					// スコアの中心座標(Y)

#define SCORE_WIDTH		(40.0f)					// スコアの幅
#define SCOREHEIGHT		(75.0f)					// スコアの高さ
#define WIDTH_X			(35.0f)					// スコアを配置する間隔

#define MAX_PATTERN     (10)					// アニメーションパターンNo.の最大数
#define TEXTURE_X		(10)					// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)						// アニメーションパターンNo.の縦の最大数

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;				// スコアのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureScore[MAX_TEXTURE] = {};		// スコアの頂点バッファのポインタ
D3DXVECTOR3 g_posScore[MAX_PLAYER] ;						// スコアの中心座標
D3DXCOLOR g_colScore;										// スコアの色
bool g_bHiddenScore;										// スコアが隠れるかどうか
int g_nScore[MAX_PLAYER];									// スコアを保存する
int g_nDisplayScore;										// 表示する数を保存

//-----------------------------------------------------------------
// スコアの初期化処理
//-----------------------------------------------------------------
HRESULT InitScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	PLAYMODE playmode = GetPlayMode();

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore[0]);

	// 変数の初期化
	switch (playmode)
	{// シングルモードのとき処理
	case PLAYMODE_SINGLE:
		g_nDisplayScore = 1;											// 表示するスコアの数
		g_posScore[0] = D3DXVECTOR3(SCORE_X_SINGLE, SCORE_Y, 0.0f);		// 配置する座標(1P)
		break;
		// マルチモードのとき処理
	case PLAYMODE_MULTI:
		g_nDisplayScore = MAX_PLAYER;									// 表示するスコアの数
		g_posScore[0] = D3DXVECTOR3(SCORE_X_MULTI, SCORE_Y, 0.0f);		// 配置する座標(1P)
		break;
	}
	g_posScore[1] = D3DXVECTOR3(SCORE_TWO_X, SCORE_Y, 0.0f);			// 配置する座標(2P)
	g_posScore[2] = D3DXVECTOR3(SCORE_TUIRD_X, SCORE_Y, 0.0f);			// 配置する座標(3P)
	g_posScore[3] = D3DXVECTOR3(SCORE_FOURTH_X, SCORE_Y, 0.0f);			// 配置する座標(4P)

	g_colScore = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);						// スコアの色
	g_bHiddenScore = false;												// スコアを消えるかどうか

	for (int nCntScore = 0; nCntScore < MAX_PLAYER; nCntScore++)
	{// スコアの初期化
		g_nScore[nCntScore] = 0;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE * g_nDisplayScore, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayScore; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posScore[nDisplayScore].x - (SCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posScore[nDisplayScore].y + (SCOREHEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posScore[nDisplayScore].x - (SCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posScore[nDisplayScore].y - (SCOREHEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posScore[nDisplayScore].x + (SCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posScore[nDisplayScore].y + (SCOREHEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posScore[nDisplayScore].x + (SCORE_WIDTH / 2) + WIDTH_X * nDigitScore, g_posScore[nDisplayScore].y - (SCOREHEIGHT / 2), 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = g_colScore;
			pVtx[1].col = g_colScore;
			pVtx[2].col = g_colScore;
			pVtx[3].col = g_colScore;

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
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// スコアの終了処理
//-----------------------------------------------------------------
void UninitScore(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureScore[nCntTexture] != NULL)
		{
			g_pTextureScore[nCntTexture]->Release();
			g_pTextureScore[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// スコアの更新処理
//-----------------------------------------------------------------
void UpdateScore(void)
{
	// 変数宣言
	PLAYMODE playmode = GetPlayMode();

	// モードによって処理
	switch (playmode)
	{// マルチモードのとき処理
	case PLAYMODE_MULTI:
		HiddenScore();
		break;
	}
}

//-----------------------------------------------------------------
// スコアの描画処理
//-----------------------------------------------------------------
void DrawScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	for (int nCntScore = 0; nCntScore < MAX_SCORE * g_nDisplayScore; nCntScore++)
	{
		pDevice->SetTexture(0, g_pTextureScore[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//-----------------------------------------------------------------
// スコアを隠す処理
//-----------------------------------------------------------------
void HiddenScore(void)
{
	// 変数宣言
	float fShaking = GetShaking();

	if (fShaking != 0.0f)
	{// 画面が揺れたとき
		g_bHiddenScore = true;
	}

	if (g_bHiddenScore == true)
	{// スコアが隠れるとき
		g_colScore.a -= 0.003f;

		if (g_colScore <= 0.0f)
		{// 完全に消えたとき
			g_bHiddenScore = false;
		}
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayScore; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// 頂点カラーの更新
			pVtx[0].col = g_colScore;
			pVtx[1].col = g_colScore;
			pVtx[2].col = g_colScore;
			pVtx[3].col = g_colScore;

			// スコアの情報を次にうつす
			pVtx += 4;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//-----------------------------------------------------------------
// スコアの加算処理
//-----------------------------------------------------------------
void AddScore(int nCntPlayer,int nValue)
{
	// 変数宣言
	int aNumber[MAX_SCORE];			// スコアを格納
	g_nScore[nCntPlayer] += nValue;

	// 0より小さくなったとき0にする
	if (g_nScore[nCntPlayer] < 0)
	{
		g_nScore[nCntPlayer] = 0;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目のスコアか
	pVtx += nCntPlayer * 4 * MAX_SCORE;
	
	// スコアを格納
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{// 桁を一つずつ格納
		aNumber[nCntScore] = (g_nScore[nCntPlayer] / (int)powf(10.0f, (float)(MAX_SCORE - (nCntScore + 1))));

		// テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 0.0f);

		// スコアの情報を次にうつす
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//-----------------------------------------------------------------
// スコアの取得
//-----------------------------------------------------------------
int GetScore(int nCntScore)
{
	return g_nScore[nCntScore];
}