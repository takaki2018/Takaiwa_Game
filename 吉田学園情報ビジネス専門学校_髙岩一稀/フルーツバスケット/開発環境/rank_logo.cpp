//-----------------------------------------------------------------
//
// ランクロゴ (rank_logo.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "rank_logo.h"
#include "gauge.h"
#include "timer.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_TEXTURE			(5)				// ランクロゴの最大値

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankLogo = NULL;			// ランクロゴのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankLogo[MAX_TEXTURE] = {};	// ランクロゴの頂点バッファのポインタ
RANKLOGO g_aRankLogo[RANK_MAX];								// ランクロゴの情報

//-----------------------------------------------------------------
// リザルトロゴの初期化処理
//-----------------------------------------------------------------
HRESULT InitRankLogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_1st.png", &g_pTextureRankLogo[RANK_1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_2nd.png", &g_pTextureRankLogo[RANK_2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_3rd.png", &g_pTextureRankLogo[RANK_3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_4th.png", &g_pTextureRankLogo[RANK_4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_5th.png", &g_pTextureRankLogo[RANK_5]);

	// 変数の初期化
	for (int nCntRankLogo = 0; nCntRankLogo < MAX_TEXTURE; nCntRankLogo++)
	{
		g_aRankLogo[nCntRankLogo].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRankLogo[nCntRankLogo].nTexType = RANK_1;
		g_aRankLogo[nCntRankLogo].fWidth = 0.0f;
		g_aRankLogo[nCntRankLogo].fHeight = 0.0f;
		g_aRankLogo[nCntRankLogo].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEXTURE,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRankLogo = 0; nCntRankLogo < MAX_TEXTURE; nCntRankLogo++)
	{
		// 頂点座標の更新
		SetVErtexRankLogo(nCntRankLogo);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの更新
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 何番目か
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffRankLogo->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// リザルトロゴの終了処理
//-----------------------------------------------------------------
void UninitRankLogo(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffRankLogo != NULL)
	{
		g_pVtxBuffRankLogo->Release();
		g_pVtxBuffRankLogo = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTextureRankLogo[nCnt] != NULL)
		{
			g_pTextureRankLogo[nCnt]->Release();
			g_pTextureRankLogo[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// リザルトロゴの更新処理
//-----------------------------------------------------------------
void UpdateRankLogo(void)
{
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		if (g_aRankLogo[nCntResultLogo].bUse == true)
		{
			// 頂点座標の更新
			SetVErtexRankLogo(nCntResultLogo);
		}
	}
}

//-----------------------------------------------------------------
// リザルトロゴの描画処理
//-----------------------------------------------------------------
void DrawRankLogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		if (g_aRankLogo[nCntResultLogo].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureRankLogo[(int)(g_aRankLogo[nCntResultLogo].nTexType)]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultLogo * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVErtexRankLogo(int nCntRankLogo)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCntRankLogo * 4;

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_aRankLogo[nCntRankLogo].pos.x - (g_aRankLogo[nCntRankLogo].fWidth / 2), g_aRankLogo[nCntRankLogo].pos.y + (g_aRankLogo[nCntRankLogo].fHeight / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aRankLogo[nCntRankLogo].pos.x - (g_aRankLogo[nCntRankLogo].fWidth / 2), g_aRankLogo[nCntRankLogo].pos.y - (g_aRankLogo[nCntRankLogo].fHeight / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aRankLogo[nCntRankLogo].pos.x + (g_aRankLogo[nCntRankLogo].fWidth / 2), g_aRankLogo[nCntRankLogo].pos.y + (g_aRankLogo[nCntRankLogo].fHeight / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aRankLogo[nCntRankLogo].pos.x + (g_aRankLogo[nCntRankLogo].fWidth / 2), g_aRankLogo[nCntRankLogo].pos.y - (g_aRankLogo[nCntRankLogo].fHeight / 2), 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffRankLogo->Unlock();
}

//-----------------------------------------------------------------
// ロゴの設置
//-----------------------------------------------------------------
void SetRankLogo(D3DXVECTOR3 pos, float fWidth,float fHeight, RANK nTexType)
{
	// 構造体のポインタ変数
	RANKLOGO *pRankLogo;

	// 変数の受け渡し
	pRankLogo = &g_aRankLogo[0];

	// 弾の設定
	for (int nCntRankLogo = 0; nCntRankLogo < MAX_TEXTURE; nCntRankLogo++, pRankLogo++)
	{
		if (pRankLogo->bUse == false)
		{// 位置設定
			pRankLogo->pos = pos;

			// 幅
			pRankLogo->fWidth = fWidth;

			// 高さ
			pRankLogo->fHeight = fHeight;

			// 何のテクスチャを使うか
			pRankLogo->nTexType = nTexType;

			// trueで返す
			pRankLogo->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// ランクロゴの変更
//-----------------------------------------------------------------
void CangeRankLogo(int nCntRankLogo, RANK nTexType)
{
	g_aRankLogo[nCntRankLogo].nTexType = nTexType;
}