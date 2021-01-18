//-----------------------------------------------------------------
//
// ゲーム画面の背景 (bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bg.h"
#include "player.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN			(2)									// 背景の種類

#define MOVE_BG				(0.0001f)							// 背景の移動量

#define SKY_X				(SCREEN_WIDTH / 2)
#define SKY_Y				(SCREEN_HEIGHT / 2)
#define SKY_WIDTH			(SCREEN_WIDTH)
#define SKY_HEIGHT			(SCREEN_HEIGHT)

#define MOUNTAIN_X			(SCREEN_WIDTH / 2)
#define MOUNTAIN_Y			(SCREEN_HEIGHT / 2)
#define MOUNTAIN_WIDTH		(SCREEN_WIDTH)
#define MOUNTAIN_HEIGHT		(SCREEN_HEIGHT)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;				// 背景の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBg[MAX_PATTERN] = {};			// 背景のテクスチャのポインタ
D3DXVECTOR3 g_posSky;
D3DXVECTOR3 g_posMountain;

float g_fMoveUV = MOVE_BG;									// 背景のUV値の減少量
int g_nMoveBg= 60000;										// 背景の移動量

//-----------------------------------------------------------------
// 背景の初期化処理
//-----------------------------------------------------------------
HRESULT InitBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();
	g_posSky = D3DXVECTOR3(SKY_X, SKY_Y, 0.0f);
	g_posMountain = D3DXVECTOR3(MOUNTAIN_X, MOUNTAIN_Y ,0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cloud.png", &g_pTextureBg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/reaf.png", &g_pTextureBg[1]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
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

	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		// 頂点座標の設定
		SetVertexBg(nCntBg);

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
	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
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

	// 背景の更新
	pVtx[0].tex = D3DXVECTOR2(0.0f - g_fMoveUV, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f - g_fMoveUV, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f - g_fMoveUV, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f - g_fMoveUV, 0.0f);

	// 変数を減算
	g_fMoveUV -= MOVE_BG ;

	// 
	if (g_fMoveUV >= 1.0f)
	{
		g_fMoveUV = 0.0f;
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
	for (int nCntBg = 0; nCntBg < MAX_PATTERN; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureBg[nCntBg]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVertexBg(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCnt * 4;

	if (nCnt == 0)
	{
		pVtx[0].pos = D3DXVECTOR3(g_posSky.x - (SKY_WIDTH / 2), g_posSky.y + (SKY_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posSky.x - (SKY_WIDTH / 2), g_posSky.y - (SKY_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posSky.x + (SKY_WIDTH / 2), g_posSky.y + (SKY_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posSky.x + (SKY_WIDTH / 2), g_posSky.y - (SKY_HEIGHT / 2), 0.0f);
	}
	else if (nCnt == 1)
	{
		pVtx[0].pos = D3DXVECTOR3(g_posMountain.x - (MOUNTAIN_WIDTH / 2), g_posMountain.y + (MOUNTAIN_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posMountain.x - (MOUNTAIN_WIDTH / 2), g_posMountain.y - (MOUNTAIN_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posMountain.x + (MOUNTAIN_WIDTH / 2), g_posMountain.y + (MOUNTAIN_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posMountain.x + (MOUNTAIN_WIDTH / 2), g_posMountain.y - (MOUNTAIN_HEIGHT / 2), 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}