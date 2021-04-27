//==============================================================================================================
//
// チュートリアル画像 (tutorial_image.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial_image.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define IMAGE_X					(SCREEN_WIDTH / 2)		// 画像の中心座標(X)
#define IMAGE_Y					(340.0f)				// 画像の中心座標(Y)
#define IMAGE_WIDTH 			(960.0f)				// 画像の幅
#define IMAGE_HEIGHT			(540.0f)				// 画像の高さ

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialImage = NULL;					// チュートリアル画像の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTutorialImage[MAX_TUTORIAL_IMAGE] = {};	// チュートリアル画像のテクスチャのポインタ
int g_nPointImage;														// チュートリアル画像のページ番号

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画像の初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialImage(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_nPointImage = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe001.png", &g_pTextureTutorialImage[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_oe002.png", &g_pTextureTutorialImage[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_se.png", &g_pTextureTutorialImage[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/screen_ge.png", &g_pTextureTutorialImage[3]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TUTORIAL_IMAGE, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,										// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialImage,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialImage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TUTORIAL_IMAGE; nCnt++)
	{
		// 背景の更新
		pVtx[0].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(IMAGE_X - IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y + IMAGE_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(IMAGE_X + IMAGE_WIDTH / 2, IMAGE_Y - IMAGE_HEIGHT / 2, 0.0f);

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
	g_pVtxBuffTutorialImage->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画像の終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitTutorialImage(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTutorialImage != NULL)
	{
		g_pVtxBuffTutorialImage->Release();
		g_pVtxBuffTutorialImage = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TUTORIAL_IMAGE; nCntTexture++)
	{
		if (g_pTextureTutorialImage[nCntTexture] != NULL)
		{
			g_pTextureTutorialImage[nCntTexture]->Release();
			g_pTextureTutorialImage[nCntTexture] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画像の更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorialImage(void)
{
	// 変数宣言
	bool *pStick = GetStickTrigger(PLAYER_1);
	bool *pCrossKey = GetCrossKeyTrigger(PLAYER_1);

	// マーカーの更新
	// JOYPAD の処理
	if (pStick[STICKTRIGGER_RIGHT] == true ||
		pCrossKey[CROSSKEYPRESS_RIGHT] == true ||
		GetKeyboardTrigger(KEYINFO_RIGHT) == true ||
		GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true)
	{// 右に移動するとき
		if (g_nPointImage != MAX_TUTORIAL_IMAGE - 1)
		{
			g_nPointImage++;		// // 画像番号を増やす

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
	if (pStick[STICKTRIGGER_LEFT] == true ||
		pCrossKey[CROSSKEYPRESS_LEFT] == true ||
		GetKeyboardTrigger(KEYINFO_LEFT) == true ||
		GetJoypadTrigger(PLAYER_1, JPINFO_CANSEL) == true)
	{// 左に移動するとき
		if (g_nPointImage != 0)
		{
			g_nPointImage--;		// 画像番号を減らす

			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画像の描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawTutorialImage(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialImage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTextureTutorialImage[g_nPointImage]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画像が何番目か取得
// 引数		：void	- 何もなし
// 返り値	：int	- 何番目の画像が選択されているか返す
//--------------------------------------------------------------------------------------------------------------
int GetPointImage(void)
{
	return g_nPointImage;
}