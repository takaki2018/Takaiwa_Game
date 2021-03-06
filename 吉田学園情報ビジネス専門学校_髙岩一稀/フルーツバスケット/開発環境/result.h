//-------------------------------------------------------------------
//
// リザルトヘッダー (title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RESULT_H_
#define _RESuLT_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitResult(void);				// リザルトの初期化処理
void UninitResult(void);				// リザルトの終了処理
void UpdateResult(void);				// リザルトの更新処理
void DrawResult(void);					// リザルトの描画処理
void SetVertexResult(int nCntResult);	// リザルト画面に貼るテクスチャの設定

#endif

