//==============================================================================================================
//
// ロード画面 (loadscreen.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial_bg.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_TUTORIALBG		(1)					// チュートリアル背景の最大数		

#define TUTORIALBG_X		(SCREEN_WIDTH / 2)	// チュートリアル背景のX座標
#define TUTORIALBG_Y		(SCREEN_HEIGHT / 2)	// チュートリアル背景のY座標
#define TUTORIALBG_WIDTH	(SCREEN_WIDTH)		// チュートリアル背景の幅
#define TUTORIALBG_HIEGHT	(SCREEN_HEIGHT)		// チュートリアル背景の高さ

#define MOVE_TUTORISLBG_UV	(0.0002f)			// チュートリアル背景の移動量

//--------------------------------------------------------------------------------------------------------------
// 構造体の定義
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float fWidth;			// 幅
	float fHieght;			// 高さ
}TutorialBg;

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialBg = NULL;			// チュートリアル背景の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTutorialBg[MAX_TUTORIALBG] = {};	// チュートリアル背景のテクスチャのポインタ
TutorialBg g_TutorialBg;										// チュートリアル背景情報
float g_fMoveTutorialBgUV;										// チュートリアル背景のUV値の移動量

//--------------------------------------------------------------------------------------------------------------
// チュートリアル背景の初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// チュートリアル背景情報の初期化
	g_TutorialBg.pos = D3DXVECTOR3(TUTORIALBG_X, TUTORIALBG_Y,0.0f);
	g_TutorialBg.fWidth = TUTORIALBG_WIDTH / 2.0f;
	g_TutorialBg.fHieght = TUTORIALBG_HIEGHT / 2.0f;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/loadbg.png", &g_pTextureTutorialBg[0]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TUTORIALBG, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialBg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TUTORIALBG; nCnt++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_TutorialBg.pos.x - g_TutorialBg.fWidth, g_TutorialBg.pos.y + g_TutorialBg.fHieght, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TutorialBg.pos.x - g_TutorialBg.fWidth, g_TutorialBg.pos.y - g_TutorialBg.fHieght, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TutorialBg.pos.x + g_TutorialBg.fWidth, g_TutorialBg.pos.y + g_TutorialBg.fHieght, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TutorialBg.pos.x + g_TutorialBg.fWidth, g_TutorialBg.pos.y - g_TutorialBg.fHieght, 0.0f);

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

		// テクスチャ座標の設定
		SetTexUVTutorialBg();

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTutorialBg->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル背景の終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitTutorialBg(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTutorialBg != NULL)
	{
		g_pVtxBuffTutorialBg->Release();
		g_pVtxBuffTutorialBg = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TUTORIALBG; nCntTexture++)
	{
		if (g_pTextureTutorialBg[nCntTexture] != NULL)
		{
			g_pTextureTutorialBg[nCntTexture]->Release();
			g_pTextureTutorialBg[nCntTexture] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル背景の更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorialBg(void)
{
	// 変数を減算
	g_fMoveTutorialBgUV -= MOVE_TUTORISLBG_UV;

	// 1.0fを超えたとき処理
	if (g_fMoveTutorialBgUV >= 1.0f)
	{
		g_fMoveTutorialBgUV = 0.0f;
	}
	// テクスチャ座標の設定
	SetTexUVTutorialBg();
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル背景の描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawTutorialBg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_TUTORIALBG; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureTutorialBg[nCnt]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル背景のテクスチャ座標の設定
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetTexUVTutorialBg(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialBg->Lock(0, 0, (void**)&pVtx, 0);

	// 背景の更新
	pVtx[0].tex = D3DXVECTOR2(0.0f - g_fMoveTutorialBgUV, 1.0f + g_fMoveTutorialBgUV);
	pVtx[1].tex = D3DXVECTOR2(0.0f - g_fMoveTutorialBgUV, 0.0f + g_fMoveTutorialBgUV);
	pVtx[2].tex = D3DXVECTOR2(1.0f - g_fMoveTutorialBgUV, 1.0f + g_fMoveTutorialBgUV);
	pVtx[3].tex = D3DXVECTOR2(1.0f - g_fMoveTutorialBgUV, 0.0f + g_fMoveTutorialBgUV);

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorialBg->Unlock();
}