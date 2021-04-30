//==============================================================================================================
//
// チュートリアル用PressOKヘッダー (tutorial_press_ok.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _TUTORIALPRESSOK_H_
#define _TUTORIALPRESSOK_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// 構造体の定義
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fMoveAlpha;		// α値の移動
	int nFlashCnt;			// 点滅カウント
	bool bDisPlay;			// 表示するかどうか
	bool bPressOK;			// 押したかどうか
	bool bGameTrasition;	// ゲーム画面に遷移するかどうか
}TutorialPressOK;

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
TutorialPressOK GetPressOKInfo(void);	// チュートリアル用PressOK情報の取得

#endif
