//-------------------------------------------------------------------
//
// プレイヤーから出るポイント (point.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "point.h"
#include "player.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_POINT			(256)					// フルーツの最大値

#define POINT_SIZE_X		(20.0f)					// フルーツのサイズ(幅)
#define POINT_SIZE_Y		(20.0f)					// フルーツのサイズ(高さ)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPoint = NULL;				// 敵のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePoint[POINTTYPE_MAX] = {};	// 敵の頂点バッファのポインタ
POINTLOGO g_aPoint[MAX_POINT];									// 敵の情報

//-----------------------------------------------------------------
// 敵の初期化処理
//-----------------------------------------------------------------
HRESULT InitPoint(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	for (int nCnt = 0; nCnt < MAX_POINT; nCnt++)
	{
		g_aPoint[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPoint[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPoint[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aPoint[nCnt].fWidth = POINT_SIZE_X;
		g_aPoint[nCnt].fHeight = POINT_SIZE_Y;
		g_aPoint[nCnt].type = POINTTYPE_PLUS_ONE;
		g_aPoint[nCnt].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/plus_1.png", &g_pTexturePoint[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/plus_5.png", &g_pTexturePoint[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/decease_1.png", &g_pTexturePoint[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/decease_5.png", &g_pTexturePoint[3]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_POINT, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPoint,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_POINT; nCnt++)
	{
		// 頂点座標の設定
		SetVertexPoint(nCnt);

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

		// 次の情報に更新
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffPoint->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 敵の終了処理
//-----------------------------------------------------------------
void UninitPoint(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffPoint != NULL)
	{
		g_pVtxBuffPoint->Release();
		g_pVtxBuffPoint = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < POINTTYPE_MAX; nCnt++)
	{
		if (g_pTexturePoint[nCnt] != NULL)
		{
			g_pTexturePoint[nCnt]->Release();
			g_pTexturePoint[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// 敵の更新処理
//-----------------------------------------------------------------
void UpdatePoint(void)
{
	// 構造体のポインタ変数
	POINTLOGO *pPoint = &g_aPoint[0];

	for (int nCntPoint = 0; nCntPoint < MAX_POINT; nCntPoint++, pPoint++)
	{
		if (pPoint->bUse == true)
		{
			// 大きさの更新
			pPoint->fWidth += 0.1f;
			pPoint->fHeight += 0.1f;

			// 移動量の減少
			pPoint->move.y *= 0.95f;

			// 位置の更新
			pPoint->pos += pPoint->move;

			// α値の減少
			pPoint->col.a -= 0.02f;

			// 頂点座標の更新
			SetVertexPoint(nCntPoint);

			// α値が0.0fより小さくなったとき
			if (pPoint->col.a <= 0.0f)
			{
				pPoint->col.a = 1.0f;				// α値を初期化
				pPoint->fWidth = POINT_SIZE_X;		// 幅を初期化
				pPoint->fHeight = POINT_SIZE_Y;		// 高さを初期化
				pPoint->bUse = false;				// 使用状態をfalseにする
			}
		}
	}
}

//-----------------------------------------------------------------
// 敵の描画処理
//-----------------------------------------------------------------
void DrawPoint(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPoint, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// バウンドの描画
	for (int nCnt = 0; nCnt < MAX_POINT; nCnt++)
	{
		if (g_aPoint[nCnt].bUse == true)
		{
			pDevice->SetTexture(0, g_pTexturePoint[(int)(g_aPoint[nCnt].type)]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// 敵の設定
//-----------------------------------------------------------------
void SetPoint(D3DXVECTOR3 pos, D3DXVECTOR3 move, POINTTYPE type)
{
	// 構造体のポインタ変数
	POINTLOGO *pPoint = &g_aPoint[0];

	// 弾の設定
	for (int nCntPoint = 0; nCntPoint < MAX_POINT; nCntPoint++, pPoint++)
	{
		if (pPoint->bUse == false)
		{// 位置設定
			pPoint->pos = pos;

			// 移動量の設定
			pPoint->move = move;

			// タイプの確定
			pPoint->type = type;

			// trueで返す
			pPoint->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// 敵の頂点座標
//-----------------------------------------------------------------
void SetVertexPoint(int nIdx)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の情報を次にうつす
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPoint[nIdx].pos.x - g_aPoint[nIdx].fWidth, g_aPoint[nIdx].pos.y + g_aPoint[nIdx].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPoint[nIdx].pos.x - g_aPoint[nIdx].fWidth, g_aPoint[nIdx].pos.y - g_aPoint[nIdx].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPoint[nIdx].pos.x + g_aPoint[nIdx].fWidth, g_aPoint[nIdx].pos.y + g_aPoint[nIdx].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPoint[nIdx].pos.x + g_aPoint[nIdx].fWidth, g_aPoint[nIdx].pos.y - g_aPoint[nIdx].fHeight, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = g_aPoint[nIdx].col;
	pVtx[1].col = g_aPoint[nIdx].col;
	pVtx[2].col = g_aPoint[nIdx].col;
	pVtx[3].col = g_aPoint[nIdx].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffPoint->Unlock();
}