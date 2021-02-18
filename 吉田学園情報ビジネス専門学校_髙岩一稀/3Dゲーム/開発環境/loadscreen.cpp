//-----------------------------------------------------------------
//
// ロード画面 (loadscreen.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "loadscreen.h"
#include "input.h"
#include "gamepad.h"
#include "selectmode.h"
#include "fade.h"
#include "sound.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN     (1)							// 使うテクスチャの最大数
#define MAX_TEXTURE		(1)
#define MAX_BG			(1)							// 背景の最大数
#define MAX_SELECT		(4)							// マーカーの最大値
#define MAX_IMAGE		(4)							// 画像の最大数

#define MOVE_ALUFA		(0.02f)						// α値の移動量
#define MOVE_BG			(0.002f)					// ロード画面の移動量

#define IMAGE_X			(SCREEN_WIDTH / 2)			// 画像の中心座標(X)
#define IMAGE_Y			(340.0f)					// 画像の中心座標(Y)
#define IMAGE_WIDTH 	(SCREEN_WIDTH / 4 * 3)		// 画像の幅
#define IMAGE_HEIGHT	(SCREEN_HEIGHT / 4 * 3)		// 画像の高さ

#define SELECT_X		(560.0f)					// マーカーの中心座標(X)
#define SELECT_Y		(660.0f)					// マーカーの中心座標(Y)
#define SELECT_SIZE 	(20.0f)						// マーカーのサイズ
#define SELECT_WIDTH	(50.0f)						// マーカーの間隔

#define PRESSOK_X		(1050.0f)					// プレスOKの中心座標(X)
#define PRESSOK_Y		(660.0f)					// プレスOKの中心座標(Y)
#define PRESSOK_WIDTH 	(300.0f)					// プレスOKの幅
#define PRESSOK_HEIGHT	(50.0f)						// プレスOKの高さ

#define FADE_INTERBAL	(60)						// フェードのインターバル
#define ALUFA_INTERBAL	(10)						// 点滅のインターバルインターバル

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLoadScreen = NULL;			// ロード画面の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureLoadScreen[MAX_TEXTURE] = {};		// ロード画面のテクスチャのポインタ
float g_colPressOK;												// プレスOKの点滅用		
float g_fmoveAlufa;												// α値の移動
int g_nCntSelect;												// 何個目か識別
int g_nCntImage;												// 何番目の画像を出すか
bool bPressOK;													// 決定したかどうか
int g_fadeCnt;													// フェードカウント
int g_PressOKFlashCnt;											// プレスOKの点滅カウント

//-----------------------------------------------------------------
// ロード画面の初期化処理
//-----------------------------------------------------------------
HRESULT InitLoadScreen(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_colPressOK = 0.0f;
	g_fmoveAlufa = MOVE_ALUFA;
	g_nCntSelect = 0;
	g_nCntImage = 4;
	bPressOK = false;
	g_fadeCnt = 0;
	g_PressOKFlashCnt = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.jpg", &g_pTextureLoadScreen[0]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffLoadScreen,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		// 頂点座標の設定
		SetVertexLoadScreen(nCnt);

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

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffLoadScreen->Unlock();

	// サウンド
	PlaySound(SOUND_LABEL_OE);

	return S_OK;
}

//-----------------------------------------------------------------
// ロード画面の終了処理
//-----------------------------------------------------------------
void UninitLoadScreen(void)
{
	// サウンドの一時停止
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffLoadScreen != NULL)
	{
		g_pVtxBuffLoadScreen->Release();
		g_pVtxBuffLoadScreen = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		if (g_pTextureLoadScreen[nCnt] != NULL)
		{
			g_pTextureLoadScreen[nCnt]->Release();
			g_pTextureLoadScreen[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ロード画面の更新処理
//-----------------------------------------------------------------
void UpdateLoadScreen(void)
{
	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(PLAYER_1,JPINFO_JUMP) == true)
	{
		// 画面モードの初期化処理
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//-----------------------------------------------------------------
// ロード画面の描画処理
//-----------------------------------------------------------------
void DrawLoadScreen(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLoadScreen, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureLoadScreen[nCnt]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
} 

//-----------------------------------------------------------------
// ロード画面の頂点座標の設定
//-----------------------------------------------------------------
void SetVertexLoadScreen(int nCnt)
{
	if (nCnt < MAX_BG)
	{// ロード画面の背景の設定
		LoadScreenBg(nCnt);
	}
}

//-----------------------------------------------------------------
// ロード画面の背景
//-----------------------------------------------------------------
void LoadScreenBg(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か確定
	pVtx += nCnt * 4;

	// 背景の更新
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffLoadScreen->Unlock();
}

//-----------------------------------------------------------------
// ロード画面の画像
//-----------------------------------------------------------------
void LoadScreenImage(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か確定
	pVtx += nCnt * 4;

	// 背景の更新
	pVtx[0].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2,0.0f);
	pVtx[1].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2,0.0f);
	pVtx[2].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2,0.0f);
	pVtx[3].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2,0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffLoadScreen->Unlock();
}

//-----------------------------------------------------------------
// ロード画面のマーカー
//-----------------------------------------------------------------
void LoadScreenSelect(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か確定
	pVtx += nCnt * 4;

	// 間隔の確定
	float nWidth = ((nCnt - 3) % MAX_SELECT) * SELECT_WIDTH;
	
	// 背景の更新
	pVtx[0].pos = D3DXVECTOR3(SELECT_X - SELECT_SIZE / 2 + nWidth, SELECT_Y + SELECT_SIZE / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SELECT_X - SELECT_SIZE / 2 + nWidth, SELECT_Y - SELECT_SIZE / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SELECT_X + SELECT_SIZE / 2 + nWidth, SELECT_Y + SELECT_SIZE / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SELECT_X + SELECT_SIZE / 2 + nWidth, SELECT_Y - SELECT_SIZE / 2, 0.0f);

	// 頂点カラーの設定
	if ((nCnt - 3) == g_nCntSelect)
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	}


	// 頂点バッファをアンロックする
	g_pVtxBuffLoadScreen->Unlock();
}

//-----------------------------------------------------------------
// ロード画面のプレスOK
//-----------------------------------------------------------------
void LoadScreenPressOK(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLoadScreen->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か確定
	pVtx += nCnt * 4;

	// 背景の更新
	pVtx[0].pos = D3DXVECTOR3(PRESSOK_X - PRESSOK_WIDTH / 2 , PRESSOK_Y + PRESSOK_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PRESSOK_X - PRESSOK_WIDTH / 2 , PRESSOK_Y - PRESSOK_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PRESSOK_X + PRESSOK_WIDTH / 2 , PRESSOK_Y + PRESSOK_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PRESSOK_X + PRESSOK_WIDTH / 2 , PRESSOK_Y - PRESSOK_HEIGHT / 2, 0.0f);

	// 点滅
	if (bPressOK == false)
	{
		if (3 == g_nCntSelect)
		{// 最後のページにいったとき
			g_colPressOK += g_fmoveAlufa;

			if (g_colPressOK <= 0.0f || g_colPressOK >= 1.0f)
			{// 0.0f以下または1.0f以上になったとき-1をかける
				g_fmoveAlufa *= -1;
			}
		}
		else if (3 != g_nCntSelect)
		{// 最後のページにいっていないとき
			g_colPressOK = 0.0f;
		}
	}
	else if (bPressOK == true)
	{// プレスOKを押したとき点滅
		g_PressOKFlashCnt++;

		if (0 == g_PressOKFlashCnt % ALUFA_INTERBAL)
		{
			g_colPressOK = 1.0f;
		}
		else if ((ALUFA_INTERBAL / 2) == g_PressOKFlashCnt % ALUFA_INTERBAL)
		{
			g_colPressOK = 0.2f;
		}
	}
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colPressOK);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colPressOK);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colPressOK);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colPressOK);

	// 頂点バッファをアンロックする
	g_pVtxBuffLoadScreen->Unlock();
}