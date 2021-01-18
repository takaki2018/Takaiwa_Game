//-------------------------------------------------------------------
//
// 結果発表ヘッダー (result_ann.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RESULTANN_H_
#define _RESULTANN_H_

#include "main.h"

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXCOLOR col;		// 色
	float fWidth;		// 幅
	float fHeight;		// 高さ
	int nCounterState;	// 状態カウンタ
	bool bTurn;			// 移動したかどうか
	bool bSound;		// サウンドを使用したかどうか
	bool bUse;			// 使用しているかどうか
}RESULTANN;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitResultAnn(void);						// 結果発表の初期化処理
void UninitResultAnn(void);							// 結果発表の終了処理
void UpdateResultAnn(void);							// 結果発表の更新処理
void DrawResultAnn(void);							// 結果発表の描画処理
void SetVErtexResultAnn(void);						// 結果発表の頂点座標の設定
bool GetResultAnn(void);							// 結果発表の情報の取得

#endif