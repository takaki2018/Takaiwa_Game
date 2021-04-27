//==============================================================================================================
//
// ランキング (ranking.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "ranking_logo.h"
#include "score.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_PATTERN				(1)					// テクスチャの数

#define RANKING_LOGO_X			(SCREEN_WIDTH / 2)	// ランキングロゴの頂点座標(X)
#define RANKING_LOGO_Y			(90.0f)				// ランキングロゴの頂点座標(Y)
#define RANKING_LOGO_WIDTH		(600.0f)			// ランキングロゴの幅
#define RANKING_LOGO_HEIGHT		(70.0f)				// ランキングロゴの高さ

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingLogo = NULL;							// ランキングの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankingLogo[MAX_PATTERN] = {};						// ランキングのテクスチャのポインタ
D3DXVECTOR3 g_posRankingLogo;												// ランキングロゴの頂点座標

//--------------------------------------------------------------------------------------------------------------
// ランキングロゴの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitRankingLogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posRankingLogo = D3DXVECTOR3(RANKING_LOGO_X, RANKING_LOGO_Y, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_logo.png", &g_pTextureRankingLogo[0]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y + RANKING_LOGO_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y - RANKING_LOGO_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y + RANKING_LOGO_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y - RANKING_LOGO_HEIGHT / 2, 0.0f);

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

	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRankingLogo->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// ランキングロゴの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitRankingLogo(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffRankingLogo != NULL)
	{
		g_pVtxBuffRankingLogo->Release();
		g_pVtxBuffRankingLogo = NULL;
	}

	// テクスチャの開放
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		if (g_pTextureRankingLogo[nCntRanking] != NULL)
		{
			g_pTextureRankingLogo[nCntRanking]->Release();
			g_pTextureRankingLogo[nCntRanking] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ランキングロゴの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateRankingLogo(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// ランキングロゴの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawRankingLogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRankingLogo[nCntRanking]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}
}