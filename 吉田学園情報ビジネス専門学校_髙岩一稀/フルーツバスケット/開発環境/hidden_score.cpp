//-----------------------------------------------------------------
//
// はてなマーク (hidden_score.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "hidden_score.h"
#include "bear.h"
#include "selectmode.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_PLAYER			(4)					// プレイヤーの最大数
#define MAX_TEXTURE			(1)					// テクスチャの最大数
#define MAX_SCORE			(2)					// UIに表示するはてなマークの桁数

#define QUESTION_X			(160.0f)			// 1番目のはてなマークの中心座標(X)
#define QUESTION_TWO_X		(410.0f)			// 2番目のはてなマークの中心座標(X)
#define QUESTION_TUIRD_X	(920.0f)			// 3番目のはてなマークの中心座標(X)
#define QUESTION_FOURTH_X	(1170.0f)			// 4番目のはてなマークの中心座標(X)
#define QUESTION_Y			(70.0f)				// はてなマークの中心座標(Y)

#define QUESTION_WIDTH		(40.0f)				// はてなマークの幅
#define QUESTION_HEIGHT		(75.0f)				// はてなマークの高さ
#define WIDTH_X				(35.0f)				// はてなマークを配置する間隔

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHiddenScore = NULL;	// スコアのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureHiddenScore = NULL;		// スコアの頂点バッファのポインタ
D3DXVECTOR3 g_posHiddenScore[MAX_PLAYER];				// スコアの中心座標
D3DXCOLOR g_colHiddenScore;								// はてなマークの色
bool g_bAppearScore;									// スコアが現れるかどうか
int g_nDisplayHiddenScore;								// 表示する数を保存

//-----------------------------------------------------------------
// はてなマークの初期化処理
//-----------------------------------------------------------------
HRESULT InitHiddenScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();
	g_nDisplayHiddenScore = MAX_PLAYER;

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_question.png", &g_pTextureHiddenScore);

	// 変数の初期化
	g_posHiddenScore[0] = D3DXVECTOR3(QUESTION_X, QUESTION_Y, 0.0f);
	g_posHiddenScore[1] = D3DXVECTOR3(QUESTION_TWO_X, QUESTION_Y, 0.0f);
	g_posHiddenScore[2] = D3DXVECTOR3(QUESTION_TUIRD_X, QUESTION_Y, 0.0f);
	g_posHiddenScore[3] = D3DXVECTOR3(QUESTION_FOURTH_X, QUESTION_Y, 0.0f);

	g_colHiddenScore = D3DXCOLOR(1.0f, 1.0f, 1.0f,0.0f);

	g_bAppearScore = false;

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE * g_nDisplayHiddenScore, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffHiddenScore,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHiddenScore->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayHiddenScore; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posHiddenScore[nDisplayScore].x - (QUESTION_WIDTH / 2) + WIDTH_X * nDigitScore, g_posHiddenScore[nDisplayScore].y + (QUESTION_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posHiddenScore[nDisplayScore].x - (QUESTION_WIDTH / 2) + WIDTH_X * nDigitScore, g_posHiddenScore[nDisplayScore].y - (QUESTION_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posHiddenScore[nDisplayScore].x + (QUESTION_WIDTH / 2) + WIDTH_X * nDigitScore, g_posHiddenScore[nDisplayScore].y + (QUESTION_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posHiddenScore[nDisplayScore].x + (QUESTION_WIDTH / 2) + WIDTH_X * nDigitScore, g_posHiddenScore[nDisplayScore].y - (QUESTION_HEIGHT / 2), 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = g_colHiddenScore;
			pVtx[1].col = g_colHiddenScore;
			pVtx[2].col = g_colHiddenScore;
			pVtx[3].col = g_colHiddenScore;

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

			// スコアの情報を次にうつす
			pVtx += 4;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffHiddenScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// はてなマークの終了処理
//-----------------------------------------------------------------
void UninitHiddenScore(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffHiddenScore != NULL)
	{
		g_pVtxBuffHiddenScore->Release();
		g_pVtxBuffHiddenScore = NULL;
	}

	// テクスチャの開放
	if (g_pTextureHiddenScore != NULL)
	{
		g_pTextureHiddenScore->Release();
		g_pTextureHiddenScore = NULL;
	}
}

//-----------------------------------------------------------------
// はてなマークの更新処理
//-----------------------------------------------------------------
void UpdateHiddenScore(void)
{
	// 変数宣言
	PLAYMODE playmode = GetPlayMode();

	// モードによって処理
	switch (playmode)
	{// マルチモードのとき処理
	case PLAYMODE_MULTI:
		DisplayQuestionMark();
		break;
	}
}

//-----------------------------------------------------------------
// はてなマークの描画処理
//-----------------------------------------------------------------
void DrawHiddenScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHiddenScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	for (int nCntHiddenScore = 0; nCntHiddenScore < MAX_SCORE * g_nDisplayHiddenScore; nCntHiddenScore++)
	{
		pDevice->SetTexture(0, g_pTextureHiddenScore);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntHiddenScore * 4, 2);
	}
}

//-----------------------------------------------------------------
// はてなマークを表示する処理
//-----------------------------------------------------------------
void DisplayQuestionMark(void)
{
	// 変数宣言
	float fShaking = GetShaking();

	if (fShaking != 0.0f)
	{// 画面が揺れたとき
		g_bAppearScore = true;
	}

	// スコアがあらわあれるとき
	if (g_bAppearScore == true)
	{
		g_colHiddenScore.a += 0.003f;

		if (g_colHiddenScore <= 0.0f)
		{// 完全に現れたとき
			g_bAppearScore = false;
		}
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHiddenScore->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayHiddenScore; nDisplayScore++)
	{
		for (int nDigitScore = 0; nDigitScore < MAX_SCORE; nDigitScore++)
		{
			// 頂点カラーの更新
			pVtx[0].col = g_colHiddenScore;
			pVtx[1].col = g_colHiddenScore;
			pVtx[2].col = g_colHiddenScore;
			pVtx[3].col = g_colHiddenScore;

			// スコアの情報を次にうつす
			pVtx += 4;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffHiddenScore->Unlock();
}