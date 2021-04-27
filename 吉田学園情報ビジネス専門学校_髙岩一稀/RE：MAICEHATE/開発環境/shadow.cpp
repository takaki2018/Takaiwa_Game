//==============================================================================================================
//
// 影 (shadow.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "model_set.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_SHADOW		(256)			// 用意する影データの数

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;			// 影の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;					// 影のテクスチャのポインタ
Shadow g_aShadow[MAX_SHADOW];								// 影の情報

//--------------------------------------------------------------------------------------------------------------
// 影の初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitShadow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	// 構造体の要素の初期化
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].fWidth = 1.0f;
		g_aShadow[nCntShadow].fDepth = 1.0f;
		g_aShadow[nCntShadow].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_SHADOW,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fDepth);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fDepth);
		pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fDepth);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fDepth);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.9f);
		pVtx[1].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.9f);
		pVtx[2].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.9f);
		pVtx[3].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.9f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 次の情報へ
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// 影の終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitShadow(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

	// テクスチャの開放
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// 影の更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateShadow(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// 影の描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawShadow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ
	D3DXMATRIX mtxScaling,mtxRot, mtxTrans;	// 計算用マトリックス
	Shadow *pShadow = &g_aShadow[0];

	// 変数の受け渡し
	pDevice = GetDevice();

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pShadow++)
	{
		if (pShadow->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			// 向きを反映
			pShadow->mtxWorld._11 = pShadow->fWidth;
			pShadow->mtxWorld._33 = pShadow->fDepth;

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureShadow);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}
	// 通常の設定に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------------------------------------------------------------------------------------
// 影の配置
// 引数		：pos		- 位置情報
//			：fWidth	- 幅
//			：fDepth	- 奥行
// 返り値	：void		- 何も返さない
//--------------------------------------------------------------------------------------------------------------
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	// 変数宣言
	int nIdx = -1;						// 何番目の影か保存するための変数
	Shadow *pShadow = &g_aShadow[0];	// 影の構造体のポインタ変数

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pShadow++)
	{
		if (pShadow->bUse == false)
		{
			// 位置の設定
			pShadow->pos = pos;

			// 幅の設定
			pShadow->fWidth = fWidth;

			// 奥行の設定
			pShadow->fDepth = fDepth;

			// 使用状況をtrueにする
			pShadow->bUse = true;

			// 何番目に使用している影なのか保存する
			nIdx = nCntShadow;

			break;
		}
	}

	return nIdx;
}

//--------------------------------------------------------------------------------------------------------------
// 影の位置
// 引数		：nIdx	- 影番号をもらう
//			：pos	- 位置情報
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos)
{
	g_aShadow[nIdx].pos = pos;
}

//--------------------------------------------------------------------------------------------------------------
// 影の消去
// 引数		：nIdx	- 影番号をもらう
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DeleteShadow(int nIdx)
{	
	// 使用状態をfalseにする
	g_aShadow[nIdx].bUse = false;
}

//--------------------------------------------------------------------------------------------------------------
// 影情報の取得
// 引数		：void		- 何もなし
// 返り値	：Shadow	- 影情報の先頭アドレスを渡す
//--------------------------------------------------------------------------------------------------------------
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}

