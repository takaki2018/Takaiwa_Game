//-----------------------------------------------------------------
//
// リザルトロゴ (count_down.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_logo.h"
#include "gauge.h"
#include "timer.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_TEXTURE			(4)				// スコアテクスチャの最大値

#define RESULT_LOGO_WIDTH	(90.0f)			// リザルトロゴの幅
#define RESULT_LOGO_HEIGHT	(55.0f)			// リザルトロゴの高さ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultLogo = NULL;			// リザルトロゴのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResultLogo[MAX_TEXTURE] = {};		// リザルトロゴの頂点バッファのポインタ
RESULTLOGO g_aResultLogo[MAX_TEXTURE];							// リザルトロゴの情報

//-----------------------------------------------------------------
// リザルトロゴの初期化処理
//-----------------------------------------------------------------
HRESULT InitResultLogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_1st.png", &g_pTextureResultLogo[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_2nd.png", &g_pTextureResultLogo[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_3rd.png", &g_pTextureResultLogo[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_4th.png", &g_pTextureResultLogo[3]);

	// 変数の初期化
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		g_aResultLogo[nCntResultLogo].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aResultLogo[nCntResultLogo].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aResultLogo[nCntResultLogo].nTexType = RANK_FOURTH;
		g_aResultLogo[nCntResultLogo].fHeight = 0.0f;
		g_aResultLogo[nCntResultLogo].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEXTURE, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		// 頂点座標の更新
		SetVErtexResultLogo(nCntResultLogo);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 何番目か
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResultLogo->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// リザルトロゴの終了処理
//-----------------------------------------------------------------
void UninitResultLogo(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffResultLogo != NULL)
	{
		g_pVtxBuffResultLogo->Release();
		g_pVtxBuffResultLogo = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTextureResultLogo[nCnt] != NULL)
		{
			g_pTextureResultLogo[nCnt]->Release();
			g_pTextureResultLogo[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// リザルトロゴの更新処理
//-----------------------------------------------------------------
void UpdateResultLogo(void)
{
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		if (g_aResultLogo[nCntResultLogo].bUse == true)
		{
			// 頂点座標の更新
			SetVErtexResultLogo(nCntResultLogo);
		}
	}
}

//-----------------------------------------------------------------
// リザルトロゴの描画処理
//-----------------------------------------------------------------
void DrawResultLogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		if (g_aResultLogo[nCntResultLogo].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureResultLogo[(int)(g_aResultLogo[nCntResultLogo].nTexType)]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultLogo * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVErtexResultLogo(int nCntResultLogo)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	RESULTLOGO *pResultLogo = &g_aResultLogo[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCntResultLogo * 4;
	pResultLogo += nCntResultLogo;

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(pResultLogo->pos.x - (RESULT_LOGO_WIDTH / 2), pResultLogo->pos.y + (RESULT_LOGO_HEIGHT / 2) - pResultLogo->fHeight - 75.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pResultLogo->pos.x - (RESULT_LOGO_WIDTH / 2), pResultLogo->pos.y - (RESULT_LOGO_HEIGHT / 2) - pResultLogo->fHeight - 75.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pResultLogo->pos.x + (RESULT_LOGO_WIDTH / 2), pResultLogo->pos.y + (RESULT_LOGO_HEIGHT / 2) - pResultLogo->fHeight - 75.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pResultLogo->pos.x + (RESULT_LOGO_WIDTH / 2), pResultLogo->pos.y - (RESULT_LOGO_HEIGHT / 2) - pResultLogo->fHeight - 75.0f, 0.0f);

	// 頂点カラーの更新
	pVtx[0].col = g_aResultLogo[nCntResultLogo].col;
	pVtx[1].col = g_aResultLogo[nCntResultLogo].col;
	pVtx[2].col = g_aResultLogo[nCntResultLogo].col;
	pVtx[3].col = g_aResultLogo[nCntResultLogo].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffResultLogo->Unlock();
}

//-----------------------------------------------------------------
// ロゴの設置
//-----------------------------------------------------------------
void SetResultLogo(D3DXVECTOR3 pos,float fHeight, RANK nTexType)
{
	// 構造体のポインタ変数
	RESULTLOGO *pResultLogo;

	// 変数の受け渡し
	pResultLogo = &g_aResultLogo[0];

	// 弾の設定
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++, pResultLogo++)
	{
		if (pResultLogo->bUse == false)
		{// 位置設定
			pResultLogo->pos = pos;

			// ゲージの高さ
			pResultLogo->fHeight = fHeight;

			// 何のテクスチャを使うか
			pResultLogo->nTexType = nTexType;

			// trueで返す
			pResultLogo->bUse = true;

			break;
		}
	}
}