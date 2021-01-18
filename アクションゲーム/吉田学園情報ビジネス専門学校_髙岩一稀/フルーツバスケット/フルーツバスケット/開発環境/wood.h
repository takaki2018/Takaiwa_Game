//-------------------------------------------------------------------
//
// 木ヘッダー (wood.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _WOOD_H_
#define _WOOD_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitWood(void);			// 木の初期化処理
void UninitWood(void);			// 木の終了処理
void UpdateWood(void);			// 木の更新処理
void DrawWood(void);			// 木の描画処理
void SetVertexWood(int nCnt);	// 頂点座標の設定

#endif