//-------------------------------------------------------------------
//
// タイマーヘッダー (timer.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitTimer(void);					// タイマーの初期化処理
void UninitTimer(void);						// タイマーの終了処理
void UpdateTimer(void);						// タイマーの更新処理
void DrawTimer(void);						// タイマーの描画処理
int GetTimer(void);							// タイマー情報の取得
int GetCntSetEnemy(void);					// 敵を配置する間隔の取得

#endif

