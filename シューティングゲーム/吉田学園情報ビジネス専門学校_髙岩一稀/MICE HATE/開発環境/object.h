//-------------------------------------------------------------------
//
// オブジェクトヘッダー (object.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitObject(void);		// ポリゴンの初期化処理
void UninitObject(void);		// ポリゴンの終了処理
void UpdateObject(void);		// ポリゴンの更新処理
void DrawObject(void);			// ポリゴンの描画処理
D3DXVECTOR3 GetposObject(void);	// オブジェクトの座標情報の取得

#endif
