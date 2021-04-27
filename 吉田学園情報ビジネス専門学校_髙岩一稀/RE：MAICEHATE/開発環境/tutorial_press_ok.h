//==============================================================================================================
//
// チュートリアル用PressOKヘッダー (tutorial_press_ok.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _TUTORIALPRESSOK_H_
#define _TUTORIALPRESSOK_H_

#include "main.h"

//----------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//----------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialPressOK(void);		// チュートリアル用PressOKの初期化処理
void UninitTutorialPressOK(void);		// チュートリアル用PressOKの終了処理
void UpdateTutorialPressOK(void);		// チュートリアル用PressOKの更新処理
void DrawTutorialPressOK(void);			// チュートリアル用PressOKの描画処理

void BeforePressingOK(void);			// チュートリアル用PressOKを押す前の処理
void AfterPreesingOK(void);				// チュートリアル用PressOKを押した後の処理

void SetColTutorialPressOK(void);		// チュートリアル用PressOKの色の設定
bool GetGameTransition(void);			// ゲームに遷移するかどうかを引き渡す

#endif
