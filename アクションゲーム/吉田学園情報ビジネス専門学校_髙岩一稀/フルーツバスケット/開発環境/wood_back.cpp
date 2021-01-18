//-----------------------------------------------------------------
//
// 木の裏(wood.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "wood_back.h"
#include "bear.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define WOOD_BACK_X				(SCREEN_WIDTH / 2)				// 木の中心座標(X)
#define WOOD_BACK_Y				(SCREEN_HEIGHT / 2)				// 木の中心座標(Y)
#define WOOD_BACK_WIDTH			(SCREEN_WIDTH)					// 木の幅
#define WOOD_BACK_HEIGHT		(SCREEN_HEIGHT)					// 木の高さ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWoodBack = NULL;				// 木の裏の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWoodBack = NULL;					// 木の裏のテクスチャのポインタ
D3DXVECTOR3 g_posWoodBack;										// 木の裏の中心座標

//-----------------------------------------------------------------
// 木の裏の初期化処理
//-----------------------------------------------------------------
HRESULT InitWoodBack(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();
	g_posWoodBack = D3DXVECTOR3(WOOD_BACK_X, WOOD_BACK_Y, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wood_back.png", &g_pTextureWoodBack);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX,			// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWoodBack,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWoodBack->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetVertexWoodBack(0);

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

	// 頂点バッファをアンロックする
	g_pVtxBuffWoodBack->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 木の裏の終了処理
//-----------------------------------------------------------------
void UninitWoodBack(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffWoodBack != NULL)
	{
		g_pVtxBuffWoodBack->Release();
		g_pVtxBuffWoodBack = NULL;
	}

	// テクスチャの開放
	if (g_pTextureWoodBack != NULL)
	{
		g_pTextureWoodBack->Release();
		g_pTextureWoodBack = NULL;
	}
}

//-----------------------------------------------------------------
// 木の裏の更新処理
//-----------------------------------------------------------------
void UpdateWoodBack(void)
{
	// 頂点座標の更新
	SetVertexWoodBack(0);
}

//-----------------------------------------------------------------
// 木の裏の描画処理
//-----------------------------------------------------------------
void DrawWoodBack(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWoodBack, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTextureWoodBack);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVertexWoodBack(int nCnt)
{
	// 変数宣言
	float ShaikingMove = GetShaking();

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWoodBack->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posWoodBack.x - (WOOD_BACK_WIDTH / 2), g_posWoodBack.y + (WOOD_BACK_HEIGHT / 2) + ShaikingMove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posWoodBack.x - (WOOD_BACK_WIDTH / 2), g_posWoodBack.y - (WOOD_BACK_HEIGHT / 2) + ShaikingMove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posWoodBack.x + (WOOD_BACK_WIDTH / 2), g_posWoodBack.y + (WOOD_BACK_HEIGHT / 2) + ShaikingMove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posWoodBack.x + (WOOD_BACK_WIDTH / 2), g_posWoodBack.y - (WOOD_BACK_HEIGHT / 2) + ShaikingMove, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffWoodBack->Unlock();
}