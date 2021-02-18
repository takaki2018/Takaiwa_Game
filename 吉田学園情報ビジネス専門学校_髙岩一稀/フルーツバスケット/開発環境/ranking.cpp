//-------------------------------------------------------------------
//
// ランキング (ranking.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ranking.h"
#include "score.h"
#include "selectmode.h"
#include "rank_logo.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_SCORE				(2)			// 表示するスコアの桁数
#define MAX_PATTERN				(2)			// 背景の種類
#define MAX_TEXTURE				(11)		// 貼るテクスチャの最大数
#define TEXTURE_X				(10)		// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y				(1)			// アニメーションパターンNo.の縦の最大数
#define MAX_RANKING_RANK		(5)			// 順位の数

#define RANKING_LOGO_X			(800.0f)	// ランキングロゴの頂点座標(X)
#define RANKING_LOGO_Y			(200.0f)	// ランキングロゴの頂点座標(Y)
#define RANKING_LOGO_WIDTH		(440.0f)	// ランキングロゴの幅
#define RANKING_LOGO_HEIGHT		(64.0f)		// ランキングロゴの高さ

#define RANKING_RANK_X			(650.0f)	// ランキングランクの頂点座標(X)
#define RANKING_RANK_Y			(300.0f)	// ランキングランクの頂点座標(Y)
#define RANKING_RANK_WIDTH		(110.0f)	// ランキングランクの幅
#define RANKING_RANK_HEIGHT		(70.0f)		// ランキングランクの高さ
#define RANKING_RANK_SPACE		(70.0f)		// ランキングランクを配置する間隔

#define RANKING_SCORE_X			(900.0f)	// ランキングスコアの頂点座標(X)
#define RANKING_SCORE_Y			(300.0f)	// ランキングスコアの頂点座標(Y)
#define RANKING_SCORE_WIDTH		(50.0f)		// ランキングスコアの幅
#define RANKING_SCORE_HEIGHT	(70.0f)		// ランキングスコアの高さ
#define RANKING_SCORE_SPACE		(60.0f)		// ランキングスコアを配置する間隔

#define FLASH_SCORE_INTERVAL	(10)			// 点滅のインターバル

//-----------------------------------------------------------------
// ランキングで配置するもの(列挙型)
//-----------------------------------------------------------------
typedef enum
{
	RANKINGSET_LOGO = 0,	// ランキングロゴ
	RANKINGSET_SCORE,		// ランキングに配置するスコア
	RANKINGSET_MAX,
}RANKINGSET;

//-----------------------------------------------------------------
// ランキングのスコア情報
//-----------------------------------------------------------------
typedef struct
{
	D3DXCOLOR col;		// 色
	int nScore;			// スコア
	int nRank;			// 順位
	bool bChangeRank;	// 順位が変わるかどうか
}RANKINGSCORE;

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;			// ランキングの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRanking[RANKINGSET_MAX] = {};	// ランキングのテクスチャのポインタ
D3DXVECTOR3 g_posRankingLogo;								// ランキングロゴの頂点座標
D3DXVECTOR3 g_posRankingRank;								// ランキングランクの頂点座標
D3DXVECTOR3 g_posRankingScore;								// ランキングスコアの頂点座標
RANKINGSCORE g_aRankingScore[RANK_MAX];						// ランキングのスコア情報
bool g_bChangeRank;											// 順位が変わるかどうか
bool g_bFlash;												// 点滅させるか
int g_nFlashScore;											// 点滅させるスコア
int g_nFlashScoreTimer;										// 点滅タイマー

//-----------------------------------------------------------------
// ランキングの初期化処理
//-----------------------------------------------------------------
HRESULT InitRanking(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posRankingLogo = D3DXVECTOR3(RANKING_LOGO_X, RANKING_LOGO_Y, 0.0f);
	g_posRankingRank = D3DXVECTOR3(RANKING_RANK_X, RANKING_RANK_Y, 0.0f);
	g_posRankingScore = D3DXVECTOR3(RANKING_SCORE_X, RANKING_SCORE_Y, 0.0f);
	g_bChangeRank = false;
	g_bFlash = false;
	g_nFlashScore = 0;
	g_nFlashScoreTimer = MAX_RANKING_RANK;

	// ランキングのスコア情報の更新
	for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
	{
		g_aRankingScore[nCntRank].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRankingScore[nCntRank].nScore = 0;
		g_aRankingScore[nCntRank].nRank = nCntRank;
		g_aRankingScore[nCntRank].bChangeRank = false;
	}

	// スコアのロード
	LoadRankingScore();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_logo.png", &g_pTextureRanking[RANKINGSET_LOGO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureRanking[RANKINGSET_SCORE]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEXTURE, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL)))
	{
		return E_FAIL;
	}

	// ランキング順位の配置
	for (int nCntRank = RANK_1; nCntRank < MAX_RANKING_RANK; nCntRank++)
	{
		if (g_aRankingScore[nCntRank - 1].nScore == g_aRankingScore[nCntRank].nScore && nCntRank != RANK_1)
		{
			// 前の順位と同じ順位を保存
			g_aRankingScore[nCntRank].nRank = g_aRankingScore[nCntRank - 1].nRank;
		}
		// 何位か設置
		SetRankLogo(D3DXVECTOR3(RANKING_RANK_X, RANKING_RANK_Y + RANKING_RANK_SPACE * nCntRank, 0.0f),
			RANKING_RANK_WIDTH, RANKING_RANK_HEIGHT, (RANK)g_aRankingScore[nCntRank].nRank);
	}

	// 初期化
	for (int nCntRanking = 0; nCntRanking < RANKINGSET_MAX; nCntRanking++)
	{
		// 頂点座標の設定
		SetVertexRanking(nCntRanking);
	}

	// ランキングの入れ替え
	Ranking();

	return S_OK;
}

//-----------------------------------------------------------------
// ランキングの終了処理
//-----------------------------------------------------------------
void UninitRanking(void)
{
	// サウンドの一時停止
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	// テクスチャの開放
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		if (g_pTextureRanking[nCntRanking] != NULL)
		{
			g_pTextureRanking[nCntRanking]->Release();
			g_pTextureRanking[nCntRanking] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ランキングの更新処理
//-----------------------------------------------------------------
void UpdateRanking(void)
{
	// 順位が変わるところがあるとき処理
	if (g_bChangeRank == true)
	{
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{// 何番目の順位が変わるか
			if (g_aRankingScore[nCntRank].bChangeRank == true)
			{
				if (nCntRank == RANK_1)
				{// 順位の一位より下が変わるとき
					for (int nCntRankLower = RANK_2; nCntRankLower < RANK_MAX; nCntRankLower++)
					{
						if (g_aRankingScore[nCntRankLower].nScore == g_aRankingScore[nCntRankLower - 1].nScore && nCntRankLower != RANK_2)
						{// 同率スコアで二位じゃないとき順位をそろえる
							g_aRankingScore[nCntRankLower].nRank = g_aRankingScore[nCntRankLower - 1].nRank;
						}
						else
						{
							// 二位じゃない、または同率スコアがないとき順位を変更
							g_aRankingScore[nCntRankLower].nRank = nCntRankLower;
						}

						// ランクロゴの変更
						CangeRankLogo(nCntRankLower, (RANK)g_aRankingScore[nCntRankLower].nRank);
					}
				}
				else
				{// その他の処理
					// 順位を変更
					g_aRankingScore[nCntRank].nRank = g_aRankingScore[nCntRank - 1].nRank;

					// ランクロゴの変更
					CangeRankLogo(nCntRank, (RANK)g_aRankingScore[nCntRank].nRank);
				}
				// フラグを落とす
				g_bChangeRank = false;
			}
		}
	}
	// スコアを更新したとき点滅させる
	if (g_bFlash == true)
	{
		// 点滅用カウントを増やす
		g_nFlashScoreTimer++;

		if (0 == g_nFlashScoreTimer % FLASH_SCORE_INTERVAL)
		{
			g_aRankingScore[g_nFlashScore].col.b = 0.3f;
			g_aRankingScore[g_nFlashScore].col.g = 0.3f;
		}
		else if (FLASH_SCORE_INTERVAL / 2 == g_nFlashScoreTimer % FLASH_SCORE_INTERVAL)
		{
			g_aRankingScore[g_nFlashScore].col.b = 1.0f;
			g_aRankingScore[g_nFlashScore].col.g = 1.0f;
		}
	}
	// スコアの更新
	SetVertexRanking(RANKINGSET_SCORE);
}

//-----------------------------------------------------------------
// ランキングの描画処理
//-----------------------------------------------------------------
void DrawRanking(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_TEXTURE; nCntRanking++)
	{
		// ポリゴンの描画
		if (nCntRanking == RANKINGSET_LOGO)
		{
			pDevice->SetTexture(0, g_pTextureRanking[RANKINGSET_LOGO]);
		}
		else if (nCntRanking >= RANKINGSET_SCORE)
		{
			pDevice->SetTexture(0, g_pTextureRanking[RANKINGSET_SCORE]);
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}
}

//-----------------------------------------------------------------
// ランキングスコアのロード
//-----------------------------------------------------------------
void LoadRankingScore(void)
{
	// ローカル変数宣言
	FILE *pFile;

	// データの読み込み
	pFile = fopen("data\\ranking.txt", "r");
	if (pFile != NULL)
	{ //ファイル展開可能
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{
			fscanf(pFile, "%d", &g_aRankingScore[nCntRank].nScore);
		}
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}

//-----------------------------------------------------------------
// ランキングスコアのセーブ
//-----------------------------------------------------------------
void SaveRankingScore(void)
{
	// ローカル変数宣言
	FILE *pFile;

	// ファイルの展開
	pFile = fopen("data\\ranking.txt", "w");
	if (pFile != NULL)
	{ // ファイル展開可能
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{
			fprintf(pFile, "%d\n", g_aRankingScore[nCntRank].nScore);
		}
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}

//-----------------------------------------------------------------
// ランキングの入れ替え
//-----------------------------------------------------------------
void Ranking(void)
{
	// 変数宣言
	int nScore = GetScore(PLAYER_1);
	int nSubScore = 0;
	bool bChangeRanking = false;	// ランキングの処理を行うかどうか

	if (g_aRankingScore[RANK_5].nScore <= nScore)
	{//	5位のスコアより大きいとき
		g_aRankingScore[RANK_5].nScore = nScore;
		bChangeRanking = true;
	}
	// ランキングに変動があるとき
	if (bChangeRanking == true)
	{
		// スコアが同じとき前の順位と同じにする
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{
			if (g_aRankingScore[nCntRank].nScore == g_aRankingScore[RANK_5].nScore)
			{
				// 変わるのでフラグを立てる
				g_aRankingScore[nCntRank + 1].bChangeRank = true;
				g_bChangeRank = true;
			}
		}

		// ランキングの更新
		for (int nCntRankTop = 0; nCntRankTop < RANK_MAX - 1; nCntRankTop++)
		{
			for (int nCntRankLower = nCntRankTop + 1; nCntRankLower < RANK_MAX; nCntRankLower++)
			{
				if (g_aRankingScore[nCntRankTop].nScore <= g_aRankingScore[nCntRankLower].nScore)
				{// ランキングスコア更新したとき
					nSubScore = g_aRankingScore[nCntRankTop].nScore;
					g_aRankingScore[nCntRankTop].nScore = g_aRankingScore[nCntRankLower].nScore;
					g_aRankingScore[nCntRankLower].nScore = nSubScore;
				}
			}
		}

		// 新しく入ったスコアが一位のとき
		if (g_aRankingScore[RANK_1].nScore == nScore)
		{// 一位より下の順位を更新する
			g_aRankingScore[RANK_1].bChangeRank = true;
		}

		// どこに入ったか
		for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
		{
			if (g_aRankingScore[nCntRank].nScore == nScore)
			{
				g_bFlash = true;
				g_nFlashScore = nCntRank;
				break;
			}
		}
		// スコアのセーブ
		SaveRankingScore();
	}
}

//-----------------------------------------------------------------
// ランキングの頂点座標の設定
//-----------------------------------------------------------------
void SetVertexRanking(int nCnt)
{
	// 頂点座標の設定
	if (nCnt == RANKINGSET_LOGO)
	{// ランキングロゴの配置
		RankingLogo(nCnt);
	}
	else if (nCnt == RANKINGSET_SCORE)
	{
		// スコアの配置
		RankingScore(nCnt);
	}
}

//-----------------------------------------------------------------
// ランキングのロゴ
//-----------------------------------------------------------------
void RankingLogo(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCnt * 4;

	pVtx[0].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y + RANKING_LOGO_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y - RANKING_LOGO_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y + RANKING_LOGO_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y - RANKING_LOGO_HEIGHT / 2, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//-----------------------------------------------------------------
// ランキングのスコア
//-----------------------------------------------------------------
void RankingScore(int nCnt)
{
	// 変数宣言
	int nNumber;												// スコアを保存する変数

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCnt * 4;

	for (int nCntRank = 0; nCntRank < RANK_MAX; nCntRank++)
	{
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE; nCntDigit++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRank, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRank, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRank, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * nCntRank, 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = g_aRankingScore[nCntRank].col;
			pVtx[1].col = g_aRankingScore[nCntRank].col;
			pVtx[2].col = g_aRankingScore[nCntRank].col;
			pVtx[3].col = g_aRankingScore[nCntRank].col;

			// スコアを格納
			nNumber = (g_aRankingScore[nCntRank].nScore / (int)powf(10.0f, (float)(MAX_SCORE - (nCntDigit + 1))));

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
	g_pVtxBuffRanking->Unlock();
}