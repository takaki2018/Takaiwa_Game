//-----------------------------------------------------------------
//
// エフェクト (effect.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "effect.h"
#include "shadow.h"
#include "meshfield.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_EFFECT	(4096)						// エフェクトの最大値

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 posOld;		// 前の座標
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fRemove;			// 移動量の減少量
	float fRandius;			// 半径
	float nCntAlpha;		// α値の移動
	float fGravity;			// 重力
	float fBound;			// 跳ね返り係数
	int nLife;				// 寿命
	int nIdx;				// 影の管理番号
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
		g_aEffect[nCntEffect].fRandius = 1.0f;
		g_aEffect[nCntEffect].nCntAlpha = NULL;
		g_aEffect[nCntEffect].fGravity = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].nIdx = -1;
		g_aEffect[nCntEffect].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_EFFECT, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.z);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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
	VERTEX_3D *pVtx;
	EFFECT *pEffect;
	pEffect = &g_aEffect[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 爆発の更新
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == true)
		{
			// 座標の保存
			pEffect->posOld = pEffect->pos;

			// 中心座標の更新
			pEffect->pos.x += pEffect->move.x;
			pEffect->pos.y += pEffect->move.y;
			pEffect->pos.z += pEffect->move.z;

			// 移動量の更新
			pEffect->move.x *= pEffect->fRemove;
			pEffect->move.y *= pEffect->fRemove;
			pEffect->move.z *= pEffect->fRemove;

			// 重力の加算
			pEffect->move.y -= pEffect->fGravity;

			// メッシュフィールドとの当たり判定
			bool bCollision = CollisionMeshField(&pEffect->pos, &pEffect->posOld);

			// バウンド
			if (bCollision == true)
			{
				pEffect->move.y *= -pEffect->fBound;
			}
			// α値の加算
			pEffect->col.a -= pEffect->nCntAlpha;

			// 色の更新
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			// エフェクトの削除
			if (pEffect->col.a <= 0.0f)
			{
				pEffect->bUse = false;
				pEffect->col.a = 1.0f;

				// 影を消す
				DeleteShadow(pEffect->nIdx);
			}
			// 影の移動
			SetPositionShadow(pEffect->nIdx,D3DXVECTOR3(pEffect->pos.x,0.0f,pEffect->pos.z));
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
	EFFECT *pEffect = &g_aEffect[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView;							// ビューマトリックス取得用
	D3DXMATRIX mtxScaling,mtxTrans;						// 計算用マトリックス

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファを適用
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 加算合成に設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 爆発の描画
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{// 爆発エフェクトを使っているとき処理
		if (pEffect->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pEffect->mtxWorld);

			// ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 0
			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&pEffect->mtxWorld, NULL, &mtxView);
			pEffect->mtxWorld._41 = 0.0f;
			pEffect->mtxWorld._42 = 0.0f;
			pEffect->mtxWorld._43 = 0.0f;
#else
			pEffect->mtxWorld._11 = mtxView._11 * pEffect->fRandius;
			pEffect->mtxWorld._12 = mtxView._21 * pEffect->fRandius;
			pEffect->mtxWorld._13 = mtxView._31 * pEffect->fRandius;
			pEffect->mtxWorld._21 = mtxView._12 * pEffect->fRandius;
			pEffect->mtxWorld._22 = mtxView._22 * pEffect->fRandius;
			pEffect->mtxWorld._23 = mtxView._32 * pEffect->fRandius;
			pEffect->mtxWorld._31 = mtxView._13 * pEffect->fRandius;
			pEffect->mtxWorld._32 = mtxView._23 * pEffect->fRandius;
			pEffect->mtxWorld._33 = mtxView._33 * pEffect->fRandius;
#endif

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);
			D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// 通常に戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// デスティネーションの合成方法の説明

	// 標準に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-----------------------------------------------------------------
// エフェクトの設定
//-----------------------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRemove,float fRandius, float fCntAlpha, float fGravity, float fBound)
{
	// 構造体のポインタ変数
	EFFECT *pEffect;

	// 変数の受け渡し
	pEffect = &g_aEffect[0];

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

			// 重力の設定
			pEffect->fGravity = fGravity;

			// 跳ね返り係数の設定
			pEffect->fBound = fBound;

			// trueで返す
			pEffect->bUse = true;

			// 影の設定
			pEffect->nIdx = SetShadow(D3DXVECTOR3(pos.x,0.0f,pos.z), pEffect->fRandius, pEffect->fRandius);

			break;
		}
	}
}
