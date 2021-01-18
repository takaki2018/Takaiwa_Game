//-----------------------------------------------------------------
//
// エフェクト (effect.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "effect.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_EFFECT	(2096)						// エフェクトの最大値

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fRemove;			// 移動量の減少量
	float fRandius;			// 半径
	float nCntAlpha;		// α値の移動
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
} EFFECT;

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// エフェクトのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			// エフェクトの頂点バッファのポインタ
EFFECT g_aEffect[MAX_EFFECT];						// エフェクトの情報

//-----------------------------------------------------------------
// エフェクトの初期化処理
//-----------------------------------------------------------------
HRESULT InitEffect(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	// 爆発の情報の初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aEffect[nCntEffect].fRemove = NULL;
		g_aEffect[nCntEffect].fRandius = NULL;
		g_aEffect[nCntEffect].nCntAlpha = NULL;
		g_aEffect[nCntEffect].nLife = NULL;
		g_aEffect[nCntEffect].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_EFFECT, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		// 弾の情報を次にうつす
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// エフェクトの終了処理
//-----------------------------------------------------------------
void UninitEffect(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	// テクスチャの開放
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//-----------------------------------------------------------------
// エフェクトの更新処理
//-----------------------------------------------------------------
void UpdateEffect(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	EFFECT *pEffect;
	pEffect = &g_aEffect[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 爆発の更新
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == true)
		{// 中心座標の更新
			pEffect->pos.x += pEffect->move.x;
			pEffect->pos.y += pEffect->move.y;

			// 移動量の更新
			pEffect->move.x *= pEffect->fRemove;
			pEffect->move.y *= pEffect->fRemove;
			
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - pEffect->fRandius, pEffect->pos.y + pEffect->fRandius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - pEffect->fRandius, pEffect->pos.y - pEffect->fRandius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + pEffect->fRandius, pEffect->pos.y + pEffect->fRandius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + pEffect->fRandius, pEffect->pos.y - pEffect->fRandius, 0.0f);

			// α値の加算
 			pEffect->col.a -= pEffect->nCntAlpha;

			// 寿命の更新
			pEffect->nLife--;

			// 色の更新
			pVtx[0].col = pEffect->col;
		  	pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;  
			pVtx[3].col = pEffect->col;

			// 爆発エフェクトの削除
			if (pEffect->nLife <= 0)
			{
				pEffect->bUse = false;
				pEffect->col.a = 1.0f;
				pEffect->nLife = 0;
			}
		}
		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//-----------------------------------------------------------------
// エフェクトの描画処理
//-----------------------------------------------------------------
void DrawEffect(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 加算合成に設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 爆発の描画
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// 爆発エフェクトを使っているとき処理
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// 通常に戻す
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//-----------------------------------------------------------------
// エフェクトの設定
//-----------------------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRemove,float fRandius, float fCntAlpha, int nLife)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	EFFECT *pEffect;

	// 変数の受け渡し
	pEffect = &g_aEffect[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// エフェクトの設定
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			// 位置設定
			pEffect->pos = pos;

			// 移動量の設定
			pEffect->move = move;

			// 色設定
			pEffect->col = col;

			// 移動量の減少値
			pEffect->fRemove = fRemove;

			// 半径の設定
			pEffect->fRandius = fRandius;		

			// α値の移動量
			pEffect->nCntAlpha = fCntAlpha;

			// 寿命の設定
			pEffect->nLife = nLife;

			// trueで返す
			pEffect->bUse = true;

			break;
		}
	}
	// 頂点バッファをアンロッ クする
	g_pVtxBuffEffect->Unlock();
}
