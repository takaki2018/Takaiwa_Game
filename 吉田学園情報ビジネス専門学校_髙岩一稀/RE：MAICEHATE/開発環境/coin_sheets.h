//-------------------------------------------------------------------
//
// コイン獲得状況ヘッダー (coin_sheets.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _COINSHEETS_H_
#define _COINSHEETS_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitCoinSheets(void);					// コイン獲得状況の初期化処理
void UninitCoinSheets(void);					// コイン獲得状況の終了処理
void UpdateCoinSheets(void);					// コイン獲得状況の更新処理
void DrawCoinSheets(void);						// コイン獲得状況の描画処理

void SetVertexCoinSheets(int nIdx);				// 頂点座標の設定
void SetCoinTexInfo(int nIdx);					// コインテクスチャの設置情報
void SetNumberTexInfo(int nIdx);				// 数字テクスチャの設置情報

void AddCoinSheets(int nValue);					// コイン獲得状況の加算

#endif