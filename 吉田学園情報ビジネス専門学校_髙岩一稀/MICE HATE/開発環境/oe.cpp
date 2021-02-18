//-----------------------------------------------------------------
//
// 操作説明 (oe_bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "oe.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN     (4)									// 背景の種類
#define MAX_COUNT_BG	(3)

#define MOVE_BG			(0.002f)							// 背景の移動量

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffOe = NULL;				// 背景の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureOe[MAX_PATTERN] = {};			// 背景のテクスチャのポインタ

//-----------------------------------------------------------------
// 背景の初期化処理
//-----------------------------------------------------------------
HRESULT InitOe(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial_oe001.png", &g_pTextureOe[0]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffOe,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffOe->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

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

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffOe->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 背景の終了処理
//-----------------------------------------------------------------
void UninitOe(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffOe != NULL)
	{
		g_pVtxBuffOe->Release();
		g_pVtxBuffOe = NULL;
	}

	// テクスチャの開放
	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		if (g_pTextureOe[nCntBg] != NULL)
		{
			g_pTextureOe[nCntBg]->Release();
			g_pTextureOe[nCntBg] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// 背景の更新処理
//-----------------------------------------------------------------
void UpdateOe(void)
{
	
}

//-----------------------------------------------------------------
// 背景の描画処理
//-----------------------------------------------------------------
void DrawOe(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffOe, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTextureOe[0]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}