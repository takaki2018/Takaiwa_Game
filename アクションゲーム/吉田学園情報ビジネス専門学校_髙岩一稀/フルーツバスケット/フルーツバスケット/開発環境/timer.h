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
HRESULT InitTimer(void);	// タイマーの初期化処理
void UninitTimer(void);		// タイマーの終了処理
void UpdateTimer(void);		// タイマーの更新処理
void DrawTimer(void);		// タイマーの描画処理

void CountTimer(void);		// タイマーのカウント処理
void FlashTmer(void);		// タイムアップ時のタイマー点滅処理

void SetTexUVTimer(void);	// テクスチャUVの設定
int GetTimer(void);			// タイマー情報の取得

#endif