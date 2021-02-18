//-----------------------------------------------------------------
//
// パーティクル (particle.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "particle.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_PATTERN     (8)													// アニメーションパターンNo.の最大数
#define TEXTURE_X		(1)													// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)													// アニメーションパターンNo.の縦の最大数

#define MAX_PARTICLE	(1024)												// 弾の最大値

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fRandius;			// 半径
	float fCntAlpha;		// α値のカウント
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
} PARTICLE;

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	// パーティクルのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;		// パーティクルの頂点バッファのポインタ
PARTICLE g_aParticle[MAX_PARTICLE];					// パーティクルの情報
D3DXVECTOR3 g_posBase;

//-----------------------------------------------------------------
// 爆発の初期化処理
//-----------------------------------------------------------------
HRESULT InitParticle(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureParticle);

	// 変数の初期化
	g_posBase = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	// 爆発の情報の初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRandius = NULL;
		g_aParticle[nCntParticle].fCntAlpha = NULL;
		g_aParticle[nCntParticle].nLife = NULL;
		g_aParticle[nCntParticle].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PARTICLE, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// 弾の情報を次にうつす
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 爆発の終了処理
//-----------------------------------------------------------------
void UninitParticle(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}

	// テクスチャの開放
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}
}

//-----------------------------------------------------------------
// 爆発の更新処理
//-----------------------------------------------------------------
void UpdateParticle(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	PARTICLE *pParticle;
	

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	// 爆発の更新
	pParticle = &g_aParticle[0];
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == true)
		{// 中心座標の更新
			pParticle->pos.x += pParticle->move.x;
			pParticle->pos.y += pParticle->move.y;
			
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pParticle->pos.x - pParticle->fRandius, pParticle->pos.y + pParticle->fRandius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pParticle->pos.x - pParticle->fRandius, pParticle->pos.y - pParticle->fRandius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pParticle->pos.x + pParticle->fRandius, pParticle->pos.y + pParticle->fRandius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pParticle->pos.x + pParticle->fRandius, pParticle->pos.y - pParticle->fRandius, 0.0f);

			// 頂点カラーの更新
			pVtx[0].col = pParticle->col;
			pVtx[1].col = pParticle->col;
			pVtx[2].col = pParticle->col;
			pVtx[3].col = pParticle->col;

			// α値の計算
			pParticle->col.a -= pParticle->fCntAlpha;

			// 寿命の計算
			pParticle->nLife--;

			// 爆発エフェクトの削除
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
				pParticle->nLife = 0;
			}
		}
		// 情報の更新
		pVtx += 4;
	}

	for (int nCntAppear = 0; nCntAppear < 30; nCntAppear++)
	{// アドレスの初期化
		pParticle = &g_aParticle[0];
		for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++, pParticle++)
		{
			if (pParticle->bUse == false)
			{// 中心座標の設定
				pParticle->pos = g_posBase;
				
				// 半径の設定
				pParticle->fRandius = (float)(rand() % 15 + 5);

				// α値の減少値
				pParticle->fCntAlpha = 0.01f;
				
				// 色の設定
				pParticle->col = D3DXCOLOR(0.7f, 0.4f, 0.1f,1.0f);

				// 寿命の設定
				pParticle->nLife = 100;

				float fAngle; // 発射角度
				// 角度の確定
				fAngle = (float)(rand() % 628 - 314) / 100.0f;

				// 移動量の設定
				pParticle->move.x = sinf(fAngle) * (float)((rand() % 15 + 1) * 0.1f);
				pParticle->move.y = cosf(fAngle) * (float)((rand() % 15 + 1) * 0.1f);

				// trueで返す
				pParticle->bUse = true;

				break;
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}

//-----------------------------------------------------------------
// 爆発の描画処理
//-----------------------------------------------------------------
void DrawParticle(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ソースの合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// デスティネーションの合成方法の説明

															// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 爆発の描画
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// 爆発エフェクトを使っているとき処理
		if (g_aParticle[nCntParticle].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ソースの合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// デスティネーションの合成方法の説明
}

