//-----------------------------------------------------------------
//
// UI背景 (ui.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "uibg.h"

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUibg = NULL;		// UI背景の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureUibg = NULL;			// UI背景のテクスチャのポインタ

//-----------------------------------------------------------------
// UI背景の初期化処理
//-----------------------------------------------------------------
HRESULT InitUibg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI.png", &g_pTextureUibg);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffUibg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUibg->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f,100.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH,100.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffUibg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// UI背景の終了処理
//-----------------------------------------------------------------
void UninitUibg(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffUibg != NULL)
	{
		g_pVtxBuffUibg->Release();
		g_pVtxBuffUibg = NULL;
	}

	// テクスチャの開放
	if (g_pTextureUibg != NULL)
	{
		g_pTextureUibg->Release();
		g_pTextureUibg = NULL;
	}
}


//-----------------------------------------------------------------
// UI背景の描画処理
//-----------------------------------------------------------------
void DrawUibg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUibg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTextureUibg);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}