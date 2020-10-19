#//-------------------------------------------------------------------
//
// 衝撃波ヘッダー (shockwave.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SHOCKWAVE_H_
#define _SHOCKWAVE_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitShockwave(void);										// 衝撃波の初期化処理
void UninitShockwave(void);											// 衝撃波の終了処理
void UpdateShockwave(void);											// 衝撃波の更新処理
void DrawShockwave(void);											// 衝撃波の描画処理
void SetShockwave(D3DXVECTOR3 pos,D3DXVECTOR3 size ,D3DXCOLOR col, 
	float fSize,float fNB, float fNB_MAG,float nCntAlpha);			// 衝撃波の設定
void SetVertexShockwave(int nIdx);									// 衝撃波の頂点座標の設定
float ShockwaveMove(void);											// ノックバック値の取得								

#endif

