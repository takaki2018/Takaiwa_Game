//==============================================================================================================
//
// タイトルロゴ (titlelogo.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "titlelogo.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define TITLELOGO_X			(SCREEN_WIDTH / 2)	// タイトルロゴのX座標
#define TITLELOGO_Y			(-100.0f)			// タイトルロゴの初期中心Y座標
#define TITLELOGO_WIDTH		(800.0f)			// タイトルロゴ幅
#define TITLELOGO_HEIGHT	(200.0f)			// タイトルロゴの高さ

#define TITLELOGO_Y_DEST	(250.0f)			// 目的の高さ
#define MOVE_TITLELOGO		(5.0f)				// タイトルロゴの移動量

//--------------------------------------------------------------------------------------------------------------
// 構造体の定義
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float fWidth;			// 幅
	float fHieght;			// 高さ
	bool bFall;				// 落ち切ったかどうか
}TitleLogo;

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;		// タイトルロゴの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;			// タイトルロゴのテクスチャのポインタ
TitleLogo g_TitleLogo;									// タイトルロゴ情報

//--------------------------------------------------------------------------------------------------------------
// タイトルロゴの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTitlelogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// タイトルロゴ情報の初期化
	g_TitleLogo.pos = D3DXVECTOR3(TITLELOGO_X, TITLELOGO_Y, 0.0f);
	g_TitleLogo.fWidth = TITLELOGO_WIDTH / 2.0f;
	g_TitleLogo.fHieght = TITLELOGO_HEIGHT / 2.0f;
	g_TitleLogo.bFall = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo.png", &g_pTextureTitleLogo);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetVertexTitleLogo();

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// タイトルロゴの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitTitlelogo(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}

	// テクスチャの開放
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// タイトルロゴの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateTitlelogo(void)
{
	// タイトルロゴが落ちきる前の処理
	FallBefore();

	// 頂点座標の設定
	SetVertexTitleLogo();
}

//--------------------------------------------------------------------------------------------------------------
// タイトルロゴの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawTitlelogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTextureTitleLogo);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//--------------------------------------------------------------------------------------------------------------
// タイトルロゴが落ちきる前の処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void FallBefore(void)
{
	// フェード情報の取得
	int nFade = GetFade();

	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true)
	{// エンターキーを押したとき
		if (nFade == FADE_IN)
		{// フェードインの時
			SetFade(FADE_NONE, MODE_TITLE);
			g_TitleLogo.pos.y = TITLELOGO_Y_DEST;
			g_TitleLogo.bFall = true;
		}
		else if (g_TitleLogo.pos.y <= TITLELOGO_Y_DEST)
		{// タイトルロゴが落ちてきているとき
			g_TitleLogo.pos.y = TITLELOGO_Y_DEST;
			g_TitleLogo.bFall = true;
		}
	}
	// 更新処理
	if (nFade == FADE_NONE && g_TitleLogo.pos.y != TITLELOGO_Y_DEST)
	{// タイトルロゴの移動量の更新
		g_TitleLogo.pos.y += MOVE_TITLELOGO;
	}
	else if (g_TitleLogo.pos.y >= TITLELOGO_Y_DEST)
	{// タイトルロゴが落ちてきているとき
		g_TitleLogo.pos.y = TITLELOGO_Y_DEST;
		g_TitleLogo.bFall = true;
	}
}

//--------------------------------------------------------------------------------------------------------------
// タイトルロゴの頂点座標の設定
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetVertexTitleLogo(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_TitleLogo.pos.x - g_TitleLogo.fWidth, g_TitleLogo.pos.y + g_TitleLogo.fHieght, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_TitleLogo.pos.x - g_TitleLogo.fWidth, g_TitleLogo.pos.y - g_TitleLogo.fHieght, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_TitleLogo.pos.x + g_TitleLogo.fWidth, g_TitleLogo.pos.y + g_TitleLogo.fHieght, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_TitleLogo.pos.x + g_TitleLogo.fWidth, g_TitleLogo.pos.y - g_TitleLogo.fHieght, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// タイトルロゴ情報の取得
// 引数		：void	- 何もなし
// 返り値	：bool	- タイトルロゴが目的の場所まで落ちたかどうか返す
//--------------------------------------------------------------------------------------------------------------
bool GetbFall(void)
{
	return g_TitleLogo.bFall;
}