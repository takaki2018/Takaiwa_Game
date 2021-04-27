//==============================================================================================================
//
// ランキングスコア (ranking_score.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "ranking_score.h"
#include "score.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"
#include "2Deffect.h"
#include "setparticle.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_TEXTURE				(1)											// テクスチャの最大数
#define TEXTURE_X				(10)										// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y				(1)											// アニメーションパターンNo.の縦の最大数
#define MAX_RANKING_SCORE		(5)											// 順位の数
#define MAX_SCORE_DIGIT			(8)											// 桁数
#define MAX_NUMBER				(MAX_SCORE_DIGIT * MAX_RANKING_SCORE)		// 背景の種類

#define RANKING_SCORE_X			(600.0f)									// ランキングスコアの頂点座標(X)
#define RANKING_SCORE_Y			(220.0f)									// ランキングスコアの頂点座標(Y)
#define RANKING_SCORE_WIDTH		(60.0f)										// ランキングスコアの幅
#define RANKING_SCORE_HEIGHT	(90.0f)										// ランキングスコアの高さ
#define RANKING_SCORE_SPACE		(60.0f)										// ランキングスコアを配置する間隔

#define FLASH_SCORE_INTERVAL	(8)										// 点滅のインターバル

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingScore = NULL;						// ランキングスコアの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankingScore[MAX_TEXTURE] = {};				// ランキングスコアのテクスチャのポインタ
D3DXVECTOR3 g_posRankingScore;												// ランキングスコアの頂点座標
D3DXCOLOR g_ScoreCol[MAX_RANKING_SCORE];										// スコアの色
int g_aScore[MAX_RANKING_SCORE] = { 5000,4000,3000,2000,1000 };				// ランキングスコアの初期スコア
int g_nCntScore;															// 表示するスコアの数
int g_nSubScore;															// スコアの保存先
bool g_bFlash;																// 点滅させるか
int g_nFlashScore;															// 点滅させるスコア
int g_nFlashScoreTimer;														// 点滅タイマー
int g_SelectMode2;															// どのモードを選択したか

//--------------------------------------------------------------------------------------------------------------
// ランキングスコアの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitRankingScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posRankingScore = D3DXVECTOR3(RANKING_SCORE_X, RANKING_SCORE_Y, 0.0f);
	g_nCntScore = 0;
	g_nSubScore = 0;
	g_bFlash = false;
	g_nFlashScore = -1;
	g_nFlashScoreTimer = MAX_RANKING_SCORE;

	// スコアの色の初期化
	for (int nCnt = 0; nCnt < MAX_RANKING_SCORE; nCnt++)
	{
		g_ScoreCol[nCnt] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	}

	// ランキングの入れ替え
	Ranking();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureRankingScore[0]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_NUMBER, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingScore,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化
	for (int nCntRankingRank = 0; nCntRankingRank < MAX_RANKING_SCORE; nCntRankingRank++)
	{
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + 100.0f * nCntRankingRank, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + 100.0f * nCntRankingRank, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + 100.0f * nCntRankingRank, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + 100.0f * nCntRankingRank, 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;


			// 頂点カラーの設定
			pVtx[0].col = g_ScoreCol[g_nCntScore];
			pVtx[1].col = g_ScoreCol[g_nCntScore];
			pVtx[2].col = g_ScoreCol[g_nCntScore];
			pVtx[3].col = g_ScoreCol[g_nCntScore];

			// スコアを格納
			int nNumber = (g_aScore[nCntRankingRank] / (int)powf(10.0f, (float)(MAX_SCORE - (nCntDigit + 1))));

			// テクスチャの更新
			pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[1].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 0.0f);

			// 次の情報に移す
			pVtx += 4;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRankingScore->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// ランキングスコアの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitRankingScore(void)
{
	// サウンドの一時停止
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffRankingScore != NULL)
	{
		g_pVtxBuffRankingScore->Release();
		g_pVtxBuffRankingScore = NULL;
	}

	// テクスチャの開放
	for (int nCntRanking = 0; nCntRanking < MAX_TEXTURE; nCntRanking++)
	{
		if (g_pTextureRankingScore[nCntRanking] != NULL)
		{
			g_pTextureRankingScore[nCntRanking]->Release();
			g_pTextureRankingScore[nCntRanking] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ランキングスコアの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateRankingScore(void)
{
	// 画面遷移
	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
	// 点滅
	if (g_nFlashScore != -1)
	{
		if (g_nFlashScoreTimer == FLASH_SCORE_INTERVAL / 2)
		{// 色の確定
			g_ScoreCol[g_nFlashScore] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		}
		else if (g_nFlashScoreTimer == FLASH_SCORE_INTERVAL)
		{// 色の確定
			g_ScoreCol[g_nFlashScore] = D3DXCOLOR(0.8f, 0.1f, 0.1f, 1.0f);
			// タイマーの初期化
			g_nFlashScoreTimer = 0;
		}
		g_nFlashScoreTimer++;

		// 構造体のポインタ変数
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

		// 初期化
		for (int nCntRankingRank = 0; nCntRankingRank < MAX_RANKING_SCORE; nCntRankingRank++)
		{
			for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
			{
				// 頂点カラーの設定
				pVtx[0].col = g_ScoreCol[nCntRankingRank];
				pVtx[1].col = g_ScoreCol[nCntRankingRank];
				pVtx[2].col = g_ScoreCol[nCntRankingRank];
				pVtx[3].col = g_ScoreCol[nCntRankingRank];

				// 次の情報に移す
				pVtx += 4;
			}
		}

		SetRankingEffect(D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * (MAX_SCORE_DIGIT - 1),
			g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + 100.0f * g_nFlashScore, 0.0f),
			true);
	}
}

//--------------------------------------------------------------------------------------------------------------
// ランキングスコアの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawRankingScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_NUMBER; nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRankingScore[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// ランキングの入れ替え
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void Ranking(void)
{
	// 変数宣言
	int nScore = GetScore();
	int nSubScore = 0;

	if (g_aScore[4] <= nScore)
	{//	5位のスコアより大きいとき
		g_aScore[4] = nScore;
	}

	// ランキングの更新
	for (int nCnt = 0; nCnt < MAX_RANKING_SCORE - 1; nCnt++)
	{
		for (int nCnt2 = nCnt + 1; nCnt2 < MAX_RANKING_SCORE; nCnt2++)
		{
			if (g_aScore[nCnt] <= g_aScore[nCnt2])
			{// ランキングスコア更新したとき
				nSubScore = g_aScore[nCnt];
				g_aScore[nCnt] = g_aScore[nCnt2];
				g_aScore[nCnt2] = nSubScore;
			}
		}
	}
	// どこに入ったか
	for (int nCnt = 0; nCnt < MAX_RANKING_SCORE; nCnt++)
	{
		if (g_aScore[nCnt] == nScore)
		{
			g_bFlash = true;
			g_nFlashScore = nCnt;
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// 点滅するスコアが何番目か取得
// 引数		：void	- 何もなし
// 返り値	：int	- 点滅する順位を返す
//--------------------------------------------------------------------------------------------------------------
int GetFlashNumber(void)
{
	return g_nFlashScore;
}