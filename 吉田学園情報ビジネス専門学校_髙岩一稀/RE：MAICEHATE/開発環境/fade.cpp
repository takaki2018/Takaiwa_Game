//==============================================================================================================
//
// フェード (fade.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "fade.h"
#include "game.h"
#include "input.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define REMOVE_ALFHA		(0.02f)				// α値の増減量

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;		// テクスチャのポインタ
FADE g_fade;									// フェードの状態
MODE g_modeNext;								// 次のモード
D3DXCOLOR g_colorFade;							// フェード色

//--------------------------------------------------------------------------------------------------------------
// フェードの初期化処理
// 引数		：modeNext	- 次のモード
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitFade(MODE modeNext)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// フェードの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitFade(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}

	// テクスチャの開放
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// フェードの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateFade(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	// フェード情報の分岐
	if (g_fade == FADE_IN)
	{// フェードインの時
		g_colorFade.a -= REMOVE_ALFHA;

		if (g_colorFade.a <= 0.0f)
		{// 0.0fになったとき何もしない
			g_fade = FADE_NONE;
		}
	}
	else if (g_fade == FADE_OUT)
	{// フェードアウトのとき
		g_colorFade.a += REMOVE_ALFHA;

		if (g_colorFade.a >= 1.0f)
		{// 1.0fになったときフェードイン
			g_fade = FADE_IN;

			SetPause(false);
			SetMode(g_modeNext);
		}
	}
	else if (g_fade == FADE_NONE)
	{// 何もしていないとき
		g_colorFade.a = 0.0f;
	}
}

//--------------------------------------------------------------------------------------------------------------
// フェードの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawFade(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTextureFade);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//--------------------------------------------------------------------------------------------------------------
// フェード設定
// 引数		：fade		- フェード情報
//			：modeNext	- 次のモード
// 返り値	：void		- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetFade(FADE fade, MODE modeNext)
{
	g_fade = fade;
	g_modeNext = modeNext;
}

//--------------------------------------------------------------------------------------------------------------
// フェード情報の取得
// 引数		：void	- 何もなし
// 返り値	：FADE	- フェード情報を渡す
//--------------------------------------------------------------------------------------------------------------
FADE GetFade(void)
{
	return g_fade;
}