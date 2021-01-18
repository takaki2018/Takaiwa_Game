//-----------------------------------------------------------------
//
// アイテム (item.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "item.h"
#include "shockwave.h"
#include "object.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN	(3)						// アニメーションパターンNo.の最大数

#define ITEM_SIZE_X	(20)					// アイテムの幅
#define ITEM_SIZE_Y	(20)					// アイテムの高さ
#define ITEM_WIDTH	(50)					// アイテムが置かれる幅

#define MAX_ITEM	(MAX_PATTERN)			// アイテムの最大数
//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// アイテムの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureItem = NULL;		// アイテムのテクスチャにポインタ
D3DXVECTOR3 g_posItem;							// オブジェクトの中心座標
bool g_bUseItem;								// アイテムが使用中かどうか
int g_nCntItem;									// アイテム数

//-----------------------------------------------------------------
// アイテムの初期化処理
//-----------------------------------------------------------------
HRESULT InitItem(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posItem = D3DXVECTOR3(ITEM_SIZE_X + 10, ITEM_SIZE_Y + 10, 0.0f);
	g_bUseItem = false;
	g_nCntItem = MAX_ITEM;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cheese.png", &g_pTextureItem);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_PATTERN; nCntItem++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posItem.x - ITEM_SIZE_X + ITEM_WIDTH * nCntItem, g_posItem.y + ITEM_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posItem.x - ITEM_SIZE_X + ITEM_WIDTH * nCntItem, g_posItem.y - ITEM_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posItem.x + ITEM_SIZE_X + ITEM_WIDTH * nCntItem, g_posItem.y + ITEM_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posItem.x + ITEM_SIZE_X + ITEM_WIDTH * nCntItem, g_posItem.y - ITEM_SIZE_Y, 0.0f);

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
	g_pVtxBuffItem->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// アイテムの終了処理
//-----------------------------------------------------------------
void UninitItem(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}

	// テクスチャの開放
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}
}

//-----------------------------------------------------------------
// アイテムの更新処理
//-----------------------------------------------------------------
void UpdateItem(void)
{
	// 変数宣言
	D3DXVECTOR3 posObject = GetposObject();

	// アイテムの使用できるか
	if (g_nCntItem != 0 && g_bUseItem != true)
	{
		if (GetKeyboardTrigger(KEYINFO_ITEM) == true || GetJoypadTrigger(JPINFO_ITEM) == true)
		{// サウンド
			PlaySound(SOUND_LABEL_SE_ITEM);
			
			// アイテムの使用
			g_nCntItem = (g_nCntItem - 1) % MAX_ITEM;	// アイテム数のカウント
			g_bUseItem = true;							// アイテムの使用

			// 衝撃波の配置
			SetShockwave(posObject,D3DXVECTOR3(10.0f,10.0f,10.0f) ,
				D3DXCOLOR(0.5f, 0.5f, 0.1f, 1.0f), 8.0f, 50.0f,0.9f, 0.02f);

			SetShockwave(posObject, D3DXVECTOR3(10.0f, 10.0f, 10.0f),
				D3DXCOLOR(0.5f, 0.5f, 0.1f, 1.0f), 5.0f, 0.0f,0.0f, 0.05f);
		}
	}
}

//-----------------------------------------------------------------
// アイテムの描画処理
//-----------------------------------------------------------------
void DrawItem(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntItem = 0; nCntItem < g_nCntItem; nCntItem++)
	{
		pDevice->SetTexture(0, g_pTextureItem);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
	}
}

//-----------------------------------------------------------------
// アイテムの使用判定
//-----------------------------------------------------------------
bool GetbUseItem(void)
{
	return g_bUseItem;
}

//-----------------------------------------------------------------
// アイテムの使用判定
//-----------------------------------------------------------------
void SetItem(bool bUseItem)
{
	g_bUseItem = bUseItem;
}
