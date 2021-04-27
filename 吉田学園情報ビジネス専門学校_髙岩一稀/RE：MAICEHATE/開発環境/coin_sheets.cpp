//==============================================================================================================
//
// コイン獲得状況 (coin_sheets.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "coin_sheets.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_COINSHEETS				(2)						// 画面に貼るテクスチャの最大数

#define COIN_POS_X					(60.0f)					// コインテクスチャの位置(X)
#define COIN_POS_Y					(50.0f)					// コインテクスチャの位置(Y)
#define COIN_SIZE					(40.0f)					// コインテクスチャのサイズ

#define NUMBER_POS_X				(160.0f)				// 数字テクスチャの位置(X)
#define NUMBER_POS_Y				(50.0f)					// 数字テクスチャの位置(Y)
#define NUMBER_SIZE_X				(20.0f)					// 数字テクスチャの幅
#define NUMBER_SIZE_Y				(40.0f)					// 数字テクスチャの高さ

#define MAX_PATTERN					(10)					// アニメーションパターンNo.の最大数
#define TEXTURE_X					(10)					// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y					(1)						// アニメーションパターンNo.の縦の最大数

#define FLASH_INTERBAL_NUM			(20)			// タイマーの点滅インターバル
#define CLEAR_NUMBER_NUM			(10)			// タイマーの透明番号
#define LESS_CLEAR_NUMBER_NUM		(0)				// タイマーの不透明番号

//--------------------------------------------------------------------------------------------------------------
// 列挙型
//--------------------------------------------------------------------------------------------------------------
typedef enum
{
	COINSHEETS_TEXTURETYPE_COIN = 0,	// コインのテクスチャ
	COINSHEETS_TEXTURETYPE_NUMBER,		// 数字のテクスチャ
	COINSHEETS_TEXTURETYPE_MAX,
}COINSHEETS_TEXTURETYPE;

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCoinSheets = NULL;							// コイン獲得状況の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCoinSheets[COINSHEETS_TEXTURETYPE_MAX] = {};		// コイン獲得状況のテクスチャのポインタ
D3DXVECTOR3 g_posCoinTex;														// コインテクスチャの中心座標
D3DXVECTOR3 g_posNumTex;														// テクスチャの中心座標
D3DXCOLOR g_colNumTex;															// 数字テクスチャの色
int g_nCntFlashNum;																// 数字テクスチャの点滅カウント
int g_nCntGetCoin;				

//--------------------------------------------------------------------------------------------------------------
// コイン獲得状況の初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitCoinSheets(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posCoinTex = D3DXVECTOR3(COIN_POS_X, COIN_POS_Y, 0.0f);
	g_posNumTex = D3DXVECTOR3(NUMBER_POS_X, NUMBER_POS_Y,0.0f);
	g_colNumTex = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0);
	g_nCntFlashNum = 0;
	g_nCntGetCoin = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/coin.png", &g_pTextureCoinSheets[COINSHEETS_TEXTURETYPE_COIN]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureCoinSheets[COINSHEETS_TEXTURETYPE_NUMBER]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffCoinSheets,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCoinSheets->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCoinSheets = 0; nCntCoinSheets < MAX_PATTERN; nCntCoinSheets++)
	{
		// 頂点座標の設定
		SetVertexCoinSheets(nCntCoinSheets);

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

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffCoinSheets->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// コイン獲得状況の終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitCoinSheets(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffCoinSheets != NULL)
	{
		g_pVtxBuffCoinSheets->Release();
		g_pVtxBuffCoinSheets = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < COINSHEETS_TEXTURETYPE_MAX; nCntTexture++)
	{
		if (g_pTextureCoinSheets[nCntTexture] != NULL)
		{
			g_pTextureCoinSheets[nCntTexture]->Release();
			g_pTextureCoinSheets[nCntTexture] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// コイン獲得状況の更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateCoinSheets(void)
{
	if (g_nCntGetCoin == 7)
	{
		if (CLEAR_NUMBER_NUM == g_nCntFlashNum % FLASH_INTERBAL_NUM)
		{// タイマーを点滅させる
			g_colNumTex.a = 0.0f;
		}
		else if (LESS_CLEAR_NUMBER_NUM == g_nCntFlashNum % FLASH_INTERBAL_NUM)
		{
			g_colNumTex.a = 1.0f;
		}
		// 点滅カウンタのカウント
		g_nCntFlashNum++;
	}
	// 情報の更新
	for (int nCntTex = 0; nCntTex < COINSHEETS_TEXTURETYPE_MAX; nCntTex++)
	{
		SetVertexCoinSheets(nCntTex);
	}
}

//--------------------------------------------------------------------------------------------------------------
// コイン獲得状況の描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawCoinSheets(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCoinSheets, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntCoinSheets = 0; nCntCoinSheets < COINSHEETS_TEXTURETYPE_MAX; nCntCoinSheets++)
	{
		pDevice->SetTexture(0, g_pTextureCoinSheets[nCntCoinSheets]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCoinSheets * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// 頂点座標の設定
// 引数		：nIdx	- 何番目か
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetVertexCoinSheets(int nIdx)
{
	switch (nIdx)
	{// コインテクスチャの情報
	case COINSHEETS_TEXTURETYPE_COIN:
		SetCoinTexInfo(nIdx);
		break;
		// 数字テクスチャの情報
	case COINSHEETS_TEXTURETYPE_NUMBER:
		SetNumberTexInfo(nIdx);
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------
// コインテクスチャの設置情報
// 引数		：nIdx	- 何番目か
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetCoinTexInfo(int nIdx)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCoinSheets->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の情報を次にうつす
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posCoinTex.x - COIN_SIZE, g_posCoinTex.y + COIN_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posCoinTex.x - COIN_SIZE, g_posCoinTex.y - COIN_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posCoinTex.x + COIN_SIZE, g_posCoinTex.y + COIN_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posCoinTex.x + COIN_SIZE, g_posCoinTex.y - COIN_SIZE, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffCoinSheets->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// 数字テクスチャの設置情報
// 引数		：nIdx	- 何番目か
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetNumberTexInfo(int nIdx)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCoinSheets->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の情報を次にうつす
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posNumTex.x - NUMBER_SIZE_X, g_posNumTex.y + NUMBER_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posNumTex.x - NUMBER_SIZE_X, g_posNumTex.y - NUMBER_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posNumTex.x + NUMBER_SIZE_X, g_posNumTex.y + NUMBER_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posNumTex.x + NUMBER_SIZE_X, g_posNumTex.y - NUMBER_SIZE_Y, 0.0f);

	// テクスチャの更新
	pVtx[0].tex = D3DXVECTOR2(0.0f + g_nCntGetCoin * 0.1f, 1.0f / TEXTURE_Y);
	pVtx[1].tex = D3DXVECTOR2(0.0f + g_nCntGetCoin * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_nCntGetCoin* 0.1f, 1.0f / TEXTURE_Y);
	pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_nCntGetCoin* 0.1f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = g_colNumTex;
	pVtx[1].col = g_colNumTex;
	pVtx[2].col = g_colNumTex;
	pVtx[3].col = g_colNumTex;

	// 頂点バッファをアンロックする
	g_pVtxBuffCoinSheets->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// コイン獲得状況の加算
// 引数		：nValue	- 加算数
// 返り値	：void		- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void AddCoinSheets(int nValue)
{
	// スコアを格納
	g_nCntGetCoin += nValue;

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCoinSheets->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの更新
	pVtx[4].tex = D3DXVECTOR2(0.0f + g_nCntGetCoin * 0.1f, 1.0f / TEXTURE_Y);
	pVtx[5].tex = D3DXVECTOR2(0.0f + g_nCntGetCoin * 0.1f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_nCntGetCoin* 0.1f, 1.0f / TEXTURE_Y);
	pVtx[7].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_nCntGetCoin* 0.1f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffCoinSheets->Unlock();
}