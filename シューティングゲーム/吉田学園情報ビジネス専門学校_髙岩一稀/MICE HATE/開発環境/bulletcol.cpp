//-----------------------------------------------------------------
//
// 弾の色確認 (bulletcol.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bulletcol.h"
#include "bullet.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define BULLETCOL_X			(300.0f)				// 弾の色確認用のテクスチャの中心座標(X)
#define BULLETCOL_Y			(75.0f)					// 弾の色確認用のテクスチャの中心座標(Y)

#define BULLETCOL_SIZE		(15.0f)					// 弾の色確認用のテクスチャのサイズ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletcol = NULL;	// 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBulletcol = NULL;		// テクスチャのポインタ
D3DXVECTOR3 g_posBulletcol;							// 中心座標
int g_nCntCol;										// 弾の色カウント
D3DXCOLOR g_Bulletcol;								// 色の設定

//-----------------------------------------------------------------
// 弾の色確認の初期化処理
//-----------------------------------------------------------------
HRESULT InitBulletcol(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posBulletcol = D3DXVECTOR3(BULLETCOL_X, BULLETCOL_Y,0.0f );
	g_nCntCol = 0;
	g_Bulletcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBulletcol);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletcol,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletcol->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posBulletcol.x - BULLETCOL_SIZE, g_posBulletcol.y + BULLETCOL_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posBulletcol.x - BULLETCOL_SIZE, g_posBulletcol.y - BULLETCOL_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posBulletcol.x + BULLETCOL_SIZE, g_posBulletcol.y + BULLETCOL_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posBulletcol.x + BULLETCOL_SIZE, g_posBulletcol.y - BULLETCOL_SIZE, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = g_Bulletcol;
	pVtx[1].col = g_Bulletcol;
	pVtx[2].col = g_Bulletcol;
	pVtx[3].col = g_Bulletcol;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBulletcol->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 弾の色確認の終了処理
//-----------------------------------------------------------------
void UninitBulletcol(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBulletcol != NULL)
	{
		g_pVtxBuffBulletcol->Release();
		g_pVtxBuffBulletcol = NULL;
	}

	// テクスチャの開放
	if (g_pTextureBulletcol != NULL)
	{
		g_pTextureBulletcol->Release();
		g_pTextureBulletcol = NULL;
	}
}

//-----------------------------------------------------------------
// 弾の色確認の更新処理
//-----------------------------------------------------------------
void UpdateBulletcol(void)
{
	// 弾の色の切り替え
	if (GetKeyboardTrigger(KEYINFO_COLOR_CHANGE) == true || GetJoypadTrigger(JPINFO_COLOR_CHANGE) == true)
	{
		g_nCntCol = (g_nCntCol + 1) % BULLETCOLOR_MAX;
	}

	// 色の確定
	if (g_nCntCol == BULLETCOLOR_WHITE)
	{// 白に設定
		g_Bulletcol = D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.9f);
	}
	else if (g_nCntCol == BULLETCOLOR_RED)
	{// 赤に設定
		g_Bulletcol = D3DXCOLOR(0.9f, 0.3f, 0.1f, 0.9f);
	}
	else if (g_nCntCol == BULLETCOLOR_BLUE)
	{// 青に設定
		g_Bulletcol = D3DXCOLOR(0.1f, 0.3f, 0.9f, 0.9f);
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletcol->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = g_Bulletcol;
	pVtx[1].col = g_Bulletcol;
	pVtx[2].col = g_Bulletcol;
	pVtx[3].col = g_Bulletcol;

	// 頂点バッファをアンロックする
	g_pVtxBuffBulletcol->Unlock();
}

//-----------------------------------------------------------------
// 弾の色確認の描画処理
//-----------------------------------------------------------------
void DrawBulletcol(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBulletcol, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTextureBulletcol);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}