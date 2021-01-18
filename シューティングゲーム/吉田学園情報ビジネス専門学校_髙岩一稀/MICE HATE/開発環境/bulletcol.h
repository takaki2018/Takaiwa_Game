//-------------------------------------------------------------------
//
// 弾の色確認ヘッダー (bulletcol.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _BULLETCOL_H_
#define _BULLETCOL_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitBulletcol(void);	// 弾の色の初期化処理
void UninitBulletcol(void);		// 弾の色の終了処理
void UpdateBulletcol(void);		// 弾の色の更新処理
void DrawBulletcol(void);		// 弾の色の描画処理

#endif
