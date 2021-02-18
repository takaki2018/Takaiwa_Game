//-------------------------------------------------------------------
//
// タイトルヘッダー (title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitTitle(void);				// タイトルの初期化処理
void UninitTitle(void);					// タイトルの終了処理
void UpdateTitle(void);					// タイトルの更新処理
void DrawTitle(void);					// タイトルの描画処理
void SetVertexTexture(int nCntTitle);	// タイトル画面に貼るテクスチャの指定

#endif
