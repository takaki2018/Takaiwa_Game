//-------------------------------------------------------------------
//
// UIヘッダー (ui.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _UI_H_
#define _UI_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitUI(void);				// UIの初期化処理
void UninitUI(void);				// UIの終了処理
void UpdateUI(void);				// UIの更新処理
void DrawUI(void);					// UIの描画処理

#endif