//==============================================================================================================
//
// フェードヘッダー (fade.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// 構造体の初期化
//--------------------------------------------------------------------------------------------------------------
typedef enum
{
	FADE_NONE = 0,	// 何もしていない
	FADE_IN,		// フェードイン
	FADE_OUT,		// フェードアウト
	FADE_MAX,
}FADE;

//--------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------
HRESULT InitFade(MODE modeNext);		// タイトルの初期化処理
void UninitFade(void);					// タイトルの終了処理
void UpdateFade(void);					// タイトルの更新処理
void DrawFade(void);					// タイトルの描画処理
void SetFade(FADE fade,MODE modeNext);	// フェード設定
FADE GetFade(void);						// フェード情報の取得

#endif
