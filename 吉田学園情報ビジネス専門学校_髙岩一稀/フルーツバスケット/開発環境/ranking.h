//-------------------------------------------------------------------
//
// ランキングヘッダー (ranking.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitRanking(void);			// ランキングの初期化処理
void UninitRanking(void);			// ランキングの終了処理
void UpdateRanking(void);			// ランキングの更新処理
void DrawRanking(void);				// ランキングの描画処理

void LoadRankingScore(void);		// ランキングスコアのロード
void SaveRankingScore(void);		// ランキングスコアのセーブ
void Ranking(void);					// ランキングのスコアの順番入れ替え
void SetVertexRanking(int nCnt);	// ランキングの頂点座標の設定

void RankingLogo(int nCnt);			// ランニングのロゴ
void RankingScore(int nCnt);		// ランニングのスコア

#endif
