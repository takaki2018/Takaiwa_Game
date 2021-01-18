//-------------------------------------------------------------------
//
// カラスヘッダー (crow.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _CROW_H_
#define _CROW_H_

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
}CROW;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitCrow(void);								// カラスの初期化処理
void UninitCrow(void);								// カラスの終了処理
void UpdateCrow(void);								// カラスの更新処理
void DrawCrow(void);								// カラスの描画処理
void SetVertexCrow(int nCnt);						// 頂点座標の設定
void SetCrow(D3DXVECTOR3 pos, D3DXVECTOR3 move);	// カラスの配置

CROW *GetCrow(void);									// カラス情報の取得

#endif