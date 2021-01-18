//-------------------------------------------------------------------
//
// 木の裏ヘッダー (wood.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _WOODBACK_H_
#define _WOODBACK_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitWoodBack(void);			// 木の裏の初期化処理
void UninitWoodBack(void);			// 木の裏の終了処理
void UpdateWoodBack(void);			// 木の裏の更新処理
void DrawWoodBack(void);			// 木の裏の描画処理
void SetVertexWoodBack(int nCnt);	// 頂点座標の設定

#endif