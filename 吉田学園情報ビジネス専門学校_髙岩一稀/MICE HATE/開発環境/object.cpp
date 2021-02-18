//-----------------------------------------------------------------
//
// オブジェクト (object.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "object.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN     (1)										// アニメーションパターンNo.の最大数
#define TEXTURE_X		(1)										// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)										// アニメーションパターンNo.の縦の最大数

#define PLAY_ANIM       ((1.0f / TEXTURE_X) * g_nPatternAnim)   // テクスチャの動作
#define UV_U			(1.0f / TEXTURE_X + PLAY_ANIM)			// Uの値
#define UV_V			(1.0f / TEXTURE_Y)						// Vの値

#define OBJECT_SIZE_X	(10)									// オブジェクトの幅
#define OBJECT_SIZE_Y	(10)									// オブジェクトの高さ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObject = NULL;	// オブジェクトの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureObject = NULL;			// オブジェクトのテクスチャのポインタ
D3DXVECTOR3 g_posObject;							// オブジェクトの中心座標

//-----------------------------------------------------------------
// オブジェクトの初期化処理
//-----------------------------------------------------------------
HRESULT InitObject(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posObject = D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + 50.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cheese.png", &g_pTextureObject);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffObject,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffObject->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posObject.x - OBJECT_SIZE_X, g_posObject.y + OBJECT_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posObject.x - OBJECT_SIZE_X, g_posObject.y - OBJECT_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posObject.x + OBJECT_SIZE_X, g_posObject.y + OBJECT_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posObject.x + OBJECT_SIZE_X, g_posObject.y - OBJECT_SIZE_Y, 0.0f);

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
	g_pVtxBuffObject->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// オブジェクトの終了処理
//-----------------------------------------------------------------
void UninitObject(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffObject != NULL)
	{
		g_pVtxBuffObject->Release();
		g_pVtxBuffObject = NULL;
	}

	// テクスチャの開放
	if (g_pTextureObject != NULL)
	{
		g_pTextureObject->Release();
		g_pTextureObject = NULL;
	}
}

//-----------------------------------------------------------------
// オブジェクトの更新処理
//-----------------------------------------------------------------
void UpdateObject(void)
{

}

//-----------------------------------------------------------------
// オブジェクトの描画処理
//-----------------------------------------------------------------
void DrawObject(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffObject, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureObject);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}

//-----------------------------------------------------------------
// オブジェクトの位置情報の取得
//-----------------------------------------------------------------
D3DXVECTOR3 GetposObject(void)
{
	return g_posObject;
}