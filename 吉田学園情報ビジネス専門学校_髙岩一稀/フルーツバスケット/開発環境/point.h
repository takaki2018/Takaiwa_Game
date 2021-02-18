//-------------------------------------------------------------------
//
// ポイントヘッダー (point.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _POINT_H_
#define _POINT_H_

#include "main.h"

//-------------------------------------------------------------------
// 列挙型
//-------------------------------------------------------------------
typedef enum
{
	POINTTYPE_PLUS_ONE = 0,		// プラス1ロゴ
	POINTTYPE_PLUS_FIVE,	// プラス5ロゴ
	POINTTYPE_DECREASE_ONE,		// マイナス1ロゴ
	POINTTYPE_DECREASE_FIVE,	// マイナス5ロゴ
	POINTTYPE_MAX,				// 獲得ポイントロゴの最大数
}POINTTYPE;						// 獲得ポイント

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
	POINTTYPE type;			// ロゴのタイプ
	bool bUse;				// 使用しているかどうか
}POINTLOGO;					// 獲得ポイントの構造体

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitPoint(void);											// ポイントの初期化処理
void UninitPoint(void);												// ポイントの終了処理
void UpdatePoint(void);												// ポイントの更新処理
void DrawPoint(void);												// ポイントの描画処理
void SetPoint(D3DXVECTOR3 pos, D3DXVECTOR3 move, POINTTYPE type);	// ポイントの配置
void SetVertexPoint(int nIdx);										// 頂点座標の設定

#endif