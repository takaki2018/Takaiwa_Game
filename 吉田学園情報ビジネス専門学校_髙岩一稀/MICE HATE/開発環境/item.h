//-------------------------------------------------------------------
//
// アイテムヘッダー (item.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitItem(void);				// アイテムの初期化処理
void UninitItem(void);				// アイテムの終了処理
void UpdateItem(void);				// アイテムの更新処理
void DrawItem(void);				// アイテムの描画処理
bool GetbUseItem(void);				// アイテムの使用判定
void SetItem(bool bUseItem);		// 使用判定

#endif
