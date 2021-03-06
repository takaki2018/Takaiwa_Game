//==============================================================================================================
//
// 2Dエフェクトヘッダー (2Deffect.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _2DEFFECT_H_
#define _2DEFFECT_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------
HRESULT Init2DEffect(void);											// エフェクトの初期化処理
void Uninit2DEffect(void);											// エフェクトの終了処理
void Update2DEffect(void);											// エフェクトの更新処理
void Draw2DEffect(void);												// エフェクトの描画処
void Set2DEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fRemove, float fRandius, float fCntAlpha, float fGravity,
	float fBound);	// エフェクトの設定

#endif