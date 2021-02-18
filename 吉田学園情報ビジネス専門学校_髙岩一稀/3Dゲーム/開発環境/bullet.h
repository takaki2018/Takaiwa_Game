//-------------------------------------------------------------------
//
// バレットヘッダー (bullet.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitBullet(void);										// 弾の初期化処理
void UninitBullet(void);										// 弾の終了処理
void UpdateBullet(void);										// 弾の更新処理
void DrawBullet(void);											// 弾の描画処理

void SetVertexBullet(int nIdx);									// 弾の頂点座標	
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight);	// 弾の配置

#endif