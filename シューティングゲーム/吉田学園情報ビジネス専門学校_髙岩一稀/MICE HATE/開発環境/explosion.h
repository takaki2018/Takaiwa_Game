//-------------------------------------------------------------------
//
// 爆発ヘッダー (explotion.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitExplosion(void);										// 弾の初期化処理
void UninitExplosion(void);											// 弾の終了処理
void UpdateExplosion(void);											// 弾の更新処理
void DrawExplosion(void);											// 弾の描画処
void SetExplosion(D3DXVECTOR3 pos,D3DCOLOR col);						// 弾の設定
void SetVertexExplosion(int nIdx);									// 弾の頂点座標

#endif