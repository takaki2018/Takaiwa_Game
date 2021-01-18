//-------------------------------------------------------------------
//
// ポーズヘッダー (pause.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//-------------------------------------------------------------------
// ポーズの状態
//-------------------------------------------------------------------
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,	// コンティニュー
	PAUSE_MENU_RETRY,			// リトライ
	PAUSE_MENU_QUIT,			// 終了
	PAUSE_MENU_MAX,
}PAUSE_MENU;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitPause(void);				// ポーズの初期化処理
void UninitPause(void);					// ポーズの終了処理
void UpdatePause(void);					// ポーズの更新処理
void DrawPause(void);					// ポーズの描画処理
void SetVertexPause(int nCntPause);		// ポーズ画面の頂点座標
void PauseMenu(int nCnt);				// ポーズメニュー

#endif