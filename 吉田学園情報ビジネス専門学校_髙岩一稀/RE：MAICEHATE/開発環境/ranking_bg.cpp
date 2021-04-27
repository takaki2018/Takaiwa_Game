//==============================================================================================================
//
// ランキング背景 (ranking_bg.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "ranking_bg.h"
#include "score.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"
#include "2Deffect.h"
#include "setparticle.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_PATTERN				(1)				// 背景の種類

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingBg = NULL;					// ランキングの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankingBg[MAX_PATTERN] = {};			// ランキングのテクスチャのポインタ

//--------------------------------------------------------------------------------------------------------------
// ランキング背景の初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitRankingBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureRankingBg[0]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingBg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingBg->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

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
	g_pVtxBuffRankingBg->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// ランキング背景の終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitRankingBg(void)
{
	// サウンドの一時停止
	StopSound();

	// エフェクトの終了処理
	Uninit2DEffect();

	// 頂点バッファの開放
	if (g_pVtxBuffRankingBg != NULL)
	{
		g_pVtxBuffRankingBg->Release();
		g_pVtxBuffRankingBg = NULL;
	}

	// テクスチャの開放
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		if (g_pTextureRankingBg[nCntRanking] != NULL)
		{
			g_pTextureRankingBg[nCntRanking]->Release();
			g_pTextureRankingBg[nCntRanking] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ランキング背景の更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateRankingBg(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// ランキング背景の描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawRankingBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		// ポリゴンの描画
		pDevice->SetTexture(0, g_pTextureRankingBg[nCntRanking]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}
}