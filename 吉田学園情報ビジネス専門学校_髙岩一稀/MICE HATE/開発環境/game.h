//-------------------------------------------------------------------
//
// ゲームヘッダー (enemy.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _z_H_
#define _GAME_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitGame(void);			// ゲーム画面の初期化処理
void UninitGame(void);			// ゲーム画面の終了処理
void UpdateGame(void);			// ゲーム画面の更新処理
void DrawGame(void);			// ゲーム画面の描画処理
void NormalMode(void);			// ノーマルモードの処理
void EndlessMode(void);			// エンドレスモードの処理
void SetPause(bool bPause);		// ポーズしてるかどうか

#endif
