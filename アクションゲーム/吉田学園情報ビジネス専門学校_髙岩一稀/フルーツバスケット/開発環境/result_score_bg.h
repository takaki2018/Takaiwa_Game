//-------------------------------------------------------------------
//
// リザルトスコア背景ヘッダー (result_score_bg.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RESULTSCOREBG_H_
#define _RESULTSCOREBG_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitResultScoreBg(void);		// リザルトスコア背景の初期化処理
void UninitResultScoreBg(void);			// リザルトスコア背景の終了処理
void UpdateResultScoreBg(void);			// リザルトスコア背景の更新処理
void DrawResultScoreBg(void);			// リザルトスコア背景の描画処理

#endif