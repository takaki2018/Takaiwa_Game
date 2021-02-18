//-------------------------------------------------------------------
//
// 敵スコアヘッダー (score_enemy.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SCOREENEMY_H_
#define _SCOREENEMY_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitScoreEnemy(void);							// スコアの初期化処理
void UninitScoreEnemy(void);							// スコアの終了処理
void UpdateScoreEnemy(void);							// スコアの更新処理
void DrawScoreEnemy(void);								// スコアの描画処理
void SetScoreEnemy(D3DXVECTOR3 pos, D3DXCOLOR col);		// スコアのセット
void SetVertexScoreEnemy(int nCnt);						// 頂点座標の設定

#endif
