//-------------------------------------------------------------------
//
// 外積を用いた当たり判定ヘッダー (cross_product.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _CROSSPRODUCT_H_
#define _CROSSPRODUCT_H_

#include "main.h"

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 aPos[2];		// 線分の二点
	D3DXVECTOR3 pos;			// ある点
	D3DXVECTOR3 vecA;			// 線分のベクトル
	D3DXVECTOR3 vecB;			// 点とaPos[0]のベクトル
	float fVec;					// 外積の値
}CrossProduct;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
void CrossProductTest(void);			// 外積を用いた当たり判定テスト
CrossProduct *GetCrossProduct(void);		// 情報の取得

#endif