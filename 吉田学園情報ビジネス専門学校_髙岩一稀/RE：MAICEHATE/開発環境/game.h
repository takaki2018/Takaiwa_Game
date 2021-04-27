//==============================================================================================================
//
// ゲームメインヘッダー (game.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------
HRESULT InitGame(void);			// ベースの初期化処理
void UninitGame(void);			// ベースの終了処理
void UpdateGame(void);			// ベースの更新処理
void DrawGame(void);			// ベースの描画処理

void SetPause(bool bPause);		// ポーズの切り替え

#endif