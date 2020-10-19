//-------------------------------------------------------------------
//
// ポーズOEヘッダー (pause_oe.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PAUSEOE_H_
#define _PAUSEOE_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitPauseOe(void);			// ポーズOEの初期化処理
void UninitPauseOe(void);			// ポーズOEの終了処理
void UpdatePauseOe(void);			// ポーズOEの更新処理
void DrawPauseOe(void);				// ポーズOEの描画処理
void SetVertexPauseOe(int nCnt);	// ポーズOEの頂点座標の設定
void PauseOeImage(int nCnt);		// ポーズOEの画像
void PauseOeSelect(int nCnt);		// ポーズOEのセレクト

#endif
