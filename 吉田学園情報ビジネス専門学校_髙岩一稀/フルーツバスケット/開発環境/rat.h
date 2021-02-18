//-------------------------------------------------------------------
//
// ネズミヘッダー (crow.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RAT_H_
#define _RAT_H_

#include "main.h"

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	float fWidth;		// 幅
	float fHeight;		// 高さ
	int nCntAnim;		// アニメーションカウント
	int nPatternAnim;	// アニメーションパターン
	bool bUse;			// 使用しているかどうか
}RAT;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitRat(void);							// ネズミの初期化処理
void UninitRat(void);							// ネズミの終了処理
void UpdateRat(void);							// ネズミの更新処理
void DrawRat(void);								// ネズミの描画処理
void HitRat(void);								// ネズミの当たり判定
void SetVertexRat(int nCnt);					// 頂点座標の設定
void SetRat(D3DXVECTOR3 pos, D3DXVECTOR3 move);	// ネズミの配置

#endif