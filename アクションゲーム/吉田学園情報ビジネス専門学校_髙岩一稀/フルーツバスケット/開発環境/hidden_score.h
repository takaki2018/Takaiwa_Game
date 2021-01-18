//-------------------------------------------------------------------
//
// はてなマークヘッダー (hidden_score.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _HIDDENSCORE_H_
#define _HIDDENSCORE_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitHiddenScore(void);								// はてなマークの初期化処理
void UninitHiddenScore(void);								// はてなマークの終了処理
void UpdateHiddenScore(void);								// はてなマークの更新処理
void DrawHiddenScore(void);									// はてなマークの描画処理

void DisplayQuestionMark(void);								// はてなマークを表示する処理

#endif
