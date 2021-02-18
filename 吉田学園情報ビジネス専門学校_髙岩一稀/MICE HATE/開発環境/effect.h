//-------------------------------------------------------------------
//
// エフェクトヘッダー (effect.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitEffect(void);											// エフェクトの初期化処理
void UninitEffect(void);											// エフェクトの終了処理
void UpdateEffect(void);											// エフェクトの更新処理
void DrawEffect(void);												// エフェクトの描画処
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fRemove,float fRandius,float fCntAlpha, int nLife);		// エフェクトの設定

#endif
