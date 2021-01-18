//-----------------------------------------------------------------
//
// スコア背景 (score_bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "score_bg.h"
#include "selectmode.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_PLAYER			(4)						// プレイヤーの最大数

#define SCOREBG_X_SINGLE	(380.0f)				// 1番目のスコア背景の中心座標(X)シングルモード
#define SCOREBG_X_MULTI		(130.0f)				// 1番目のスコア背景の中心座標(X)マルチモード

#define SCOREBG_TWO_X		(380.0f)				// 2番目のスコア背景の中心座標(X)
#define SCOREBG_TUIRD_X		(890.0f)				// 3番目のスコア背景の中心座標(X)
#define SCOREBG_FOURTH_X	(1140.0f)				// 4番目のスコア背景の中心座標(X)
#define SCOREBG_Y			(70.0f)					// 1番目のスコア背景の中心座標(Y)

#define SCOREBG_WIDTH		(195.0f)				// スコア背景の幅
#define SCOREBG_HEIGHT		(95.0f)					// スコア背景の高さ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreBg = NULL;			// スコア背景のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureScoreBg[MAX_PLAYER] = {};		// スコア背景の頂点バッファのポインタ
D3DXVECTOR3 g_posScoreBg[MAX_PLAYER];						// スコア背景の中心座標
int g_nDisplayScoreBg;										// 表示する数を保存

//-----------------------------------------------------------------
// スコア背景の初期化処理
//-----------------------------------------------------------------
HRESULT InitScoreBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	PLAYMODE playmode = GetPlayMode();

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_1P.png", &g_pTextureScoreBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_2P.png", &g_pTextureScoreBg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_3P.png", &g_pTextureScoreBg[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_bg_4P.png", &g_pTextureScoreBg[3]);

	// 変数の初期化
	switch (playmode)
	{// シングルモードのとき処理
	case PLAYMODE_SINGLE:
		g_nDisplayScoreBg = 1;										// 表示するスコア背景の数
		g_posScoreBg[0] = D3DXVECTOR3(SCOREBG_X_SINGLE, SCOREBG_Y, 0.0f);		// 配置する座標(1P)
		break;
	// マルチモードのとき処理
	case PLAYMODE_MULTI:
		g_nDisplayScoreBg = MAX_PLAYER;								// 表示するスコア背景の数
		g_posScoreBg[0] = D3DXVECTOR3(SCOREBG_X_MULTI, SCOREBG_Y, 0.0f);		// 配置する座標(1P)
		break;
	}	
	g_posScoreBg[1] = D3DXVECTOR3(SCOREBG_TWO_X, SCOREBG_Y, 0.0f);		// 配置する座標(2P)
	g_posScoreBg[2] = D3DXVECTOR3(SCOREBG_TUIRD_X, SCOREBG_Y, 0.0f);	// 配置する座標(3P)
	g_posScoreBg[3] = D3DXVECTOR3(SCOREBG_FOURTH_X, SCOREBG_Y, 0.0f);	// 配置する座標(4P)

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * g_nDisplayScoreBg, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreBg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreBg->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nDisplayScore = 0; nDisplayScore < g_nDisplayScoreBg; nDisplayScore++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScoreBg[nDisplayScore].x - (SCOREBG_WIDTH / 2), g_posScoreBg[nDisplayScore].y + (SCOREBG_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScoreBg[nDisplayScore].x - (SCOREBG_WIDTH / 2), g_posScoreBg[nDisplayScore].y - (SCOREBG_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScoreBg[nDisplayScore].x + (SCOREBG_WIDTH / 2), g_posScoreBg[nDisplayScore].y + (SCOREBG_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScoreBg[nDisplayScore].x + (SCOREBG_WIDTH / 2), g_posScoreBg[nDisplayScore].y - (SCOREBG_HEIGHT / 2), 0.0f);

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
	g_pVtxBuffScoreBg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// スコア背景の終了処理
//-----------------------------------------------------------------
void UninitScoreBg(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffScoreBg != NULL)
	{
		g_pVtxBuffScoreBg->Release();
		g_pVtxBuffScoreBg = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < g_nDisplayScoreBg; nCnt++)
	{
		if (g_pTextureScoreBg[nCnt] != NULL)
		{
			g_pTextureScoreBg[nCnt]->Release();
			g_pTextureScoreBg[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// スコア背景の更新処理
//-----------------------------------------------------------------
void UpdateScoreBg(void)
{

}

//-----------------------------------------------------------------
// スコア背景の描画処理
//-----------------------------------------------------------------
void DrawScoreBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScoreBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	for (int nCntScore = 0; nCntScore < g_nDisplayScoreBg; nCntScore++)
	{
		pDevice->SetTexture(0, g_pTextureScoreBg[nCntScore]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}