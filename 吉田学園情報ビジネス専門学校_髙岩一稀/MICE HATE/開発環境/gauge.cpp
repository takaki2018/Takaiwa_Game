//-----------------------------------------------------------------
//
// ゲージ (gauge.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "gauge.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN		(2)				// 種類

#define GAUGE_CENTER_X	(135.0f)		// ゲージの中心座標(X)
#define GAUGE_CENTER_y	(75.0f)			// ゲージの中心座標(Y)

#define GAUGE_X			(250.0f)		// ゲージの幅
#define GAUGE_Y			(30.0f)			// ゲージの高さ

#define GAUGE_GREEN_X	(242.0f)		// 緑ゲージの幅
#define GAUGE_GREEN_Y	(24.0f)			// 緑ゲージの高さ

#define REMOVE			(40.0f)			// 減少量

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;			// ゲージの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureGauge[MAX_PATTERN] = {};	// ゲージのテクスチャのポインタ
D3DXVECTOR3 g_posGauge;									// ゲージの中心座標
float g_fRemove;										// ゲージの減少量
bool g_bRemove;											// ゲージが減少しきったかどうか

//-----------------------------------------------------------------
// ゲージの初期化処理
//-----------------------------------------------------------------
HRESULT InitGauge(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posGauge = D3DXVECTOR3(GAUGE_CENTER_X, GAUGE_CENTER_y, 0.0f);
	g_fRemove = 0.0f;
	g_bRemove = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge.png", &g_pTextureGauge[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge100.png", &g_pTextureGauge[1]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		// 頂点座標の設定
		switch (nCnt)
		{// ゲージ枠
		case 0:
			pVtx[0].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_X / 2, g_posGauge.y + GAUGE_Y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_X / 2, g_posGauge.y - GAUGE_Y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_X / 2, g_posGauge.y + GAUGE_Y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_X / 2, g_posGauge.y - GAUGE_Y / 2, 0.0f);
			break;
			// ゲージ
		case 1:
			pVtx[0].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_GREEN_X / 2, g_posGauge.y + GAUGE_GREEN_Y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_GREEN_X / 2, g_posGauge.y - GAUGE_GREEN_Y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_GREEN_X / 2, g_posGauge.y + GAUGE_GREEN_Y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_GREEN_X / 2, g_posGauge.y - GAUGE_GREEN_Y / 2, 0.0f);
			break;

		default:
			break;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// ゲージの終了処理
//-----------------------------------------------------------------
void UninitGauge(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		if (g_pTextureGauge[nCnt] != NULL)
		{
			g_pTextureGauge[nCnt]->Release();
			g_pTextureGauge[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ゲージの更新処理
//-----------------------------------------------------------------
void UpdateGauge(void)
{
	// 弾を撃った時
	if (g_bRemove == false)
	{
		if (GetKeyboardTrigger(KEYINFO_SHOOT) == true || GetJoypadTrigger(JPINFO_SHOOT) == true)
		{// 減少値を増やす
			g_fRemove += REMOVE;

			if (g_fRemove >= GAUGE_GREEN_X)
			{// 端まで来たとき
				g_fRemove = GAUGE_GREEN_X;
				g_bRemove = true;
			}
		}
	}
	// 減少値を減らす
	g_fRemove -= 0.8f;

	if (g_fRemove <= 0.0f)
	{// 減少しなくする
		g_fRemove = 0.0f;
		g_bRemove = false;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[4].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_GREEN_X / 2, g_posGauge.y + GAUGE_GREEN_Y / 2, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posGauge.x - GAUGE_GREEN_X / 2, g_posGauge.y - GAUGE_GREEN_Y / 2, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_GREEN_X / 2 - g_fRemove, g_posGauge.y + GAUGE_GREEN_Y / 2, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posGauge.x + GAUGE_GREEN_X / 2 - g_fRemove, g_posGauge.y - GAUGE_GREEN_Y / 2, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

//-----------------------------------------------------------------
// ゲージの描画処理
//-----------------------------------------------------------------
void DrawGauge(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureGauge[nCnt]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//-----------------------------------------------------------------
// 減少値の取得
//-----------------------------------------------------------------
bool GetRemove(void)
{
	return g_bRemove;
}