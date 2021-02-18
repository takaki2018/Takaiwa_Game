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
#define MAX_MODE					(2)					// アニメーションパターンNo.の最大数

#define SELECTMODE_X				(SCREEN_WIDTH / 2)	// セレクトモードの中心座標(X)
#define SELECTMODE_Y				(410.0f)			// セレクトモードの中心座標(Y)
#define SELECTMODE_WIDTH			(400.0f)			// セレクトモードの幅
#define SELECTMODE_HEIGHT			(70.0f)				// セレクトモードの高さ

#define SELECTMODE_SCALE_X			(0.6f)				// セレクトモードの拡大縮小値(X)
#define SELECTMODE_SCALE_Y			(0.14f)				// セレクトモードの拡大縮小値(Y)
#define SELECTMODE_CHANGE_COLOR_B	(0.01f)				// セレクトモードの色変化値(B)

#define FLASH_INTERBAL_MODELOGO		(10)				// 点滅インターバル
#define CLEAR_NUMBER_MODELOGO		(5)					// 透明
#define LESS_CLEAR_NUMBER_MODELOGO	(0)					// 不透明

#define TURN_TEXT_RIGHT				(70)				// テキストの右傾きカウント
#define TURN_TEXT_LEFT				(140)				// テキストの左傾きカウント
#define FLASH_COUNT					(50)				// フラッシュカウント

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectmode = NULL;			// セレクトモードの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureSelectmode[MAX_MODE] = {};			// セレクトモードのテクスチャのポインタ
SELECTMODE g_aSelectMode[MAX_MODE];								// セレクトモードの情報
D3DXCOLOR g_LogoCol;											// 色
PLAYMODE g_playmode;											// プレイモード
int g_nCntMode;													// モードカウント
bool g_bPressOK;												// キーを押したか
int g_nCntScale;												// 拡大縮小カウント
int g_nCntFlash;												// 点滅カウント

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
	for (int nCntSM = 0; nCntSM < MAX_MODE; nCntSM++)
	{
		g_aSelectMode[nCntSM].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SELECTMODE_Y + 100.0f * nCntSM, 0.0f);
		g_aSelectMode[nCntSM].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSelectMode[nCntSM].fWidth = SELECTMODE_WIDTH;
		g_aSelectMode[nCntSM].fHeight = SELECTMODE_HEIGHT;
	}
	g_playmode = PLAYMODE_SINGLE;
	g_nCntMode = (int)g_playmode;
	g_bPressOK = false;
	g_nCntScale = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/play_single.png", &g_pTextureSelectmode[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/play_multi.png", &g_pTextureSelectmode[1]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_MODE, // 確保するバッファサイズ
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

	for (int nCntLogo = 0; nCntLogo < MAX_MODE; nCntLogo++)
	{
		// 頂点座標の設定
		SetVertexLogo(nCntLogo);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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
	for (int nCntTexture = 0; nCntTexture < MAX_MODE; nCntTexture++)
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
	// どのモードを選択しているか
	g_playmode = (PLAYMODE)g_nCntMode;

	switch (g_bPressOK)
	{// モード選択中
	case false:
		SelectMode();
		break;
		// モード選択完了
	case true:
		DecideMode();
		break;
	}
	// 頂点座標の更新
	for (int nCntMode = 0; nCntMode < MAX_MODE; nCntMode++)
	{
		SetVertexLogo(nCntMode);
	}
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
	for (int nCntTexture = 0; nCntTexture < MAX_MODE; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureSelectmode[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//-----------------------------------------------------------------
// モード選択中
//-----------------------------------------------------------------
void SelectMode(void)
{
	// 変数宣言
	bool *pStickTrigger = GetStickTrigger(PLAYER_1);
	bool *pCrossKeyTrigger = GetCrossKeyTrigger(PLAYER_1);

	// 拡大縮小をカウント
	g_nCntScale++;

	// JOYPAD の処理
	if (pStickTrigger[STICKTRIGGER_UP] == true || 
		pCrossKeyTrigger[CROSSKEYPRESS_UP] == true ||
		GetKeyboardTrigger(KEYINFO_UP) == true)
	{// スティックを上に倒したとき
		g_aSelectMode[g_nCntMode].fWidth = SELECTMODE_WIDTH;		// 幅の初期化
		g_aSelectMode[g_nCntMode].fHeight = SELECTMODE_HEIGHT;		// 高さの初期化
		g_aSelectMode[g_nCntMode].col.b = 1.0f;						// 青成分の初期化
		g_nCntScale = 0;											// カウントの初期化
		g_nCntMode--;												// モードの確定

		if (g_nCntMode < (int)PLAYMODE_SINGLE)
		{// 初期化
			g_nCntMode = (int)PLAYMODE_MULTI;
		}
		// SE
		PlaySound(SOUND_LABEL_SE_DOWN);
	}
	else if (pStickTrigger[STICKTRIGGER_DOWN] == true || 
		pCrossKeyTrigger[CROSSKEYPRESS_DOWN] == true ||
		GetKeyboardTrigger(KEYINFO_DOWN) == true)
	{// スティックを下に倒したとき
		g_aSelectMode[g_nCntMode].fWidth = SELECTMODE_WIDTH;		// 幅の初期化
		g_aSelectMode[g_nCntMode].fHeight = SELECTMODE_HEIGHT;		// 高さの初期化
		g_aSelectMode[g_nCntMode].col.b = 1.0f;						// 青成分の初期化
		g_nCntScale = 0;											// カウントの初期化
		g_nCntMode++;												// モードの確定

		if (g_nCntMode > (int)PLAYMODE_MULTI)
		{// 初期化
			g_nCntMode = (int)PLAYMODE_SINGLE;
		}
		// SE
		PlaySound(SOUND_LABEL_SE_DOWN);
	}

	if (0 <= g_nCntScale && TURN_TEXT_RIGHT > g_nCntScale)
	{// 大きさと色の変更
		g_aSelectMode[g_nCntMode].fWidth += SELECTMODE_SCALE_X;
		g_aSelectMode[g_nCntMode].fHeight += SELECTMODE_SCALE_Y;
		g_aSelectMode[g_nCntMode].col.b -= SELECTMODE_CHANGE_COLOR_B;
	}
	else if (TURN_TEXT_RIGHT <= g_nCntScale && TURN_TEXT_LEFT > g_nCntScale)
	{// 大きさと色の変更
		g_aSelectMode[g_nCntMode].fWidth -= SELECTMODE_SCALE_X;
		g_aSelectMode[g_nCntMode].fHeight -= SELECTMODE_SCALE_Y;
		g_aSelectMode[g_nCntMode].col.b += SELECTMODE_CHANGE_COLOR_B;
	}
	else if (g_nCntScale == TURN_TEXT_LEFT)
	{// カウントが140になったとき元に戻す
		g_aSelectMode[g_nCntMode].fWidth = SELECTMODE_WIDTH;
		g_aSelectMode[g_nCntMode].fHeight = SELECTMODE_HEIGHT;
		g_aSelectMode[g_nCntMode].col.b = 1.0f;
		g_nCntScale = 0;
	}
	// 各モードに遷移
	if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJAMP) == true || 
		GetKeyboardTrigger(KEYINFO_OK) == true)
	{// 押したのでtrueにする
		g_bPressOK = true;

		// 選択したモードのロゴの状態を初期化
		g_aSelectMode[g_nCntMode].fWidth = SELECTMODE_WIDTH;
		g_aSelectMode[g_nCntMode].fHeight = SELECTMODE_HEIGHT;
		g_aSelectMode[g_nCntMode].col.b = 1.0f;

		// サウンド
		PlaySound(SOUND_LABEL_SE_OK);
	}
}

//-----------------------------------------------------------------
// モード選択完了
//-----------------------------------------------------------------
void DecideMode(void)
{
	// 点滅させるためにカウントを進める
	g_nCntFlash++;

	if (CLEAR_NUMBER_MODELOGO == g_nCntFlash % FLASH_INTERBAL_MODELOGO)
	{// 透明
		g_aSelectMode[g_nCntMode].col.a = 0.0f;
	}
	else if (LESS_CLEAR_NUMBER_MODELOGO == g_nCntFlash % FLASH_INTERBAL_MODELOGO)
	{// 不透明
		g_aSelectMode[g_nCntMode].col.a = 1.0f;
	}
	// カウントがある程度進んだら遷移
	if (g_nCntFlash >= FLASH_COUNT)
	{// 次に進んだとき処理
	 // ゲームに遷移
		SetFade(FADE_OUT, MODE_LOADSCREEN);
	}
}

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVertexLogo(int nCntLogo)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	SELECTMODE *pSelectMode = &g_aSelectMode[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelectmode->Lock(0, 0, (void**)&pVtx, 0);

	//	何番目か
	pVtx += nCntLogo * 4;
	pSelectMode += nCntLogo;

	if (nCntLogo == (int)PLAYMODE_SINGLE)
	{// 頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(pSelectMode->pos.x - (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y + (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pSelectMode->pos.x - (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y - (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pSelectMode->pos.x + (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y + (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pSelectMode->pos.x + (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y - (pSelectMode->fHeight / 2.0f), 0.0f);
	}
	else if (nCntLogo == (int)PLAYMODE_MULTI)
	{// 頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(pSelectMode->pos.x - (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y + (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pSelectMode->pos.x - (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y - (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pSelectMode->pos.x + (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y + (pSelectMode->fHeight / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pSelectMode->pos.x + (pSelectMode->fWidth / 2.0f), pSelectMode->pos.y - (pSelectMode->fHeight / 2.0f), 0.0f);
	}

	// 頂点カラーの設定
	pVtx[0].col = pSelectMode->col;
	pVtx[1].col = pSelectMode->col;
	pVtx[2].col = pSelectMode->col;
	pVtx[3].col = pSelectMode->col;

	// 頂点バッファをアンロックする
	g_pVtxBuffSelectmode->Unlock();
}

//-----------------------------------------------------------------
// どのモードを選んだか
//-----------------------------------------------------------------
PLAYMODE GetPlayMode(void)
{
	return g_playmode;
}