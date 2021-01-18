//-----------------------------------------------------------------
//
// カウントダウン (count_down.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "count_down.h"
#include "timer.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define COUNT_DOWN_X		(SCREEN_WIDTH / 2.0f)		// カウントダウンの中心座標(X)
#define COUNT_DOWN_Y		(SCREEN_HEIGHT / 2.0f)		// カウントダウンの中心座標(Y)
#define COUNT_DOWN_WIDTH	(100.0f)						// カウントダウンの幅
#define COUNT_DOWN_HEIGHT	(160.0f)						// カウントダウンの高さ

#define START_WIDTH			(320.0f)
#define START_HEIGHT		(90.0f)

#define MAX_PATTERN			(10)						// アニメーションパターンNo.の最大数
#define TEXTURE_X			(10)						// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y			(1)							// アニメーションパターンNo.の縦の最大数

#define MAX_TEXTURE			(3)							// スコアテクスチャの最大値
#define COUNT_DOWN			(3)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCountDown = NULL;				// カウントダウンのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCountDown[MAX_TEXTURE] = {};		// カウントダウンの頂点バッファのポインタ
D3DXVECTOR3 g_posCountDown;										// カウントダウンの中心座標
int g_nCountDown;												// カウントダウン
int g_nCountDownCnt;											// カウントダウンカウント
D3DXCOLOR g_colCountDown;										// カウントダウンの色
float g_fNumber;												// カウントダウンの数字
COUNTDOWN g_CountDown;											// カウントダウンの情報
bool g_bSound;

//-----------------------------------------------------------------
// カウントダウンの初期化処理
//-----------------------------------------------------------------
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
	g_nCountDown = COUNT_DOWN;										// カウントダウン			
	g_nCountDownCnt = 60;											// カウントダウンカウント
	g_fNumber = 3.0f;
	g_bSound = false;

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

//-----------------------------------------------------------------
// カウントダウンの終了処理
//-----------------------------------------------------------------
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

//-----------------------------------------------------------------
// カウントダウンの更新処理
//-----------------------------------------------------------------
void UpdateCountDown(void)
{
	// 変数宣言
	int nTimer = GetTimer();

	if (g_nCountDownCnt != 0)
	{// サイズカウントの減少
		g_CountDown.fCntSize -= 0.005f;
	}
	if (g_nCountDownCnt == 60)
	{
		// サウンド
		PlaySound(SOUND_LABEL_SE_COUNTDOWN);
	}

	// タイマーカウントの減少
	g_nCountDownCnt--;

	// α値の移動
	g_CountDown.col.a -= 0.02f;

	if (g_nCountDownCnt == 0 && g_nCountDown != 0)
	{
		// サウンド
		PlaySound(SOUND_LABEL_SE_COUNTDOWN);

		// 残り時間を減らす
		g_nCountDown -= 1;

		// 初期化
		g_nCountDownCnt = 60;

		g_fNumber -= 1.0f;

		if (g_nCountDown == 0)
		{// カウントダウンのカウントの初期化
			g_nCountDownCnt = 0;

			// カウントダウンのテクスチャの更新
			g_CountDown.nTExType = 1;

			// サウンド
			PlaySound(SOUND_LABEL_SE_START);

			//	サウンドの設定
			PlaySound(SOUND_LABEL_BATTLE);
		}
		// α値の初期化
		g_CountDown.col.a = 1.0f;

		// サイズカウントの初期化
		g_CountDown.fCntSize = 1.0f;
	}
	if (nTimer == 0)
	{
		// サウンドの一時停止
		StopSound(SOUND_LABEL_BATTLE);

		if (g_bSound == false)
		{
			// サウンド
			PlaySound(SOUND_LABEL_SE_TIMEUP);

			g_bSound = true;
		}

		// カウントダウンのテクスチャの更新
		g_CountDown.nTExType = 2;

		// α値の初期化
		g_CountDown.col.a = 1.0f;

	}

	// 頂点座標の更新
	SetVertexCountDown();
}

//-----------------------------------------------------------------
// カウントダウンの描画処理
//-----------------------------------------------------------------
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

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVertexCountDown(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

	if (g_CountDown.nTExType == 0)
	{// 頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x - (COUNT_DOWN_WIDTH / 2) * g_CountDown.fCntSize, g_CountDown.pos.y + (COUNT_DOWN_HEIGHT / 2) * g_CountDown.fCntSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x - (COUNT_DOWN_WIDTH / 2) * g_CountDown.fCntSize, g_CountDown.pos.y - (COUNT_DOWN_HEIGHT / 2) * g_CountDown.fCntSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x + (COUNT_DOWN_WIDTH / 2) * g_CountDown.fCntSize, g_CountDown.pos.y + (COUNT_DOWN_HEIGHT / 2) * g_CountDown.fCntSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + (COUNT_DOWN_WIDTH / 2) * g_CountDown.fCntSize, g_CountDown.pos.y - (COUNT_DOWN_HEIGHT / 2) * g_CountDown.fCntSize, 0.0f);

		// テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_fNumber * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_fNumber * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_fNumber * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_fNumber * 0.1f, 0.0f);
	}
	else if (g_CountDown.nTExType == 1)
	{
		// 頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(g_CountDown.pos.x - (START_WIDTH / 2), g_CountDown.pos.y + (START_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_CountDown.pos.x - (START_WIDTH / 2), g_CountDown.pos.y - (START_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_CountDown.pos.x + (START_WIDTH / 2), g_CountDown.pos.y + (START_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_CountDown.pos.x + (START_WIDTH / 2), g_CountDown.pos.y - (START_HEIGHT / 2) , 0.0f);

		// テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	// 頂点カラーの更新
	pVtx[0].col = g_CountDown.col;
	pVtx[1].col = g_CountDown.col;
	pVtx[2].col = g_CountDown.col;
	pVtx[3].col = g_CountDown.col;

	// 頂点バッファをアンロックする
	g_pVtxBuffCountDown->Unlock();
}


//-----------------------------------------------------------------
// カウントダウン情報の取得
//-----------------------------------------------------------------
int GetCountDown(void)
{
	return g_nCountDown;
}