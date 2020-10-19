//-----------------------------------------------------------------
//
// 背景 (bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bg.h"
#include "player.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN     (4)				// 背景の種類
#define MAX_COUNT_BG	(3)				// 背景の数

#define MOVE_BG			(0.002f)		// 背景の移動量

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;							// 背景の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBg[MAX_PATTERN] = {};						// 背景のテクスチャのポインタ

float g_fMoveUV[MAX_COUNT_BG] = { MOVE_BG ,MOVE_BG * 2 ,MOVE_BG * 3 };	// 背景のUV値の減少量
int g_nMoveBg[MAX_COUNT_BG] = {6000,4500,3000 };						// 背景の移動量

int g_nCntPattern;														// 読み込むテクスチャの数
MODE g_nmode;															// モードの識別

//-----------------------------------------------------------------
// 背景の初期化処理
//-----------------------------------------------------------------
HRESULT InitBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();
	g_nCntPattern = MAX_PATTERN;
	g_nmode = GetMode();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureBg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureBg[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg200.png", &g_pTextureBg[3]);

	// モードがタイトルのとき処理
	if (g_nmode == MODE_TITLE)
	{
		g_nCntPattern = MAX_COUNT_BG;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * g_nCntPattern, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < g_nCntPattern; nCntBg++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		if (nCntBg == 3)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT + 50.0f , 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0, 50.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT + 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,50.0f, 0.0f);
		}

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
	g_pVtxBuffBg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 背景の終了処理
//-----------------------------------------------------------------
void UninitBg(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}

	// テクスチャの開放
	for (int nCntBg = 0; nCntBg < g_nCntPattern; nCntBg++)
	{
		if (g_pTextureBg[nCntBg] != NULL)
		{
			g_pTextureBg[nCntBg]->Release();
			g_pTextureBg[nCntBg] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// 背景の更新処理
//-----------------------------------------------------------------
void UpdateBg(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;

	// 変数の受け渡し
	pPlayer = GetPlayer();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < MAX_COUNT_BG; nCntBg++)
	{
		// 背景の更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + pPlayer->pos.x / g_nMoveBg[nCntBg], 1.0f + g_fMoveUV[nCntBg] + pPlayer->pos.y / g_nMoveBg[nCntBg]);
		pVtx[1].tex = D3DXVECTOR2(0.0f + pPlayer->pos.x / g_nMoveBg[nCntBg], 0.0f + g_fMoveUV[nCntBg] + pPlayer->pos.y / g_nMoveBg[nCntBg]);
		pVtx[2].tex = D3DXVECTOR2(1.0f + pPlayer->pos.x / g_nMoveBg[nCntBg], 1.0f + g_fMoveUV[nCntBg] + pPlayer->pos.y / g_nMoveBg[nCntBg]);
		pVtx[3].tex = D3DXVECTOR2(1.0f + pPlayer->pos.x / g_nMoveBg[nCntBg], 0.0f + g_fMoveUV[nCntBg] + pPlayer->pos.y / g_nMoveBg[nCntBg]);

		// 情報の更新
		pVtx += 4;

		// 変数を減算
		g_fMoveUV[nCntBg] -= MOVE_BG * (nCntBg + 1);

		// 
		if (g_fMoveUV[nCntBg] >= 1.0f)
		{
			g_fMoveUV[nCntBg] = 0.0f;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

//-----------------------------------------------------------------
// 背景の描画処理
//-----------------------------------------------------------------
void DrawBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntBg = 0; nCntBg < g_nCntPattern; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureBg[nCntBg]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}