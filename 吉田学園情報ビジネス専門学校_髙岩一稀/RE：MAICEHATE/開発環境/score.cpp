//==============================================================================================================
//
// スコア (score.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "Score.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define SCORE_X			(970.0f)				// 中心座標(X)
#define SCORE_Y			(50.0f)					// 中心座標(Y)

#define SCORE_WIDTH		(45.0f)					// スコアの幅
#define SCORE_HEIGHT	(80.0f)					// スコアの高さ
#define WIDTH_X			(40.0f)					// スコアを配置する間隔

#define MAX_PATTERN     (10)					// アニメーションパターンNo.の最大数
#define TEXTURE_X		(10)					// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)						// アニメーションパターンNo.の縦の最大数

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	// スコアのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;		// スコアの頂点バッファのポインタ
D3DXVECTOR3 g_posScore;							// スコアの中心座標
int g_nScore;

//--------------------------------------------------------------------------------------------------------------
// スコアの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	// 変数の初期化
	g_posScore = D3DXVECTOR3(SCORE_X, SCORE_Y, 0.0f);
	g_nScore = 0;

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE, // 確保するバッファサイズ
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
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH / 2) + WIDTH_X * nCntScore, g_posScore.y + (SCORE_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH / 2) + WIDTH_X * nCntScore, g_posScore.y - (SCORE_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x + (SCORE_WIDTH / 2) + WIDTH_X * nCntScore, g_posScore.y + (SCORE_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + (SCORE_WIDTH / 2) + WIDTH_X * nCntScore, g_posScore.y - (SCORE_HEIGHT / 2), 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// スコアの情報を次にうつす
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// スコアの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitScore(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	// テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// スコアの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateScore(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// スコアの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
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
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		pDevice->SetTexture(0, g_pTextureScore);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// スコアの加算処理
// 引数		：nValue	- 加算数
// 返り値	：void		- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void AddScore(int nValue)
{
	// 変数宣言
	int aNumber[MAX_SCORE];			// スコアを格納
	g_nScore += nValue;

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	
	// スコアを格納
	for (int nCntScore = 0; nCntScore <	MAX_SCORE; nCntScore++)
	{// 桁を一つずつ格納
		aNumber[nCntScore] = (g_nScore / (int)powf(10.0f, (float)(MAX_SCORE - (nCntScore + 1))));

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

//--------------------------------------------------------------------------------------------------------------
// スコアの取得
// 引数		：void	- 何もなし
// 返り値	：int	- スコアを返す
//--------------------------------------------------------------------------------------------------------------
int GetScore(void)
{
	return g_nScore;
}