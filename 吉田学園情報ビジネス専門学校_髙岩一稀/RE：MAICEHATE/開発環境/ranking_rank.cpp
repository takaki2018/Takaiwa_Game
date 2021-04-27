//==============================================================================================================
//
// ランキング順位 (ranking_rank.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "ranking_rank.h"
#include "ranking_score.h"
#include "setparticle.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_PATTERN				(5)					// 背景の種類

#define RANKING_RANK_X			(350.0f)			// ランキングランクの頂点座標(X)
#define RANKING_RANK_Y			(220.0f)			// ランキングランクの頂点座標(Y)
#define RANKING_RANK_WIDTH		(140.0f)			// ランキングランクの幅
#define RANKING_RANK_HEIGHT		(70.0f)				// ランキングランクの高さ
#define RANKING_RANK_SPACE		(100.0f)			// ランキングランクを配置する間隔

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingRank = NULL;				// ランキングの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankingRank[MAX_PATTERN] = {};			// ランキングのテクスチャのポインタ
D3DXVECTOR3 g_posRankingRank;										// ランキングランクの頂点座標

//--------------------------------------------------------------------------------------------------------------
// ランキング順位の初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitRankingRank(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posRankingRank = D3DXVECTOR3(RANKING_RANK_X, RANKING_RANK_Y, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_1st.png", &g_pTextureRankingRank[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_2nd.png", &g_pTextureRankingRank[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_3rd.png", &g_pTextureRankingRank[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_4th.png", &g_pTextureRankingRank[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_5th.png", &g_pTextureRankingRank[4]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingRank,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingRank->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, (g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRanking, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, (g_posRankingRank.y - RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRanking, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posRankingRank.x + RANKING_RANK_WIDTH / 2, (g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRanking, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posRankingRank.x + RANKING_RANK_WIDTH / 2, (g_posRankingRank.y - RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRanking, 0.0f);

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

		// 次の情報に移行
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRankingRank->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// ランキング順位の終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitRankingRank(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffRankingRank != NULL)
	{
		g_pVtxBuffRankingRank->Release();
		g_pVtxBuffRankingRank = NULL;
	}

	// テクスチャの開放
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		if (g_pTextureRankingRank[nCntRanking] != NULL)
		{
			g_pTextureRankingRank[nCntRanking]->Release();
			g_pTextureRankingRank[nCntRanking] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ランキング順位の更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateRankingRank(void)
{
	// 変数宣言
	int nFalshRank = GetFlashNumber();

	if (nFalshRank != -1)
	{
		SetRankingEffect(D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, 
			(g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nFalshRank, 0.0f),
			false);
	}
}

//--------------------------------------------------------------------------------------------------------------
// ランキング順位の描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawRankingRank(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingRank, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRankingRank[nCntRanking]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}

}