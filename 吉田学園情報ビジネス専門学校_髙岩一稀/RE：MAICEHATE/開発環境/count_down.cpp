//==============================================================================================================
//
// カウントダウン (count_down.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "count_down.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define COUNT_DOWN_X		(SCREEN_WIDTH / 2.0f)		// カウントダウンの中心座標(X)
#define COUNT_DOWN_Y		(SCREEN_HEIGHT / 2.0f)		// カウントダウンの中心座標(Y)
#define COUNT_DOWN_WIDTH	(100.0f)						// カウントダウンの幅
#define COUNT_DOWN_HEIGHT	(160.0f)						// カウントダウンの高さ

#define START_WIDTH			(320.0f)
#define START_HEIGHT		(90.0f)

#define MAX_PATTERN			(10)						// アニメーションパターンNo.の最大数
#define TEXTURE_X			(10)						// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y			(1)							// アニメーションパターンNo.の縦の最大数

#define MAX_TEXTURE			(2)							// スコアテクスチャの最大値
#define COUNT_DOWN			(3)

//------------------------------------------------------------------------------------------------------------
// グローバル変数
//------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCountDown = NULL;				// カウントダウンのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCountDown[MAX_TEXTURE] = {};		// カウントダウンの頂点バッファのポインタ
D3DXVECTOR3 g_posCountDown;										// カウントダウンの中心座標
int g_nNumCountDown;											// カウントダウンの数字
int g_nCountDownCnt;											// カウントダウンカウント
D3DXCOLOR g_colCountDown;										// カウントダウンの色
COUNTDOWN g_CountDown;											// カウントダウンの情報
bool g_bFinishCountDown;											// カウントダウンが終わったかどうか

//------------------------------------------------------------------------------------------------------------
// カウントダウンの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//------------------------------------------------------------------------------------------------------------
HRESULT InitCountDown(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number001.png", &g_pTextureCountDown[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/start_logo.png", &g_pTextureCountDown[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/timeup_logo.png", &g_pTextureCountDown[2]);

	// 変数の初期化
	g_CountDown.pos = D3DXVECTOR3(COUNT_DOWN_X, COUNT_DOWN_Y, 0.0f);
	g_CountDown.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			
	g_CountDown.nCntAlufa = 50;
	g_CountDown.fCntSize = 1.0f;
	g_CountDown.nTExType = 0;
	g_CountDown.bUse = true;

	// グローバル変数の初期化
	g_nNumCountDown = COUNT_DOWN;			// カウントダウン			
	g_nCountDownCnt = 60;				// カウントダウンカウント
	g_bFinishCountDown = false;

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEXTURE, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffCountDown,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	SetVertexCountDown();

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点バッファをアンロックする
	g_pVtxBuffCountDown->Unlock();

	return S_OK;
}

//------------------------------------------------------------------------------------------------------------
// カウントダウンの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//------------------------------------------------------------------------------------------------------------
void UninitCountDown(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffCountDown != NULL)
	{
		g_pVtxBuffCountDown->Release();
		g_pVtxBuffCountDown = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTextureCountDown[nCnt] != NULL)
		{
			g_pTextureCountDown[nCnt]->Release();
			g_pTextureCountDown[nCnt] = NULL;
		}
	}
}

//------------------------------------------------------------------------------------------------------------
// カウントダウンの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//------------------------------------------------------------------------------------------------------------
void UpdateCountDown(void)
{
	// カウントダウンの数字処理
	CountDownNumber();

	// 頂点座標の更新
	SetVertexCountDown();
}

//------------------------------------------------------------------------------------------------------------
// カウントダウンの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//------------------------------------------------------------------------------------------------------------
void DrawCountDown(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCountDown, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定と描画
	if (g_CountDown.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureCountDown[g_CountDown.nTExType]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//------------------------------------------------------------------------------------------------------------
// カウントダウンの数字処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//------------------------------------------------------------------------------------------------------------
void CountDownNumber(void)
{
	if (g_nCountDownCnt == 60)
	{
		// サウンド
		PlaySound(SOUND_LABEL_SE_COUNTDOWN);
	}

	// タイマーカウントの減少
	g_nCountDownCnt--;

	// α値の移動
	g_CountDown.col.a -= 0.02f;

	if (g_nCountDownCnt == 0 && g_nNumCountDown != 0)
	{
		// 残り時間を減らす
		g_nNumCountDown -= 1;

		// 初期化
		g_nCountDownCnt = 60;

		// カウントダウンのスタート処理
		CountDownStart();

		// α値の初期化
		g_CountDown.col.a = 1.0f;
	}
}

//------------------------------------------------------------------------------------------------------------
// カウントダウンのスタート処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//------------------------------------------------------------------------------------------------------------
void CountDownStart(void)
{
	if (g_nNumCountDown == 0)
	{// カウントダウンのカウントの初期化
		g_nCountDownCnt = 0;

		// カウントダウンのテクスチャの更新
		g_CountDown.nTExType = 1;

		// サウンド
		PlaySound(SOUND_LABEL_SE_START);

		// カウントダウンを終えたことを伝える
		g_bFinishCountDown = true;
	}
}

//------------------------------------------------------------------------------------------------------------
// 頂点座標の設定
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//------------------------------------------------------------------------------------------------------------
void SetVertexCountDown(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

	if (g_CountDown.nTExType == 0)
	{
		// 数字の頂点座標の設定
		SetVertexCountDownNumber(pVtx);
	}
	else if (g_CountDown.nTExType == 1)
	{
		// スタートの頂点座標の設定
		SetVertexCountDownStart(pVtx);
	}

	// 頂点カラーの更新
	pVtx[0].col = g_CountDown.col;
	pVtx[1].col = g_CountDown.col;
	pVtx[2].col = g_CountDown.col;
	pVtx[3].col = g_CountDown.col;

	// 頂点バッファをアンロックする
	g_pVtxBuffCountDown->Unlock();
}

//------------------------------------------------------------------------------------------------------------
// カウントダウンの数字の頂点座標の設定
// 引数		：*pVtx	- 2D頂点情報(VERTEX_2D)のポインタ
// 返り値	：void	- 何も返さない
//------------------------------------------------------------------------------------------------------------
void SetVertexCountDownNumber(VERTEX_2D *pVtx)
{
	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x - (COUNT_DOWN_WIDTH / 2), g_CountDown.pos.y + (COUNT_DOWN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x - (COUNT_DOWN_WIDTH / 2), g_CountDown.pos.y - (COUNT_DOWN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x + (COUNT_DOWN_WIDTH / 2), g_CountDown.pos.y + (COUNT_DOWN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + (COUNT_DOWN_WIDTH / 2), g_CountDown.pos.y - (COUNT_DOWN_HEIGHT / 2), 0.0f);

	// テクスチャの更新
	pVtx[0].tex = D3DXVECTOR2(0.0f + (float)g_nNumCountDown * 0.1f, 1.0f / TEXTURE_Y);
	pVtx[1].tex = D3DXVECTOR2(0.0f + (float)g_nNumCountDown * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + (float)g_nNumCountDown * 0.1f, 1.0f / TEXTURE_Y);
	pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + (float)g_nNumCountDown * 0.1f, 0.0f);
}

//------------------------------------------------------------------------------------------------------------
// カウントダウンのスタートの頂点座標の設定
// 引数		：*pVtx	- 2D頂点情報(VERTEX_2D)のポインタ
// 返り値	：void	- 何も返さない
//------------------------------------------------------------------------------------------------------------
void SetVertexCountDownStart(VERTEX_2D *pVtx)
{
	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x - (START_WIDTH / 2), g_CountDown.pos.y + (START_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x - (START_WIDTH / 2), g_CountDown.pos.y - (START_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x + (START_WIDTH / 2), g_CountDown.pos.y + (START_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + (START_WIDTH / 2), g_CountDown.pos.y - (START_HEIGHT / 2), 0.0f);

	// テクスチャの更新
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
}

//------------------------------------------------------------------------------------------------------------
// カウントダウンが終わったかどうか取得
// 引数		：void	- 何もなし
// 返り値	：bool	- カウントダウンが終わったかどうか
//------------------------------------------------------------------------------------------------------------
bool GetFinishCountDown(void)
{
	return g_bFinishCountDown;
}