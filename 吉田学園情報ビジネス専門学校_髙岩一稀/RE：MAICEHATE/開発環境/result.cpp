//==============================================================================================================
//
// リザルト (result.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "result.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "timer.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_PATTERN			(11)									// アニメーションパターンNo.の最大数
#define MAX_TEXTURE			(4)										// リザルト画面に使うテクスチャの数
#define MAX_SCORE			(8)										// 表示するスコアの桁
#define TEXTURE_X			(10)									// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y			(1)										// アニメーションパターンNo.の縦の最大数

#define PLAY_ANIM			((1.0f / TEXTURE_X) * g_nPatternAnim)   // テクスチャの動作
#define UV_U				(1.0f / TEXTURE_X + PLAY_ANIM)			// Uの値
#define UV_V				(1.0f / TEXTURE_Y)						// Vの値

#define RESULT_LOGO_X		(SCREEN_WIDTH / 2)						// リザルトロゴの中心座標(X)
#define RESULT_LOGO_Y		(150.0f)								// リザルトロゴの中心座標(Y)
#define RESULT_LOGO_WIDTH	(800.0f)								// リザルトロゴの幅
#define RESULT_LOGO_HEIGHT	(100.0f)								// リザルトロゴの高さ

#define YOUR_SCORE_X		(SCREEN_WIDTH / 2)						// YOUR SCOREロゴの中心座標(X)
#define YOUR_SCORE_Y		(300.0f)								// YOUR SCOREロゴの中心座標(Y)
#define YOUR_SCORE_WIDTH	(300.0f)								// YOUR SCOREロゴの幅
#define YOUR_SCORE_HEIGHT	(40.0f)									// YOUR SCOREロゴの高さ

#define SCORE_X				(395.0f)								// スコアの中心座標(X)
#define SCORE_Y				(420.0f)								// スコアの中心座標(Y)
#define SCORE_WIDTH			(80.0f)									// スコアの幅
#define SCORE_HEIGHT		(110.0f)								// スコアの高さ
#define SCORE_SPACE			(70.0f)									// スコアを配置する間隔

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// リザルト画面の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_TEXTURE] = {};	// リザルト画面のテクスチャのポインタ
D3DXVECTOR3 g_posLogo;									// ゲームクリアーロゴの中心座標
D3DXVECTOR3 g_posYourScore;								// ロゴ
D3DXVECTOR3 g_posScoreResult;							// スコア
int g_nScoreResult;										// スコア

//--------------------------------------------------------------------------------------------------------------
// リザルトの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitResult(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();
	g_nScoreResult = GetScore();
	Player *pPlayer = GetPlayer();
	int nTimer = GetTimer();

	// 変数の初期化
	g_posLogo = D3DXVECTOR3(RESULT_LOGO_X, RESULT_LOGO_Y, 0.0f);
	g_posYourScore = D3DXVECTOR3(YOUR_SCORE_X, YOUR_SCORE_Y, 0.0f);
	g_posScoreResult = D3DXVECTOR3(SCORE_X, SCORE_Y, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureResult[0]);

	if (pPlayer->bFall == true || nTimer == 0)
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/game_over_logo.png", &g_pTextureResult[1]);
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/game_clear_logo.png", &g_pTextureResult[1]);
	}

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/your_score.png", &g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureResult[3]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL)))
	{
		return E_FAIL;
	}

	for (int nCntResult = 0; nCntResult < MAX_TEXTURE; nCntResult++)
	{
		// 頂点座標の設定
		SetVertexResult(nCntResult);
	}

	// サウンドの追加
	PlaySound(SOUND_LABEL_RESULT);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// リザルトの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitResult(void)
{
	// サウンドの一時停止
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureResult[nCntTexture] != NULL)
		{
			g_pTextureResult[nCntTexture]->Release();
			g_pTextureResult[nCntTexture] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// リザルトの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(PLAYER_1,JPINFO_OKorJUMP) == true)
	{// 画面モードの初期化処理
		SetFade(FADE_OUT,MODE_RANKING);
	}
}

//--------------------------------------------------------------------------------------------------------------
// リザルトの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawResult(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_PATTERN; nCntTexture++)
	{
		if (nCntTexture < 3)
		{
			pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
		}
		else if (nCntTexture >= 3 && nCntTexture < 11)
		{
			pDevice->SetTexture(0, g_pTextureResult[3]);
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// リザルト画面に貼るテクスチャの指定
// 引数		：nCntResult	- 何番目のリザルトで使うテクスチャなのか
// 返り値	：void			- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetVertexResult(int nCntResult)
{
	if (nCntResult == 0)
	{
		// 構造体のポインタ変数
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 何番目か
		pVtx += nCntResult * 4;

		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 頂点バッファをアンロックする
		g_pVtxBuffResult->Unlock();
	}
	else if (nCntResult == 1)
	{
		// 構造体のポインタ変数
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 何番目か
		pVtx += nCntResult * 4;

		pVtx[0].pos = D3DXVECTOR3(g_posLogo.x - (RESULT_LOGO_WIDTH / 2), g_posLogo.y + (RESULT_LOGO_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posLogo.x - (RESULT_LOGO_WIDTH / 2), g_posLogo.y - (RESULT_LOGO_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posLogo.x + (RESULT_LOGO_WIDTH / 2), g_posLogo.y + (RESULT_LOGO_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posLogo.x + (RESULT_LOGO_WIDTH / 2), g_posLogo.y - (RESULT_LOGO_HEIGHT / 2), 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 頂点バッファをアンロックする
		g_pVtxBuffResult->Unlock();
	}
	else if (nCntResult == 2)
	{
		// 構造体のポインタ変数
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 何番目か
		pVtx += nCntResult * 4;

		pVtx[0].pos = D3DXVECTOR3(g_posYourScore.x - (YOUR_SCORE_WIDTH / 2), g_posYourScore.y + (YOUR_SCORE_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posYourScore.x - (YOUR_SCORE_WIDTH / 2), g_posYourScore.y - (YOUR_SCORE_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posYourScore.x + (YOUR_SCORE_WIDTH / 2), g_posYourScore.y + (YOUR_SCORE_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posYourScore.x + (YOUR_SCORE_WIDTH / 2), g_posYourScore.y - (YOUR_SCORE_HEIGHT / 2), 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 頂点バッファをアンロックする
		g_pVtxBuffResult->Unlock();
	}
	else if (nCntResult == 3)
	{
		// 構造体のポインタ変数
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 何番目か
		pVtx += nCntResult * 4;

		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			// 変数宣言
			int aNumber[8];

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posScoreResult.x - (SCORE_WIDTH / 2) + SCORE_SPACE * nCnt, g_posScoreResult.y + (SCORE_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posScoreResult.x - (SCORE_WIDTH / 2) + SCORE_SPACE * nCnt, g_posScoreResult.y - (SCORE_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posScoreResult.x + (SCORE_WIDTH / 2) + SCORE_SPACE * nCnt, g_posScoreResult.y + (SCORE_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posScoreResult.x + (SCORE_WIDTH / 2) + SCORE_SPACE * nCnt, g_posScoreResult.y - (SCORE_HEIGHT / 2), 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// スコアを格納
			aNumber[nCnt] = (g_nScoreResult / (int)powf(10.0f, (float)(MAX_SCORE - (nCnt + 1))));

			// テクスチャの更新
			pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCnt] * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[1].tex = D3DXVECTOR2(0.0f + aNumber[nCnt] * 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCnt] * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCnt] * 0.1f, 0.0f);

			// 更新
			pVtx += 4;
		}

		// 頂点バッファをアンロックする
		g_pVtxBuffResult->Unlock();
	}
}