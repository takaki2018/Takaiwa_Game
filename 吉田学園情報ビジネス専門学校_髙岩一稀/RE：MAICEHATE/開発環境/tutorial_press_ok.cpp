//==============================================================================================================
//
// チュートリアル用PressOK (tutorial_press_ok.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial_press_ok.h"
#include "tutorial_image.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define PRESSOK_X		(1050.0f)					// プレスOKの中心座標(X)
#define PRESSOK_Y		(660.0f)					// プレスOKの中心座標(Y)
#define PRESSOK_WIDTH 	(300.0f)					// プレスOKの幅
#define PRESSOK_HEIGHT	(50.0f)						// プレスOKの高さ

#define MOVE_ALPHA		(0.01f)						// α値の移動量
#define FADE_INTERBAL	(60)						// フェードのインターバル
#define ALUFA_INTERBAL	(10)						// 点滅のインターバルインターバル

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialPressOK = NULL;		// チュートリアル用PressOKの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTutorialPressOK = NULL;			// チュートリアル用PressOKのテクスチャのポインタ
TutorialPressOK g_TutorialPressOK;								// 

//--------------------------------------------------------------------------------------------------------------
// チュートリアル用PressOKの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialPressOK(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// チュートリアル用PressOK情報の初期化
	g_TutorialPressOK.pos = D3DXVECTOR3(PRESSOK_X, PRESSOK_Y,0.0f);
	g_TutorialPressOK.col = D3DXCOLOR(1.0f, 1.0f, 1.0f,0.0f);
	g_TutorialPressOK.fWidth = PRESSOK_WIDTH / 2.0f;
	g_TutorialPressOK.fHeight = PRESSOK_HEIGHT / 2.0f;
	g_TutorialPressOK.fMoveAlpha = MOVE_ALPHA;
	g_TutorialPressOK.nFlashCnt = 0;
	g_TutorialPressOK.bDisPlay = false;
	g_TutorialPressOK.bPressOK = false;
	g_TutorialPressOK.bGameTrasition = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_A.png", &g_pTextureTutorialPressOK);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX , // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialPressOK,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialPressOK->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の更新
	pVtx[0].pos = D3DXVECTOR3(g_TutorialPressOK.pos.x - g_TutorialPressOK.fWidth, g_TutorialPressOK.pos.y + g_TutorialPressOK.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_TutorialPressOK.pos.x - g_TutorialPressOK.fWidth, g_TutorialPressOK.pos.y - g_TutorialPressOK.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_TutorialPressOK.pos.x + g_TutorialPressOK.fWidth, g_TutorialPressOK.pos.y + g_TutorialPressOK.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_TutorialPressOK.pos.x + g_TutorialPressOK.fWidth, g_TutorialPressOK.pos.y - g_TutorialPressOK.fHeight, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	SetColTutorialPressOK();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorialPressOK->Unlock();

	// サウンド
	PlaySound(SOUND_LABEL_OE);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル用PressOKの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitTutorialPressOK(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTutorialPressOK != NULL)
	{
		g_pVtxBuffTutorialPressOK->Release();
		g_pVtxBuffTutorialPressOK = NULL;
	}

	// テクスチャの開放
	if (g_pTextureTutorialPressOK != NULL)
	{
		g_pTextureTutorialPressOK->Release();
		g_pTextureTutorialPressOK = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル用PressOKの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorialPressOK(void)
{
	switch (g_TutorialPressOK.bPressOK)
	{
	case true:
		// PressOKを押した後
		AfterPreesingOK();
		break;

	case false:
		// PressOKを押す前
		BeforePressingOK();
		break;
	}

	// 色の更新
	SetColTutorialPressOK();
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル用PressOKの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawTutorialPressOK(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialPressOK, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	if (g_TutorialPressOK.bDisPlay == true)
	{
		pDevice->SetTexture(0, g_pTextureTutorialPressOK);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル用PressOKを押す前の処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void BeforePressingOK(void)
{
	// 変数宣言
	int nPointImage = GetPointImage();

	if (nPointImage == MAX_TUTORIAL_IMAGE - 1)
	{// 最後のページにいったとき
	 // 押したことを伝える
		g_TutorialPressOK.bDisPlay = true;

		// α値の減算
		g_TutorialPressOK.col.a -= g_TutorialPressOK.fMoveAlpha;

		if (g_TutorialPressOK.col.a < 0.0f || g_TutorialPressOK.col.a >= 1.0f)
		{
			// 0.0fより小さいまたは1.0fより大きくなったときα値の移動量に-1をかける
			g_TutorialPressOK.fMoveAlpha *= -1;
		}
		// Aボタンまたはエンターキーで遷移
		if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true ||
			GetKeyboardTrigger(KEYINFO_OK) == true)
		{
			// 押したことを伝える
			g_TutorialPressOK.bPressOK = true;

			// サウンド
			PlaySound(SOUND_LABEL_SE_OK);
		}
	}
	else if (nPointImage != MAX_TUTORIAL_IMAGE - 1)
	{// 最後のページにいっていないときカウント初期化とテクスチャを透明にする
		g_TutorialPressOK.nFlashCnt = 0;
		g_TutorialPressOK.col.a = 0.0f;

		// 描画しないようにする
		g_TutorialPressOK.bDisPlay = false;
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル用PressOKを押した後の処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void AfterPreesingOK(void)
{
	// 点滅カウントを進める
	g_TutorialPressOK.nFlashCnt++;

	// 点滅処理
	if (0 == g_TutorialPressOK.nFlashCnt % ALUFA_INTERBAL)
	{
		// 不透明
		g_TutorialPressOK.col.a = 1.0f;
	}
	else if ((ALUFA_INTERBAL / 2) == g_TutorialPressOK.nFlashCnt % ALUFA_INTERBAL)
	{
		// 若干薄く
		g_TutorialPressOK.col.a = 0.2f;
	}

	if (g_TutorialPressOK.nFlashCnt >= FADE_INTERBAL)
	{// ゲームに遷移する許可を与える
		g_TutorialPressOK.bGameTrasition = true;
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル用PressOKのプレスOK
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetColTutorialPressOK(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialPressOK->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = g_TutorialPressOK.col;
	pVtx[1].col = g_TutorialPressOK.col;
	pVtx[2].col = g_TutorialPressOK.col;
	pVtx[3].col = g_TutorialPressOK.col;

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorialPressOK->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル用PressOK情報の取得
// 引数		：void	- 何もなし
// 返り値	：bool	- チュートリアル用PressOK情報を渡す
//--------------------------------------------------------------------------------------------------------------
TutorialPressOK GetPressOKInfo(void)
{
	return g_TutorialPressOK;
}