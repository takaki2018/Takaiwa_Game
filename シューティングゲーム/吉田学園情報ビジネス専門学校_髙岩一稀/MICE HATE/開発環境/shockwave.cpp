//-----------------------------------------------------------------
//
// 衝撃波 (shockwave.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "shockwave.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_SHOCKWAVE	(256)						// 衝撃波の最大値

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	D3DXVECTOR3 size;		// サイズ
	float fSize;			// サイズの倍率
	float fNB;				// ノックバック値
	float fNB_MAG;			// ノックバック倍率
	float nCntAlpha;		// α値の移動
	int nCntState;			// 状態カウンタ
	bool bUse;				// 使用しているかどうか
} SHOCKWAVE;

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShockwave = NULL;	// 衝撃波のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShockwave = NULL;		// 衝撃波の頂点バッファのポインタ
SHOCKWAVE g_aShockwave[MAX_SHOCKWAVE];				// 衝撃波の情報
float g_fKnockBack;									// ノックバック値

//-----------------------------------------------------------------
// 衝撃波の初期化処理
//-----------------------------------------------------------------
HRESULT InitShockwave(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &g_pTextureShockwave);

	g_fKnockBack = 0.0f;

	// 爆発の情報の初期化
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++)
	{
		g_aShockwave[nCntShockwave].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShockwave[nCntShockwave].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aShockwave[nCntShockwave].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;
		g_aShockwave[nCntShockwave].fSize = NULL;
		g_aShockwave[nCntShockwave].fNB = NULL;
		g_aShockwave[nCntShockwave].fNB_MAG = NULL;
		g_aShockwave[nCntShockwave].nCntAlpha = NULL;
		g_aShockwave[nCntShockwave].nCntState = NULL;
		g_aShockwave[nCntShockwave].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SHOCKWAVE, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffShockwave,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShockwave->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++)
	{
		// 頂点座標の設定
		SetVertexShockwave(nCntShockwave);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 弾の情報を次にうつす
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffShockwave->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 衝撃波の終了処理
//-----------------------------------------------------------------
void UninitShockwave(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffShockwave != NULL)
	{
		g_pVtxBuffShockwave->Release();
		g_pVtxBuffShockwave = NULL;
	}

	// テクスチャの開放
	if (g_pTextureShockwave != NULL)
	{
		g_pTextureShockwave->Release();
		g_pTextureShockwave = NULL;
	}
}

//-----------------------------------------------------------------
// 衝撃波の更新処理
//-----------------------------------------------------------------
void UpdateShockwave(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	SHOCKWAVE *pShockwave;
	pShockwave = &g_aShockwave[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShockwave->Lock(0, 0, (void**)&pVtx, 0);

	// 爆発の更新
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++, pShockwave++)
	{
		if (pShockwave->bUse == true)
		{
			// サイズの確定
			pShockwave->size.x += 1.0f * pShockwave->fSize;
			pShockwave->size.y += 1.0f * pShockwave->fSize;

			// ノックバック値
			pShockwave->fNB *= pShockwave->fNB_MAG;

			if (pShockwave->fNB > 0.0f)
			{
				g_fKnockBack = pShockwave->fNB;
			}

			// 頂点座標の設定
			SetVertexShockwave(nCntShockwave);

			// α値の加算
			pShockwave->col.a -= pShockwave->nCntAlpha;

			// 衝撃波の削除
			if (pShockwave->col.a <= 0.0f)
			{
				pShockwave->bUse = false;
				pShockwave->fNB = 0.0f;
				pShockwave->size = D3DXVECTOR3(0.0f,0.0f,0.0f);
				pShockwave->col.a = 1.0f;
			}
		}
		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffShockwave->Unlock();
}

//-----------------------------------------------------------------
// 衝撃波の描画処理
//-----------------------------------------------------------------
void DrawShockwave(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ソースの合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// デスティネーションの合成方法の説明

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffShockwave, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 爆発の描画
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++)
	{// 爆発エフェクトを使っているとき処理
		if (g_aShockwave[nCntShockwave].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShockwave);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShockwave * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ソースの合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// デスティネーションの合成方法の説明
}

//-----------------------------------------------------------------
// 衝撃波の設定
//-----------------------------------------------------------------
void SetShockwave(D3DXVECTOR3 pos,D3DXVECTOR3 size, D3DXCOLOR col, float fSize, float fNB, float fNB_MAG, float nCntAlpha)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	SHOCKWAVE *pShockwave;

	// 変数の受け渡し
	pShockwave = &g_aShockwave[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShockwave->Lock(0, 0, (void**)&pVtx, 0);

	// 爆発の設定
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++, pShockwave++)
	{
		if (pShockwave->bUse == false)
		{
			// 位置設定
			pShockwave->pos = pos;

			// サイズ
			pShockwave->size = size;

			// 色設定
			pShockwave->col = col;

			// 拡大サイズの指定
			pShockwave->fSize = fSize;

			// 頂点座標の設定
			SetVertexShockwave(nCntShockwave);

			// ノックバック値の設定
			pShockwave->fNB = fNB;

			// ノックバック倍率
			pShockwave->fNB_MAG = fNB_MAG;

			// α値の移動量
			pShockwave->nCntAlpha = nCntAlpha;

			// trueで返す
			pShockwave->bUse = true;

			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffShockwave->Unlock();
}

//-----------------------------------------------------------------
// 衝撃波の頂点座標の設定
//-----------------------------------------------------------------
void SetVertexShockwave(int nIdx)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShockwave->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か指定
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aShockwave[nIdx].pos.x - g_aShockwave[nIdx].size.x, g_aShockwave[nIdx].pos.y + g_aShockwave[nIdx].size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aShockwave[nIdx].pos.x - g_aShockwave[nIdx].size.x, g_aShockwave[nIdx].pos.y - g_aShockwave[nIdx].size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aShockwave[nIdx].pos.x + g_aShockwave[nIdx].size.x, g_aShockwave[nIdx].pos.y + g_aShockwave[nIdx].size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aShockwave[nIdx].pos.x + g_aShockwave[nIdx].size.x, g_aShockwave[nIdx].pos.y - g_aShockwave[nIdx].size.y, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = g_aShockwave[nIdx].col;
	pVtx[1].col = g_aShockwave[nIdx].col;
	pVtx[2].col = g_aShockwave[nIdx].col;
	pVtx[3].col = g_aShockwave[nIdx].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffShockwave->Unlock();
}

//-----------------------------------------------------------------
// ノックバックの指定
//-----------------------------------------------------------------
float ShockwaveMove(void)
{
	return g_fKnockBack;
}