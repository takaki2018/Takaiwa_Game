//-----------------------------------------------------------------
//
// ポリゴン (polygon.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "polygon.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_POLYGON			(64)			// 用意するポリゴンデータの数
#define POLYGON_SIZE		(200.0f)		// ポリゴンのサイズ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;				// ポリゴンの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;					// ポリゴンのテクスチャのポインタ
Floor g_aFloor[MAX_POLYGON];								// ポリゴン情報

//-----------------------------------------------------------------
// ポリゴンの初期化処理
//-----------------------------------------------------------------
HRESULT InitPolygon(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		g_aFloor[nCntPolygon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFloor[nCntPolygon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFloor[nCntPolygon].fWidth = 1.0f;
		g_aFloor[nCntPolygon].fDepth = 1.0f;
		g_aFloor[nCntPolygon].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field001.jpg", &g_pTexturePolygon);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_POLYGON,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aFloor[nCntPolygon].pos.x - g_aFloor[nCntPolygon].fWidth / 2.0f, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z + g_aFloor[nCntPolygon].fDepth / 2.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFloor[nCntPolygon].pos.x + g_aFloor[nCntPolygon].fWidth / 2.0f, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z + g_aFloor[nCntPolygon].fDepth / 2.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFloor[nCntPolygon].pos.x - g_aFloor[nCntPolygon].fWidth / 2.0f, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z - g_aFloor[nCntPolygon].fDepth / 2.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFloor[nCntPolygon].pos.x + g_aFloor[nCntPolygon].fWidth / 2.0f, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z - g_aFloor[nCntPolygon].fDepth / 2.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		// 次の情報へ
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();

	// 床の配置
	for (int nCntPolygon = 0; nCntPolygon < 5; nCntPolygon++)
	{
		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			SetPolygon(D3DXVECTOR3(-400.0f + POLYGON_SIZE * nCntPolygon, 0.0f, -400.0f + POLYGON_SIZE * nCnt),
				POLYGON_SIZE, POLYGON_SIZE);
		}
	}

	return S_OK;
}

//-----------------------------------------------------------------
// ポリゴンの終了処理
//-----------------------------------------------------------------
void UninitPolygon(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}

	// テクスチャの開放
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}
}

//-----------------------------------------------------------------
// ポリゴンの更新処理
//-----------------------------------------------------------------
void UpdatePolygon(void)
{

}

//-----------------------------------------------------------------
// ポリゴンの描画処理
//-----------------------------------------------------------------
void DrawPolygon(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// 変数の受け渡し
	pDevice = GetDevice();

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (g_aFloor[nCntPolygon].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aFloor[nCntPolygon].mtxWorld);

			// スケールの反映
			g_aFloor[nCntPolygon].mtxWorld._11 = g_aFloor[nCntPolygon].fWidth;
			g_aFloor[nCntPolygon].mtxWorld._33 = g_aFloor[nCntPolygon].fDepth;

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aFloor[nCntPolygon].rot.y, g_aFloor[nCntPolygon].rot.x, g_aFloor[nCntPolygon].rot.z);
			D3DXMatrixMultiply(&g_aFloor[nCntPolygon].mtxWorld, &g_aFloor[nCntPolygon].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aFloor[nCntPolygon].pos.x, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z);
			D3DXMatrixMultiply(&g_aFloor[nCntPolygon].mtxWorld, &g_aFloor[nCntPolygon].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aFloor[nCntPolygon].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// ポリゴンの描画
			pDevice->SetTexture(0, g_pTexturePolygon);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// ポリゴンの設置
//-----------------------------------------------------------------
void SetPolygon(D3DXVECTOR3 pos,float fWidth,float fDepth)
{
	// 構造体のポインタ変数
	Floor *pFloor = &g_aFloor[0];

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++,pFloor++)
	{
		if (pFloor->bUse == false)
		{
			// 位置の設定
			pFloor->pos = pos;

			// 幅の設定
			pFloor->fWidth = fWidth;

			// 奥行の設定
			pFloor->fDepth = fDepth;

			// 使用している状態にする
			pFloor->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// オブジェクトの当たり判定
//-----------------------------------------------------------------
bool CollisionFloor(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// 構造体のポインタ変数
	Floor *pFloor = &g_aFloor[0];

	// 変数宣言
	bool bGetCoin = false;		// コインを獲得したかどうか

	/*for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++, pFloor++)
	{
		if (pFloor->bUse == true)
		{
			if ((pPos->x - pvtxMin->x) < (pFloor->pos.x + pFloor->vtxMaxCoin.x) &&
				(pPos->x + pvtxMax->x) > (pFloor->pos.x - pFloor->vtxMinCoin.x))
			{// プレイヤーがブロックの中にいるとき
				if (pPosOld->z + pvtxMax->z <= (pFloor->pos.z - pFloor->vtxMinCoin.z))
				{// プレイヤーがブロックの上にいるとき
					if (pPos->z + pvtxMax->z >(pFloor->pos.z - pFloor->vtxMinCoin.z))
					{// めり込んだら処理
						pPos->z = pFloor->pos.z - pFloor->vtxMinCoin.z - pvtxMax->z;
						bGetCoin = true;
					}
				}
				else if ((pPosOld->z - pvtxMin->z) >= (pFloor->pos.z + pFloor->vtxMaxCoin.z))
				{// プレイヤーがブロックの下にいるとき
					if ((pPos->z - pvtxMin->z) < (pFloor->pos.z + pFloor->vtxMaxCoin.z))
					{// めり込んだらブロックの下の座標にする
						pPos->z = (pFloor->pos.z + pFloor->vtxMaxCoin.z + pvtxMin->z);
						bGetCoin = true;
					}
				}
			}
			if ((pPos->z + pvtxMax->z) > pFloor->pos.z - pFloor->vtxMinCoin.z &&
				(pPos->z - pvtxMin->z) < (pFloor->pos.z + pFloor->vtxMaxCoin.z))
			{// プレイヤーがブロックのY幅にいるとき
				if ((pPosOld->x + pvtxMax->x) <= pFloor->pos.x - pFloor->vtxMinCoin.x)
				{// プレイヤーがブロックの左側にいるとき
					if ((pPos->x + pvtxMax->x) > pFloor->pos.x - pFloor->vtxMinCoin.x)
					{// めり込んだらブロックの左の座標にする
						pPos->x = pFloor->pos.x - pFloor->vtxMinCoin.x - pvtxMax->x;
						bGetCoin = true;
					}
				}
				else if ((pPosOld->x - pvtxMin->x) >= (pFloor->pos.x + pFloor->vtxMaxCoin.x))
				{// プレイヤーがブロックの右側にいるとき
					if ((pPos->x - pvtxMin->x) < (pFloor->pos.x + pFloor->vtxMaxCoin.x))
					{// めり込んだらブロックの右の座標にする
						pPos->x = pFloor->pos.x + pFloor->vtxMaxCoin.x + pvtxMin->x;
						bGetCoin = true;
					}
				}
			}
		}
	}
	*/
	return bGetCoin;
}

//-----------------------------------------------------------------
// ポリゴン情報の取得
//-----------------------------------------------------------------
Floor *Getpolygon(void)
{
	return &g_aFloor[0];
}