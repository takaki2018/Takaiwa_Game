//-----------------------------------------------------------------
//
// セレクトモード (selectmode.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "selectmode.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN			(3)					// アニメーションパターンNo.の最大数
#define MARKAR_NUMBER		(2)

#define SELECTMODE_X		(400.0f)			// セレクトモードの幅
#define SELECTMODE_Y		(40.0f)				// セレクトモードの高さ

#define MARKAR_X			(40.0f)				// マーカーの幅
#define MARKAR_Y			(40.0f)				// マーカーの高さ

#define FLASH_INTERBAL		(10)				// 点滅インターバル
#define CLEAR_NUMBER		(5)					// 透明
#define LESS_CLEAR_NUMBER	(0)					// 不透明

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectmode = NULL;			// セレクトモードの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureSelectmode[MAX_PATTERN] = {};		// セレクトモードのテクスチャのポインタ
D3DXVECTOR3 g_posSelectMode;									// プレスエンターの中心座標
D3DXVECTOR3 g_posMarcar;										// マーカーの中心座標
float g_posY;													// Y座標の移動量
int g_SelectMode;												// モードカウント
D3DXCOLOR g_LogoCol;											// 色		
bool g_PressOK;													// キーを押したか
int g_nCntFlash;												// カウント

//-----------------------------------------------------------------
// セレクトモードの初期化処理
//-----------------------------------------------------------------
HRESULT InitSelectmode(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posSelectMode = D3DXVECTOR3(SCREEN_WIDTH / 2, 450.0f, 0.0f);
	g_posMarcar = D3DXVECTOR3(SCREEN_WIDTH / 3, 450.0f, 0.0f);
	g_posY = 0.0f;
	g_SelectMode = SELECTMODE_NORMAL;
	g_LogoCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_PressOK = false;
	g_nCntFlash = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Normalmode_logo.png", &g_pTextureSelectmode[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Endlessmode_logo.png", &g_pTextureSelectmode[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/markar.png", &g_pTextureSelectmode[2]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectmode,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLogo = 0; nCntLogo < MAX_PATTERN; nCntLogo++)
	{
		// 頂点座標の設定
		SetVertexLogo(nCntLogo);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_LogoCol;
		pVtx[1].col = g_LogoCol;
		pVtx[2].col = g_LogoCol;
		pVtx[3].col = g_LogoCol;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffSelectmode->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// セレクトモードの終了処理
//-----------------------------------------------------------------
void UninitSelectmode(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffSelectmode != NULL)
	{
		g_pVtxBuffSelectmode->Release();
		g_pVtxBuffSelectmode = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_PATTERN; nCntTexture++)
	{
		if (g_pTextureSelectmode[nCntTexture] != NULL)
		{
			g_pTextureSelectmode[nCntTexture]->Release();
			g_pTextureSelectmode[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// セレクトモードの更新処理
//-----------------------------------------------------------------
void UpdateSelectmode(void)
{
	// 変数宣言
	bool *pStick = GetControllerTrigger();

	if (g_PressOK == false)
	{
		if (GetKeyboardTrigger(KEYINFO_UP) == true)
		{// マーカーを上に動かしたとき
			// モードの確定
			g_SelectMode--;
			g_posY -= 100.0f;

			if (g_SelectMode < SELECTMODE_NORMAL)
			{// 初期化
				g_SelectMode = SELECTMODE_ENDLESS;
				g_posY = 100.0f;
			}
			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
		else if (GetKeyboardTrigger(KEYINFO_DOWN) == true)
		{// マーカーを下に動かしたとき
			// モードの確定
			g_SelectMode++;
			g_posY += 100.0f;

			if (g_SelectMode > SELECTMODE_ENDLESS)
			{// 初期化
				g_SelectMode = SELECTMODE_NORMAL;
				g_posY = 0.0f;
			}
			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
		// JOYPAD の処理
		if (pStick[0] == true)
		{// マーカーを上に動かしたとき
			// モードの確定
			g_SelectMode--;
			g_posY -= 100.0f;

			if (g_SelectMode < SELECTMODE_NORMAL)
			{// 初期化
				g_SelectMode = SELECTMODE_ENDLESS;
				g_posY = 100.0f;
			}
			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
		else if (pStick[1] == true)
		{// マーカーを上に動かしたとき
		 // モードの確定
			g_SelectMode++;
			g_posY += 100.0f;

			if (g_SelectMode > SELECTMODE_ENDLESS)
			{// 初期化
				g_SelectMode = SELECTMODE_NORMAL;
				g_posY = 0.0f;
			}
			// SE
			PlaySound(SOUND_LABEL_SE_DOWN);
		}
		// 各モードに遷移
		if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(JPINFO_OK) == true)
		{// 押したのでtrueにする
			g_PressOK = true;

			// サウンド
			PlaySound(SOUND_LABEL_SE_OK);
		}
	}
	else if (g_PressOK == true)
	{// 点滅させるためにカウントを進める
		g_nCntFlash++;

		if (CLEAR_NUMBER == g_nCntFlash % FLASH_INTERBAL)
		{// 透明
			g_LogoCol.a = 0.0f;
		}
		else if (LESS_CLEAR_NUMBER == g_nCntFlash % FLASH_INTERBAL)
		{// 不透明
			g_LogoCol.a = 1.0f;
		}
		// カウントがある程度進んだら遷移
		if (g_nCntFlash >= 50)
		{// 次に進んだとき処理
			if (g_SelectMode == SELECTMODE_NORMAL)
			{// ゲームに遷移
				SetFade(FADE_OUT, MODE_LOADSCREEN);
			}
			else if (g_SelectMode == SELECTMODE_ENDLESS)
			{// ゲームに遷移
				SetFade(FADE_OUT, MODE_LOADSCREEN);
			}
		}
	}
	// 頂点座標の更新
	SetVertexLogo(g_SelectMode);
	SetVertexLogo(MARKAR_NUMBER);
}

//-----------------------------------------------------------------
// セレクトモードの描画処理
//-----------------------------------------------------------------
void DrawSelectmode(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelectmode, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_PATTERN; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureSelectmode[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//-----------------------------------------------------------------
// タイトル画面に貼るテクスチャの指定
//-----------------------------------------------------------------
void SetVertexLogo(int nCntLogo)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

	//	何番目か
	pVtx += nCntLogo * 4;

	if (nCntLogo == SELECTMODE_NORMAL)
	{// 頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(g_posSelectMode.x - (SELECTMODE_X / 2), g_posSelectMode.y + (SELECTMODE_Y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posSelectMode.x - (SELECTMODE_X / 2), g_posSelectMode.y - (SELECTMODE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posSelectMode.x + (SELECTMODE_X / 2), g_posSelectMode.y + (SELECTMODE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posSelectMode.x + (SELECTMODE_X / 2), g_posSelectMode.y - (SELECTMODE_Y / 2), 0.0f);
	}
	else if (nCntLogo == SELECTMODE_ENDLESS)
	{// 頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(g_posSelectMode.x - (SELECTMODE_X / 2), g_posSelectMode.y + (SELECTMODE_Y / 2) + 100, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posSelectMode.x - (SELECTMODE_X / 2), g_posSelectMode.y - (SELECTMODE_Y / 2) + 100, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posSelectMode.x + (SELECTMODE_X / 2), g_posSelectMode.y + (SELECTMODE_Y / 2) + 100, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posSelectMode.x + (SELECTMODE_X / 2), g_posSelectMode.y - (SELECTMODE_Y / 2) + 100, 0.0f);
	}
	else if (nCntLogo == MARKAR_NUMBER)
	{// 頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(g_posMarcar.x - (MARKAR_X / 2), g_posMarcar.y + (MARKAR_Y / 2) + g_posY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posMarcar.x - (MARKAR_X / 2), g_posMarcar.y - (MARKAR_Y / 2) + g_posY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posMarcar.x + (MARKAR_X / 2), g_posMarcar.y + (MARKAR_Y / 2) + g_posY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posMarcar.x + (MARKAR_X / 2), g_posMarcar.y - (MARKAR_Y / 2) + g_posY, 0.0f);
	}
	// 頂点カラーの設定
	pVtx[0].col = g_LogoCol;
	pVtx[1].col = g_LogoCol;
	pVtx[2].col = g_LogoCol;
	pVtx[3].col = g_LogoCol;

	// 頂点バッファをアンロックする
	g_pVtxBuffSelectmode->Unlock();
}

//-----------------------------------------------------------------
// どのモードを選んだか
//-----------------------------------------------------------------
int GetSelectMode(void)
{
	return g_SelectMode;
}