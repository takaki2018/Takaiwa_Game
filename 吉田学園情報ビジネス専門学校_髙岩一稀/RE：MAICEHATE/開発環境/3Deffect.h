//==============================================================================================================
//
// 3Dエフェクトヘッダー (3Deffect.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _3DEFFECT_H_
#define _3DEFFECT_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------
HRESULT Init3DEffect(void);											// エフェクトの初期化処理
void Uninit3DEffect(void);											// エフェクトの終了処理
void Update3DEffect(void);											// エフェクトの更新処理
void Draw3DEffect(void);												// エフェクトの描画処
void Set3DEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fRemove,float fRandius,float fCntAlpha, float fGravity,
	float fBound);	// エフェクトの設定

#endif
