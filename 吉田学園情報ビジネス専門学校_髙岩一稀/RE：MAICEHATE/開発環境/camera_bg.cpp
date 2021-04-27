//==============================================================================================================
//
// カメラ背景 (camera_bg.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "camera_bg.h"
#include "camera.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define POS_ADJUST (10.0f)			// 座標調整用

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCameraBg = NULL;		// カメラ背景の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCameraBg = NULL;			// カメラ背景のテクスチャのポインタ

//--------------------------------------------------------------------------------------------------------------
// カメラ背景の初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitCameraBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();
	Camera pCamera = GetLookDownCamera();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/camera_bg.png", &g_pTextureCameraBg);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffCameraBg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCameraBg->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3((float)pCamera.viewport.X, (float)pCamera.viewport.Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)(pCamera.viewport.X + pCamera.viewport.Width), (float)pCamera.viewport.Y , 0.0f);
	pVtx[2].pos = D3DXVECTOR3((float)pCamera.viewport.X, (float)(pCamera.viewport.Y + pCamera.viewport.Height), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)(pCamera.viewport.X + pCamera.viewport.Width), (float)(pCamera.viewport.Y + pCamera.viewport.Height), 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffCameraBg->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// カメラ背景の終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitCameraBg(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffCameraBg != NULL)
	{
		g_pVtxBuffCameraBg->Release();
		g_pVtxBuffCameraBg = NULL;
	}

	// テクスチャの開放
	if (g_pTextureCameraBg != NULL)
	{
		g_pTextureCameraBg->Release();
		g_pTextureCameraBg = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// カメラ背景の更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateCameraBg(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// カメラ背景の描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawCameraBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCameraBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTextureCameraBg);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}