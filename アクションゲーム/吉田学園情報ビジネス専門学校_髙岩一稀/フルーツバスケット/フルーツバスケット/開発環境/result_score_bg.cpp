//-----------------------------------------------------------------
//
// リザルトスコア背景 (result_score_bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_score_bg.h"
#include "selectmode.h"
#include "gauge.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_PLAYER				(4)						// プレイヤーの最大数
#define RESULTSCOREBG_X			(370.0f)				// 1番目のスコアの中心座標(X)
#define RESULTSCOREBG_Y			(500.0f)					// 1番目のスコアの中心座標(Y)

#define RESULTSCOREBG_WIDTH		(150.0f)				// スコアの幅
#define RESULTSCOREBG_HEIGHT	(75.0f)					// スコアの高さ
#define WIDTH_X					(40.0f)					// スコアを配置する間隔
#define PlAYER_WIDTH			(180.0f)				// プレイヤーを配置する間隔

#define MAX_PATTERN				(1)						// アニメーションパターンNo.の最大数
#define TEXTURE_X				(1)						// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y				(1)						// アニメーションパターンNo.の縦の最大数

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScoreBg = NULL;			// リザルトスコア背景のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResultScoreBg[MAX_PLAYER] = {};	// リザルトスコア背景の頂点バッファのポインタ
D3DXVECTOR3 g_posResultScoreBg[MAX_PLAYER];						// リザルトスコア背景の中心座標
int g_nDisplayResultScoreBg;									// 表示する数を保存

//-----------------------------------------------------------------
// リザルトスコア背景の初期化処理
//-----------------------------------------------------------------
HRESULT InitResultScoreBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// シングルモードの時処理
	case PLAYMODE_SINGLE:
		g_nDisplayResultScoreBg = 1;
		break;
		// マルチモードの時処理
	case PLAYMODE_MULTI:
		g_nDisplayResultScoreBg = MAX_PLAYER;
		break;
	}

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_1P.png", &g_pTextureResultScoreBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_2P.png", &g_pTextureResultScoreBg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_3P.png", &g_pTextureResultScoreBg[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_4P.png", &g_pTextureResultScoreBg[3]);

	// 変数の初期化
	for (int nCntResultScoreBg = 0; nCntResultScoreBg < MAX_PLAYER; nCntResultScoreBg++)
	{
		g_posResultScoreBg[nCntResultScoreBg] = D3DXVECTOR3(RESULTSCOREBG_X + PlAYER_WIDTH * nCntResultScoreBg, RESULTSCOREBG_Y, 0.0f);
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PLAYER, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScoreBg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScoreBg->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nDisplayScore = 0; nDisplayScore < MAX_PLAYER; nDisplayScore++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x - (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y + (RESULTSCOREBG_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x - (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y - (RESULTSCOREBG_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x + (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y + (RESULTSCOREBG_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x + (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y - (RESULTSCOREBG_HEIGHT / 2), 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// スコアの情報を次にうつす
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultScoreBg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// リザルトスコア背景の終了処理
//-----------------------------------------------------------------
void UninitResultScoreBg(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffResultScoreBg != NULL)
	{
		g_pVtxBuffResultScoreBg->Release();
		g_pVtxBuffResultScoreBg = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < g_nDisplayResultScoreBg; nCnt++)
	{
		if (g_pTextureResultScoreBg[nCnt] != NULL)
		{
			g_pTextureResultScoreBg[nCnt]->Release();
			g_pTextureResultScoreBg[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// リザルトスコア背景の更新処理
//-----------------------------------------------------------------
void UpdateResultScoreBg(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScoreBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDisplayScore = 0;nDisplayScore < g_nDisplayResultScoreBg;nDisplayScore++)
	{
		// 変数宣言
		float fmove = GetMoveGauge(nDisplayScore);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x - (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y + (RESULTSCOREBG_HEIGHT / 2) - fmove, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x - (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y - (RESULTSCOREBG_HEIGHT / 2) - fmove, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x + (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y + (RESULTSCOREBG_HEIGHT / 2) - fmove, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResultScoreBg[nDisplayScore].x + (RESULTSCOREBG_WIDTH / 2), g_posResultScoreBg[nDisplayScore].y - (RESULTSCOREBG_HEIGHT / 2) - fmove, 0.0f);
	
		// 次の情報
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResultScoreBg->Unlock();
}

//-----------------------------------------------------------------
// リザルトスコア背景の描画処理
//-----------------------------------------------------------------
void DrawResultScoreBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultScoreBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	for (int nCntScore = 0; nCntScore < g_nDisplayResultScoreBg; nCntScore++)
	{
		pDevice->SetTexture(0, g_pTextureResultScoreBg[nCntScore]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}