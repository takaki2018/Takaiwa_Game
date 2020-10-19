//-----------------------------------------------------------------
//
// 弾 (bullet.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "explosion.h"
#include "particle.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define VERTEX_CENTER_X (20)												// 中心座標のX座標
#define VERTEX_CENTER_Y (20)												// 中心座標のY座標

#define MAX_PATTERN     (8)													// アニメーションパターンNo.の最大数
#define TEXTURE_X		(8)													// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)													// アニメーションパターンNo.の縦の最大数

#define PLAY_ANIM       ((1.0f / TEXTURE_X) * pExprosion->nPatternAnim)		// テクスチャの動作
#define UV_U			(1.0f / TEXTURE_X + PLAY_ANIM)						// Uの値
#define UV_V			(1.0f / TEXTURE_Y)									// Vの値

#define MAX_EXPROSION	(256)												// 弾の最大値
#define MOVE_BULLET		(10.0f)												// 弾の移動量

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DCOLOR col;			// 色
	int nCounterAnim;		// アニメーションカウンター
	int nPatternAnim;		// テクスチャパターンのカウント
	bool bUse;				// 使用しているかどうか
} EXPLOSION;

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// 弾のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		// 弾の頂点バッファのポインタ
EXPLOSION g_aExplosion[MAX_EXPROSION];				// 弾の情報

//-----------------------------------------------------------------
// 爆発の初期化処理
//-----------------------------------------------------------------
HRESULT InitExplosion(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &g_pTextureExplosion);

	// 爆発の情報の初期化
	for (int nCntExprosion = 0; nCntExprosion < MAX_EXPROSION; nCntExprosion++)
	{
		g_aExplosion[nCntExprosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExprosion].nCounterAnim = 0;
		g_aExplosion[nCntExprosion].nPatternAnim = 0;
		g_aExplosion[nCntExprosion].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_aExplosion[nCntExprosion].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_EXPROSION, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++)
	{
		// 頂点座標の設定
		SetVertexExplosion(nCntExplosion);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// 弾の情報を次にうつす
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 爆発の終了処理
//-----------------------------------------------------------------
void UninitExplosion(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	// テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//-----------------------------------------------------------------
// 爆発の更新処理
//-----------------------------------------------------------------
void UpdateExplosion(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	EXPLOSION *pExprosion;
	pExprosion = &g_aExplosion[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// 爆発の更新
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++, pExprosion++)
	{
		if (pExprosion->bUse == true)
		{// カウンターアップ																																																																		
			pExprosion->nCounterAnim += 1;

			// 指定の値のとき処理
			if ((pExprosion->nCounterAnim % 4) == 0)
			{// アニメーションパターンNo.を更新
				pExprosion->nPatternAnim = (pExprosion->nPatternAnim + 1) % MAX_PATTERN;
			}

			// アニメーションの更新
			pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
			pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
			pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);

			// 頂点座標の設定
			SetVertexExplosion(nCntExplosion);

			// 色の更新
			pVtx[0].col = pExprosion->col;
			pVtx[1].col = pExprosion->col;
			pVtx[2].col = pExprosion->col;
			pVtx[3].col = pExprosion->col;

			// 爆発エフェクトの削除
			if (pExprosion->nPatternAnim == 7)
			{
				pExprosion->bUse = false;
				pExprosion->nPatternAnim = 0;
			}
		}
		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//-----------------------------------------------------------------
// 爆発の描画処理
//-----------------------------------------------------------------
void DrawExplosion(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 爆発の描画
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++)
	{// 爆発エフェクトを使っているとき処理
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// 爆発の設定
//-----------------------------------------------------------------
void SetExplosion(D3DXVECTOR3 pos,D3DCOLOR col)
{
	// 構造体のポインタ変数
	EXPLOSION *pExprosion;

	// 変数の受け渡し
	pExprosion = &g_aExplosion[0];

	// 爆発の設定
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++, pExprosion++)
	{
		if (pExprosion->bUse == false)
		{
			// 位置設定
			pExprosion->pos = pos;

			// 頂点座標の設定
			SetVertexExplosion(nCntExplosion);

			// 色設定
			pExprosion->col = col;

			// trueで返す
			pExprosion->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// 爆発の頂点座標
//-----------------------------------------------------------------
void SetVertexExplosion(int nIdx)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の情報を次にうつす
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
