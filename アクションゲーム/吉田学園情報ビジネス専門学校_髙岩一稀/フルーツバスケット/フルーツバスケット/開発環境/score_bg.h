//-------------------------------------------------------------------
//
// スコア背景ヘッダー (score_bg.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SCOREBG_H_
#define _SCOREBG_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitScoreBg(void);			// スコア背景の初期化処理
void UninitScoreBg(void);			// スコア背景の終了処理
void UpdateScoreBg(void);			// スコア背景の更新処理
void DrawScoreBg(void);				// スコア背景の描画処理

#endif