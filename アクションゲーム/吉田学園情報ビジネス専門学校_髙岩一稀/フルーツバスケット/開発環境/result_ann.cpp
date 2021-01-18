//-----------------------------------------------------------------
//
// リザルトロゴ (count_down.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_ann.h"
#include "gauge.h"
#include "timer.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_TEXTURE			(1)						// スコアテクスチャの最大値

#define RESULT_ANN_X		(0.0f)					// 結果発表の中心座標(X)
#define RESULT_ANN_Y		(SCREEN_HEIGHT / 2.0f)	// 結果発表の中心座標(Y)

#define RESULT_ANN_WIDTH	(500.0f)					// リザルトロゴの幅
#define RESULT_ANN_HEIGHT	(125.0f)					// リザルトロゴの高さ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultAnn = NULL;		// 結果発表のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResultAnn = NULL;			// 結果発表の頂点バッファのポインタ
RESULTANN g_aResultAnn;									// 結果発表の情報

//-----------------------------------------------------------------
// 結果発表の初期化処理
//-----------------------------------------------------------------
HRESULT InitResultAnn(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result_ann.png", &g_pTextureResultAnn);

	// 変数の初期化
	g_aResultAnn.pos = D3DXVECTOR3(RESULT_ANN_X, RESULT_ANN_Y, 0.0f);
	g_aResultAnn.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aResultAnn.fWidth = RESULT_ANN_WIDTH;
	g_aResultAnn.fHeight = RESULT_ANN_HEIGHT;
	g_aResultAnn.nCounterState = 100;
	g_aResultAnn.bSound = false;
	g_aResultAnn.bUse = true;

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultAnn,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultAnn->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	SetVErtexResultAnn();

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// テクスチャの更新
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffResultAnn->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 結果発表の終了処理
//-----------------------------------------------------------------
void UninitResultAnn(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffResultAnn != NULL)
	{
		g_pVtxBuffResultAnn->Release();
		g_pVtxBuffResultAnn = NULL;
	}

	// テクスチャの開放
	if (g_pTextureResultAnn != NULL)
	{
		g_pTextureResultAnn->Release();
		g_pTextureResultAnn = NULL;
	}
}

//-----------------------------------------------------------------
// 結果発表の更新処理
//-----------------------------------------------------------------
void UpdateResultAnn(void)
{
	if (g_aResultAnn.bUse == true)
	{// 使われているとき
		if (g_aResultAnn.pos.x < SCREEN_WIDTH / 2.0f  && g_aResultAnn.nCounterState > 0)
		{// まずは左から中央に移動
			g_aResultAnn.pos.x += 15.0f;

			if (g_aResultAnn.bSound == false)
			{// サウンド
				PlaySound(SOUND_LABEL_SE_THROW);

				g_aResultAnn.bSound = true;
			}
		}
		else if (g_aResultAnn.pos.x >= SCREEN_WIDTH / 2.0f && g_aResultAnn.nCounterState > 0)
		{// 中央に来たら少し止まる
			g_aResultAnn.pos.x = SCREEN_WIDTH / 2.0f;

			g_aResultAnn.nCounterState--;

			g_aResultAnn.bSound = false;
		}
		else if (g_aResultAnn.nCounterState <= 0)
		{// ある程度止まったら右に向かって動く
			g_aResultAnn.pos.x += 15.0f;

			if (g_aResultAnn.bSound == false)
			{// サウンド
				PlaySound(SOUND_LABEL_SE_DORAM);

				g_aResultAnn.bSound = true;
			}
		}

		if (g_aResultAnn.pos.x - g_aResultAnn.fWidth / 2.0f > 2000.0f)
		{// ある程度まで行ったら消す
			g_aResultAnn.bUse = false;
		}

		// 頂点座標の更新
		SetVErtexResultAnn();
	}
}

//-----------------------------------------------------------------
// 結果発表の描画処理
//-----------------------------------------------------------------
void DrawResultAnn(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultAnn, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	if (g_aResultAnn.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureResultAnn);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVErtexResultAnn(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	RESULTANN *pResultAnn = &g_aResultAnn;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultAnn->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_aResultAnn.pos.x - (g_aResultAnn.fWidth / 2), g_aResultAnn.pos.y + (g_aResultAnn.fHeight / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aResultAnn.pos.x - (g_aResultAnn.fWidth / 2), g_aResultAnn.pos.y - (g_aResultAnn.fHeight / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aResultAnn.pos.x + (g_aResultAnn.fWidth / 2), g_aResultAnn.pos.y + (g_aResultAnn.fHeight / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aResultAnn.pos.x + (g_aResultAnn.fWidth / 2), g_aResultAnn.pos.y - (g_aResultAnn.fHeight / 2), 0.0f);

	// 頂点カラーの更新
	pVtx[0].col = g_aResultAnn.col;
	pVtx[1].col = g_aResultAnn.col;
	pVtx[2].col = g_aResultAnn.col;
	pVtx[3].col = g_aResultAnn.col;

	// 頂点バッファをアンロックする
	g_pVtxBuffResultAnn->Unlock();
}

//-----------------------------------------------------------------
// 結果発表の情報の取得
//-----------------------------------------------------------------
bool GetResultAnn(void)
{
	return g_aResultAnn.bUse;
}