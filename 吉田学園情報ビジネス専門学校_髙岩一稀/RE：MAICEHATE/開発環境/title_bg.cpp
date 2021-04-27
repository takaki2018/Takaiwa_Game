//==============================================================================================================
//
// タイトル画面の背景 (title_bg.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "title_bg.h"
#include "player.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_TITLEBG			(3)					// タイトル背景の種類

#define MOVE_UV				(0.001f)			// タイトル背景のUV値移動量

#define TITLEBG_X			(SCREEN_WIDTH / 2)	// タイトル背景のX座標
#define TITLEBG_Y			(SCREEN_HEIGHT / 2)	// タイトル背景のY座標
#define TITLEBG_WIDTH		(SCREEN_WIDTH)		// タイトル背景の幅
#define TITLEBG_HEIGHT		(SCREEN_HEIGHT)		// タイトル背景の高さ

//--------------------------------------------------------------------------------------------------------------
// 構造体の定義
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float fWidth;			// 幅
	float fHieght;			// 高さ
}TitleBg;

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleBg = NULL;						// タイトル背景の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitleBg[MAX_TITLEBG] = {};					// タイトル背景のテクスチャのポインタ
TitleBg g_TitleBg[MAX_TITLEBG];											// タイトル背景情報

float g_fMoveUV[MAX_TITLEBG] = { MOVE_UV ,MOVE_UV * 2 ,MOVE_UV * 3 };	// タイトル背景のUV値の減少量
int g_nMoveBg[MAX_TITLEBG] = { 6000,4500,3000 };						// タイトル背景の移動量

//--------------------------------------------------------------------------------------------------------------
// タイトル背景の初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTitleBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// タイトル背景情報の初期化
	for (int nCntTitleBg = 0; nCntTitleBg < MAX_TITLEBG; nCntTitleBg++)
	{
		g_TitleBg[nCntTitleBg].pos = D3DXVECTOR3(TITLEBG_X, TITLEBG_Y, 0.0f);
		g_TitleBg[nCntTitleBg].fWidth = TITLEBG_WIDTH / 2.0f;
		g_TitleBg[nCntTitleBg].fHieght = TITLEBG_HEIGHT / 2.0f;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureTitleBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureTitleBg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureTitleBg[2]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TITLEBG,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleBg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitleBg = 0; nCntTitleBg < MAX_TITLEBG; nCntTitleBg++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_TitleBg[nCntTitleBg].pos.x - g_TitleBg[nCntTitleBg].fWidth, g_TitleBg[nCntTitleBg].pos.y + g_TitleBg[nCntTitleBg].fHieght, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TitleBg[nCntTitleBg].pos.x - g_TitleBg[nCntTitleBg].fWidth, g_TitleBg[nCntTitleBg].pos.y - g_TitleBg[nCntTitleBg].fHieght, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TitleBg[nCntTitleBg].pos.x + g_TitleBg[nCntTitleBg].fWidth, g_TitleBg[nCntTitleBg].pos.y + g_TitleBg[nCntTitleBg].fHieght, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TitleBg[nCntTitleBg].pos.x + g_TitleBg[nCntTitleBg].fWidth, g_TitleBg[nCntTitleBg].pos.y - g_TitleBg[nCntTitleBg].fHieght, 0.0f);

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

		// テクスチャ座標の設定
		SetTexUVTitleBg(nCntTitleBg);

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTitleBg->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// タイトル背景の終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitTitleBg(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTitleBg != NULL)
	{
		g_pVtxBuffTitleBg->Release();
		g_pVtxBuffTitleBg = NULL;
	}

	// テクスチャの開放
	for (int nCntBg = 0; nCntBg < MAX_TITLEBG; nCntBg++)
	{
		if (g_pTextureTitleBg[nCntBg] != NULL)
		{
			g_pTextureTitleBg[nCntBg]->Release();
			g_pTextureTitleBg[nCntBg] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// タイトル背景の更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateTitleBg(void)
{
	for (int nCntBg = 0; nCntBg < MAX_TITLEBG; nCntBg++)
	{
		// 変数を減算
		g_fMoveUV[nCntBg] -= MOVE_UV * (nCntBg + 1);

		// UVの値が1.0fを超えたとき0.0fに戻す
		if (g_fMoveUV[nCntBg] >= 1.0f)
		{
			g_fMoveUV[nCntBg] = 0.0f;
		}
		// テクスチャ座標の更新
		SetTexUVTitleBg(nCntBg);
	}
}

//--------------------------------------------------------------------------------------------------------------
// タイトル背景の描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawTitleBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntBg = 0; nCntBg < MAX_TITLEBG; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureTitleBg[nCntBg]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// 頂点座標の設定
// 引数		：nCntTitleBg	- 何番目のタイトル背景かカウントする
// 返り値	：void			- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetTexUVTitleBg(int nCntTitleBg)
{
	// TitleBg型構造体のポインタ変数
	TitleBg *pTitleBg = &g_TitleBg[0];

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCntTitleBg * 4;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + g_nMoveBg[nCntTitleBg], 1.0f + g_fMoveUV[nCntTitleBg] + g_nMoveBg[nCntTitleBg]);
	pVtx[1].tex = D3DXVECTOR2(0.0f + g_nMoveBg[nCntTitleBg], 0.0f + g_fMoveUV[nCntTitleBg] + g_nMoveBg[nCntTitleBg]);
	pVtx[2].tex = D3DXVECTOR2(1.0f + g_nMoveBg[nCntTitleBg], 1.0f + g_fMoveUV[nCntTitleBg] + g_nMoveBg[nCntTitleBg]);
	pVtx[3].tex = D3DXVECTOR2(1.0f + g_nMoveBg[nCntTitleBg], 0.0f + g_fMoveUV[nCntTitleBg] + g_nMoveBg[nCntTitleBg]);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleBg->Unlock();
}