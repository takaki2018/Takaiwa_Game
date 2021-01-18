//-----------------------------------------------------------------
//
// カラス (crow.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "crow.h"
#include "sound.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_CROW			(4)												// カラスの最大数
#define TEXTURE_X			(4)													// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y			(1)													// アニメーションパターンNo.の縦の最大数

#define PLAY_ANIM			((1.0f / TEXTURE_X) * g_aCrow[nCnt].nPatternAnim)	// テクスチャの動作
#define UV_U				(1.0f / TEXTURE_X + PLAY_ANIM)						// Uの値
#define UV_V				(1.0f / TEXTURE_Y)									// Vの値

#define CROW_WIDTH			(20.0f)												// カラスの幅
#define CROW_HEIGHT			(20.0f)												// カラスの高さ

#define ANIMATION_CNT		(10)
#define MOVE_CROW			(2.0f)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCrow = NULL;	// 背景の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCrow = NULL;		// 背景のテクスチャのポインタ
CROW g_aCrow[MAX_CROW];							// カラスの情報
int g_nRandSetCnt;								// ランダムで配置するカウント
int g_nSetCrowCnt;								// カラスの設置カウント

//-----------------------------------------------------------------
// カラスの初期化処理
//-----------------------------------------------------------------
HRESULT InitCrow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();
	g_nRandSetCnt = rand() % 300 + 1000;
	g_nSetCrowCnt = 0;

	// 情報の初期化処理
	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++)
	{
		g_aCrow[nCntCrow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCrow[nCntCrow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCrow[nCntCrow].fWidth = CROW_WIDTH / 2.0f;
		g_aCrow[nCntCrow].fHeight = CROW_HEIGHT / 2.0f;
		g_aCrow[nCntCrow].nCntAnim = 0;
		g_aCrow[nCntCrow].nPatternAnim = 0;
		g_aCrow[nCntCrow].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/crow.png", &g_pTextureCrow);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_CROW,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffCrow,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCrow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++)
	{
		// 頂点座標の設定
		SetVertexCrow(nCntCrow);

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
	g_pVtxBuffCrow->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// カラスの終了処理
//-----------------------------------------------------------------
void UninitCrow(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffCrow != NULL)
	{
		g_pVtxBuffCrow->Release();
		g_pVtxBuffCrow = NULL;
	}

	// テクスチャの開放
	if (g_pTextureCrow != NULL)
	{
		g_pTextureCrow->Release();
		g_pTextureCrow = NULL;
	}
}

//-----------------------------------------------------------------
// カラスの更新処理
//-----------------------------------------------------------------
void UpdateCrow(void)
{
	// 構造体のポインタ変数
	CROW *pCrow = &g_aCrow[0];

	// 設置カウントのカウント
	g_nSetCrowCnt++;

	// 割り切れるとき設置
	if (g_nRandSetCnt == g_nSetCrowCnt)
	{// カラスの設置
		SetCrow(D3DXVECTOR3(SCREEN_WIDTH, (float)(rand() % 200 + 200), 0.0f),
			D3DXVECTOR3(-MOVE_CROW, 0.0f, 0.0f));

		// サウンド
		PlaySound(SOUND_LABEL_SE_CROW);

		// 変数の初期化
		g_nRandSetCnt = rand() % 300 + 1000;
		g_nSetCrowCnt = 0;
	}

	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++, pCrow++)
	{
		if (pCrow->bUse == true)
		{
			// 位置の更新
			pCrow->pos += pCrow->move;

			// 頂点座標の更新
			SetVertexCrow(nCntCrow);

			// 画面外に行ったとき
			if (pCrow->pos.x < 0.0f)
			{
				pCrow->bUse = false;
			}
		}
	}
}

//-----------------------------------------------------------------
// カラスの描画処理
//-----------------------------------------------------------------
void DrawCrow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCrow, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++)
	{
		if (g_aCrow[nCntCrow].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureCrow);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCrow * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVertexCrow(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCrow->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCnt * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aCrow[nCnt].pos.x - g_aCrow[nCnt].fWidth, g_aCrow[nCnt].pos.y + g_aCrow[nCnt].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCrow[nCnt].pos.x - g_aCrow[nCnt].fWidth, g_aCrow[nCnt].pos.y - g_aCrow[nCnt].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCrow[nCnt].pos.x + g_aCrow[nCnt].fWidth, g_aCrow[nCnt].pos.y + g_aCrow[nCnt].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCrow[nCnt].pos.x + g_aCrow[nCnt].fWidth, g_aCrow[nCnt].pos.y - g_aCrow[nCnt].fHeight, 0.0f);

	// アニメーションカウンターのカウント
	g_aCrow[nCnt].nCntAnim++;

	if (0 == g_aCrow[nCnt].nCntAnim % ANIMATION_CNT)
	{// 割り切れるとき処理
		g_aCrow[nCnt].nPatternAnim = (g_aCrow[nCnt].nPatternAnim + 1) % TEXTURE_X;
	}
	// アニメーションの更新
	pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
	pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
	pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffCrow->Unlock();
}

//-----------------------------------------------------------------
// カラスの設置
//-----------------------------------------------------------------
void SetCrow(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	// 構造体のポインタ変数
	CROW *pCrow = &g_aCrow[0];

	// カラスの配置
	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++, pCrow++)
	{
		if (pCrow->bUse == false)
		{
			// 位置の設定
			pCrow->pos = pos;

			// 移動量の設定
			pCrow->move = move;

			// 使用状態をtrueにする
			pCrow->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// カラス情報の取得
//-----------------------------------------------------------------
CROW *GetCrow(void)
{
	return &g_aCrow[0];
}