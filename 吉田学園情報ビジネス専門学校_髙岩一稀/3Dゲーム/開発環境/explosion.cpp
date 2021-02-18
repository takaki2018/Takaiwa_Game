//-----------------------------------------------------------------
//
// 爆発 (explosion.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "explosion.h"

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
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	int nCounterAnim;		// アニメーションカウンター
	int nPatternAnim;		// テクスチャパターンのカウント
	bool bUse;				// 使用しているかどうか
} Explosion;

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// 弾のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		// 弾の頂点バッファのポインタ
Explosion g_aExplosion[MAX_EXPROSION];				// 弾の情報

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
		g_aExplosion[nCntExprosion].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_EXPROSION, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - VERTEX_CENTER_X, g_aExplosion[nCntExplosion].pos.y + VERTEX_CENTER_Y, g_aExplosion[nCntExplosion].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + VERTEX_CENTER_X, g_aExplosion[nCntExplosion].pos.y + VERTEX_CENTER_Y, g_aExplosion[nCntExplosion].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - VERTEX_CENTER_X, g_aExplosion[nCntExplosion].pos.y - VERTEX_CENTER_Y, g_aExplosion[nCntExplosion].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + VERTEX_CENTER_X, g_aExplosion[nCntExplosion].pos.y - VERTEX_CENTER_Y, g_aExplosion[nCntExplosion].pos.z);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	VERTEX_3D *pVtx;
	Explosion *pExprosion;
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
	Explosion *pExplosion = &g_aExplosion[0];
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxView;							// ビューマトリックス取得用
	D3DXMATRIX mtxScaling, mtxTrans;			// 計算用マトリックス

	// 変数の受け渡し
	pDevice = GetDevice();

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファを適用
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pExplosion->mtxWorld);

			// ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 0
			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&pBillboard->mtxWorld, NULL, &mtxView);
			pBillboard->mtxWorld._41 = 0.0f;
			pBillboard->mtxWorld._42 = 0.0f;
			pBillboard->mtxWorld._43 = 0.0f;
#else
			pExplosion->mtxWorld._11 = mtxView._11;
			pExplosion->mtxWorld._12 = mtxView._21;
			pExplosion->mtxWorld._13 = mtxView._31;
			pExplosion->mtxWorld._21 = mtxView._12;
			pExplosion->mtxWorld._22 = mtxView._22;
			pExplosion->mtxWorld._23 = mtxView._32;
			pExplosion->mtxWorld._31 = mtxView._13;
			pExplosion->mtxWorld._32 = mtxView._23;
			pExplosion->mtxWorld._33 = mtxView._33;
#endif
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pExplosion->pos.x, pExplosion->pos.y, pExplosion->pos.z);
			D3DXMatrixMultiply(&pExplosion->mtxWorld, &pExplosion->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pExplosion->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureExplosion);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
	// 標準に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-----------------------------------------------------------------
// 爆発の設定
//-----------------------------------------------------------------
void SetExplosion(D3DXVECTOR3 pos)
{
	// 構造体のポインタ変数
	Explosion *pExprosion = &g_aExplosion[0];

	// 爆発の設定
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++, pExprosion++)
	{
		if (pExprosion->bUse == false)
		{
			// 位置設定
			pExprosion->pos = pos;

			// trueで返す
			pExprosion->bUse = true;

			break;
		}
	}
}
