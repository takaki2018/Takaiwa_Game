//-------------------------------------------------------------------
//
// カウントダウンヘッダー (count_down.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include "main.h"

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXCOLOR col;		// 色
	int nTExType;		// テクスチャの種類
	int nCntAlufa;		// α値の移動値
	float fCntSize;		// サイズの移動値
	bool bUse;			// 使用しているかどうか
}COUNTDOWN;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitCountDown(void);						// カウントダウンの初期化処理
void UninitCountDown(void);							// カウントダウンの終了処理
void UpdateCountDown(void);							// カウントダウンの更新処理
void DrawCountDown(void);							// カウントダウンの描画処理

void CountDownNumber(void);							// カウントダウンの数字処理
void CountDownStart(void);							// カウントダウンのスタート処理

void SetVertexCountDown(void);						// カウントダウンの頂点座標の設定
void SetVertexCountDownNumber(VERTEX_2D *pVtx);		// カウントダウンの数字の頂点座標の設定
void SetVertexCountDownStart(VERTEX_2D *pVtx);		// カウントダウンのスタートの頂点座標の設定
bool GetFinishCountDown(void);						// カウントダウンが終わったかどうか取得

#endif