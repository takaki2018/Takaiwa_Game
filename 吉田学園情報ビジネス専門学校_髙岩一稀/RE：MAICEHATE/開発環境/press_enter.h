//==============================================================================================================
//
// プレスエンターヘッダー (title.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _PRESSENTER_H_
#define _PRESSENTER_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------
HRESULT InitPressEnter(void);		// プレスエンターの初期化処理
void UninitPressEnter(void);		// プレスエンターの終了処理
void UpdatePressEnter(void);		// プレスエンターの更新処理
void DrawPressEnter(void);			// プレスエンターの描画処理

void BeforePressingEnter(void);		// エンターキーを押す前の処理
void AfterPreesingEnter(void);		// エンターキーを押した後の処理

void SetVertexPressEnter(void);		// プレスエンターの頂点座標の設定
bool bGetTutorialTransition(void);	// チュートリアル画面に遷移するかどうか渡す
#endif
