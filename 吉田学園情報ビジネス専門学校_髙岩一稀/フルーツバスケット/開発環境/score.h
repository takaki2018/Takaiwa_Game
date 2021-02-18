//-------------------------------------------------------------------
//
// スコアヘッダー (score.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitScore(void);								// スコアの初期化処理
void UninitScore(void);									// スコアの終了処理
void UpdateScore(void);									// スコアの更新処理
void DrawScore(void);									// スコアの描画処理

void HiddenScore(void);									// スコアを隠す処理
void AddScore(int nCntPlayer,int nValue);				// スコアの加算
int GetScore(int nCntScore);							// スコアの取得

#endif
