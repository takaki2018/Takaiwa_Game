//-----------------------------------------------------------------
//
// ポーズOE (pause_oe.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "pause_oe.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN     (4)			// 使うテクスチャの最大数
#define MAX_TEXTURE		(4)
#define MAX_SELECT		(4)			// マーカーの最大値

#define IMAGE_X			(850.0f)	// 画像の中心座標(X)
#define IMAGE_Y			(390.0f)	// 画像の中心座標(Y)
#define IMAGE_WIDTH 	(768.0f)	// 画像の幅
#define IMAGE_HEIGHT	(432.0f)	// 画像の高さ

#define SELECT_X		(800.0f)	// マーカーの中心座標(X)
#define SELECT_Y		(660.0f)	// マーカーの中心座標(Y)
#define SELECT_SIZE 	(20.0f)		// マーカーのサイズ
#define SELECT_WIDTH	(50.0f)		// マーカーの間隔

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseOe = NULL;			// ロード画面の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePauseOe[MAX_TEXTURE] = {};		// ロード画面のテクスチャのポインタ
int g_nCountSelect;											// 何個目か識別
int g_nCountImage;											// 何番目の画像を出すか

//-----------------------------------------------------------------
// ポーズOEの初期化処理
//-----------------------------------------------------------------
HRESULT InitPauseOe(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_nCountSelect = 0;
	g_nCountImage = 1;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTexturePauseOe[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe001.png", &g_pTexturePauseOe[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe002.png", &g_pTexturePauseOe[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_se.png", &g_pTexturePauseOe[3]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseOe,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPauseOe->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		// 頂点座標の設定
		SetVertexPauseOe(nCnt);

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
	g_pVtxBuffPauseOe->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// ポーズOEの終了処理
//-----------------------------------------------------------------
void UninitPauseOe(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffPauseOe != NULL)
	{
		g_pVtxBuffPauseOe->Release();
		g_pVtxBuffPauseOe = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTexturePauseOe[nCnt] != NULL)
		{
			g_pTexturePauseOe[nCnt]->Release();
			g_pTexturePauseOe[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ポーズOEの更新処理
//-----------------------------------------------------------------
void UpdatePauseOe(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	bool *pStick = GetControllerTrigger();

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	// ポーズメニューを開いたとき処理
	if (GetKeyboardTrigger(KEYINFO_PAUSE) == true || GetJoypadTrigger(JPINFO_PAUSE) == true)
	{
		g_nCountSelect = 0;
		g_nCountImage = 1;
	}
	// マーカーの更新
	if (GetKeyboardTrigger(KEYINFO_RIGHT) == true)
	{
		if (g_nCountSelect != 2)
		{
			g_nCountSelect++;		// マーカーの移動値の増加
			g_nCountImage++;		// 画像を次のものに移す

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	else if (GetKeyboardTrigger(KEYINFO_LEFT) == true)
	{// 左に移動するとき
		if (g_nCountSelect != 0)
		{
			g_nCountSelect--;		// マーカーの移動値の減少
			g_nCountImage--;		// 画像を前のものに移す

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	// JOYPAD の操作
	if (pStick[2] == true)
	{
		if (g_nCountSelect != 2)
		{
			g_nCountSelect++;		// マーカーの移動値の増加
			g_nCountImage++;		// 画像を次のものに移す

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	else if (pStick[3] == true)
	{// 左に移動するとき
		if (g_nCountSelect != 0)
		{
			g_nCountSelect--;		// マーカーの移動値の減少
			g_nCountImage--;		// 画像を前のものに移す

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	// 頂点座標の更新
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		SetVertexPauseOe(nCnt);
	}
}

//-----------------------------------------------------------------
// ポーズOEの描画処理
//-----------------------------------------------------------------
void DrawPauseOe(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPauseOe, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		if (nCnt <= 2)
		{// マーカー
			pDevice->SetTexture(0, g_pTexturePauseOe[0]);
		}
		else if (nCnt == 3)
		{// 画像
			pDevice->SetTexture(0, g_pTexturePauseOe[g_nCountImage]);
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//-----------------------------------------------------------------
// ポーズOEの頂点座標の設定
//-----------------------------------------------------------------
void SetVertexPauseOe(int nCnt)
{
	if (nCnt <= 2)
	{// マーカーの設定
		PauseOeSelect(nCnt);
	}
	else if (nCnt == 3)
	{// 画像の設定
		PauseOeImage(nCnt);
	}
}

//-----------------------------------------------------------------
// ポーズOEの画像
//-----------------------------------------------------------------
void PauseOeImage(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPauseOe->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か確定
	pVtx += nCnt * 4;

	// 背景の更新
	pVtx[0].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPauseOe->Unlock();
}

//-----------------------------------------------------------------
// ポーズOEのマーカー
//-----------------------------------------------------------------
void PauseOeSelect(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPauseOe->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か確定
	pVtx += nCnt * 4;

	// 間隔の確定
	float nWidth = (nCnt % MAX_SELECT) * SELECT_WIDTH;

	// 背景の更新
	pVtx[0].pos = D3DXVECTOR3(SELECT_X - SELECT_SIZE / 2 + nWidth, SELECT_Y + SELECT_SIZE / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SELECT_X - SELECT_SIZE / 2 + nWidth, SELECT_Y - SELECT_SIZE / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SELECT_X + SELECT_SIZE / 2 + nWidth, SELECT_Y + SELECT_SIZE / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SELECT_X + SELECT_SIZE / 2 + nWidth, SELECT_Y - SELECT_SIZE / 2, 0.0f);

	// 頂点カラーの設定
	if (nCnt == g_nCountSelect)
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
	g_pVtxBuffPauseOe->Unlock();
}