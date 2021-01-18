//-------------------------------------------------------------------
//
// リザルトスコアヘッダー (score.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitResultScore(void);								// リザルトスコアの初期化処理
void UninitResultScore(void);								// リザルトスコアの終了処理
void UpdateResultScore(void);								// リザルトスコアの更新処理
void DrawResultScore(void);									// リザルトスコアの描画処理
void AddResultScore(int nCntPlayer, int nValue);			// リザルトスコアの加算

#endif
