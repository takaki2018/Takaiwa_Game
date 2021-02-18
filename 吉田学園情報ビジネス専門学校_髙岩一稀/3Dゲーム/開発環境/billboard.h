//-------------------------------------------------------------------
//
// ビルボードヘッダー (billboard.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitBillboard(void);						// ビルボードの初期化処理
void UninitBillboard(void);							// ビルボードの終了処理
void UpdateBillboard(void);							// ビルボードの更新処理
void DrawBillboard(void);							// ビルボードの描画処理
void SetVertexBillboard(int nCntBillboard);			// ビルボードの頂点座標の設定

void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fDepth);					// ビルボードの配置

#endif