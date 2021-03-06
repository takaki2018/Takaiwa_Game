//==============================================================================================================
//
// ランキング背景ヘッダー (ranking_bg.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _RANKINGBG_H_
#define _RANKINGBG_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------
HRESULT InitRankingBg(void);		// ランキング背景の初期化処理
void UninitRankingBg(void);			// ランキング背景の終了処理
void UpdateRankingBg(void);			// ランキング背景の更新処理
void DrawRankingBg(void);			// ランキング背景の描画処理

#endif