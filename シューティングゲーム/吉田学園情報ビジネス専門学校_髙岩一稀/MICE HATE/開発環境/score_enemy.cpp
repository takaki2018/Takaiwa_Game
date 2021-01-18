//-----------------------------------------------------------------
//
// 敵スコア (score_enemy.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "score_enemy.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define SCORE_X			(45.0f)				// 左X座標
#define SCORE_Y			(80.0f)				// 上Y座標
#define WIDTH_X			(40.0f)				// スコアの幅

#define MAX_PATTERN     (10)				// アニメーションパターンNo.の最大数
#define TEXTURE_X		(10)				// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)					// アニメーションパターンNo.の縦の最大数

#define MAX_SCORE		(256)				// スコアテクスチャの最大値
#define MAX_NUM			(8)					// UIに表示するスコアの桁数

//-----------------------------------------------------------------
// 構造体の宣言
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 中心座標
	D3DXCOLOR col;		// 頂点カラー
	int nScore;			// スコア
	bool bUse;			// 使用判定
}SCORE;

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreEnemy = NULL;	// スコアのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureScoreEnemy = NULL;		// スコアの頂点バッファのポインタ
int g_nScoreEnemy;
SCORE g_aScoreEnemy[MAX_SCORE];						// スコア

//-----------------------------------------------------------------
// スコアの初期化処理
//-----------------------------------------------------------------
HRESULT InitScoreEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScoreEnemy);

	// 変数の初期化
	g_nScoreEnemy = 0;

	// スコア情報の初期化
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		g_aScoreEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aScoreEnemy[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aScoreEnemy[nCnt].nScore = 0;
		g_aScoreEnemy[nCnt].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		// 頂点座標の設定
		SetVertexScoreEnemy(nCntScore);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// スコアの情報を次にうつす
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScoreEnemy->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// スコアの終了処理
//-----------------------------------------------------------------
void UninitScoreEnemy(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffScoreEnemy != NULL)
	{
		g_pVtxBuffScoreEnemy->Release();
		g_pVtxBuffScoreEnemy = NULL;
	}

	// テクスチャの開放
	if (g_pTextureScoreEnemy != NULL)
	{
		g_pTextureScoreEnemy->Release();
		g_pTextureScoreEnemy = NULL;
	}
}

//-----------------------------------------------------------------
// スコアの更新処理
//-----------------------------------------------------------------
void UpdateScoreEnemy(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	SCORE *pScoreEnemy;
	pScoreEnemy = &g_aScoreEnemy[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += MAX_NUM * 4;
	// スコアの更新
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++, pScoreEnemy++)
	{
		if (pScoreEnemy->bUse == true)
		{// 頂点座標の更新
			SetVertexScoreEnemy(nCntScore);
		}
		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScoreEnemy->Unlock();
}

//-----------------------------------------------------------------
// スコアの描画処理
//-----------------------------------------------------------------
void DrawScoreEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScoreEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		if (g_aScoreEnemy[nCntScore].bUse == true)
		{// スコアの表示
			pDevice->SetTexture(0, g_pTextureScoreEnemy);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// スコアの設置
//-----------------------------------------------------------------
void SetScoreEnemy(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	SCORE *pScoreEnemy;

	// 変数の受け渡し
	pScoreEnemy = &g_aScoreEnemy[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 爆発の設定
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++, pScoreEnemy++)
	{
		if (pScoreEnemy->bUse == false)
		{
			// 位置設定
			pScoreEnemy->pos = pos;

			// 色設定
			pScoreEnemy->col = col;

			// trueで返す
			pScoreEnemy->bUse = true;

			break;
		}
		// 次の情報にうつす
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScoreEnemy->Unlock();
}

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVertexScoreEnemy(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCnt * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aScoreEnemy[nCnt].pos.x - (40.0f / 2), g_aScoreEnemy[nCnt].pos.y + (40.0f / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aScoreEnemy[nCnt].pos.x - (40.0f / 2), g_aScoreEnemy[nCnt].pos.y - (40.0f / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aScoreEnemy[nCnt].pos.x + (40.0f / 2), g_aScoreEnemy[nCnt].pos.y + (40.0f / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aScoreEnemy[nCnt].pos.x + (40.0f / 2), g_aScoreEnemy[nCnt].pos.y - (40.0f / 2), 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = g_aScoreEnemy[nCnt].col;
	pVtx[1].col = g_aScoreEnemy[nCnt].col;
	pVtx[2].col = g_aScoreEnemy[nCnt].col;
	pVtx[3].col = g_aScoreEnemy[nCnt].col;

	// 頂点バッファをアンロッ クする
	g_pVtxBuffScoreEnemy->Unlock();
}