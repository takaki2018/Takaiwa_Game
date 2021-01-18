//-------------------------------------------------------------------
//
// ロード画面ヘッダー (loadscreen.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _LOADSCREEN_H_
#define _LOADSCREEN_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitLoadScreen(void);			// ロード画面の初期化処理
void UninitLoadScreen(void);			// ロード画面の終了処理
void UpdateLoadScreen(void);			// ロード画面の更新処理
void DrawLoadScreen(void);				// ロード画面の描画処理
void SetVertexLoadScreen(int nCnt);		// ロード画面の頂点座標の設定
void LoadScreenBg(int nCnt);			// ロード画面の背景
void LoadScreenImage(int nCnt);			// ロード画面の画像
void LoadScreenSelect(int nCnt);		// ロード画面のセレクト
void LoadScreenPressOK(int nCnt);		// ロード画面のプレスOK

#endif
