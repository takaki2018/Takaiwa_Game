//==============================================================================================================
//
// チュートリアル背景ヘッダー (tutorial_bg.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================-------------------------------------------------------------------
#ifndef _TUTORIALBG_H_
#define _TUTORIALBG_H_

#include "main.h"

//----------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//----------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialBg(void);	// チュートリアル背景の初期化処理
void UninitTutorialBg(void);	// チュートリアル背景の終了処理
void UpdateTutorialBg(void);	// チュートリアル背景の更新処理
void DrawTutorialBg(void);		// チュートリアル背景の描画処理

void SetTexUVTutorialBg(void);	// チュートリアル背景のテクスチャ座標の設定

#endif