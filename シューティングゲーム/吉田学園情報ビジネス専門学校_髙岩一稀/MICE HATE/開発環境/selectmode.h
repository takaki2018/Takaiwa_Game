//-------------------------------------------------------------------
//
// タイトルヘッダー (title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SELECTMODE_H_
#define _SELECTMODE_H_

#include "main.h"

//-------------------------------------------------------------------
// 列挙型
//-------------------------------------------------------------------
typedef enum
{
	SELECTMODE_NORMAL = 0,	// ノーマルモード
	SELECTMODE_ENDLESS,		// エンドレスモード
	SELECTMODE_TUTORIAL,	// チュートリアルモード
}SELECTMODE;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitSelectmode(void);		// ロゴの初期化処理
void UninitSelectmode(void);		// ロゴの終了処理
void UpdateSelectmode(void);		// ロゴの更新処理
void DrawSelectmode(void);			// ロゴの描画処理
void SetVertexLogo(int nCntLogo);	// タイトル画面に貼るロゴの指定
int GetSelectMode(void);			// 何のモードを選んだか

#endif
