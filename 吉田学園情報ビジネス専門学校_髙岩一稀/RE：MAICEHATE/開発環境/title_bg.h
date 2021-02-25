//-------------------------------------------------------------------
//
// タイトル背景ヘッダー (title_bg.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _TITLEBG_H_
#define _TITLEBG_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitTitleBg(void);		 // ポリゴンの初期化処理
void UninitTitleBg(void);		 // ポリゴンの終了処理
void UpdateTitleBg(void);		// ポリゴンの更新処理
void DrawTitleBg(void);			// ポリゴンの描画処理
void SetVertexTitleBg(int nCnt); // 頂点座標の設定

#endif