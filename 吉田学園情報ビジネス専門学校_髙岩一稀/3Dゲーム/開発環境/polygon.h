//-------------------------------------------------------------------
//
// ポリゴンヘッダー (polygon.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 pos;			// 壁の中心座標
	D3DXVECTOR3 rot;			// 壁の向き
	float fWidth;				// 幅
	float fDepth;				// 奥行
	bool bUse;					// 使用しているかどうか
}Floor;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitPolygon(void);										// ポリゴンの初期化処理
void UninitPolygon(void);										// ポリゴンの終了処理
void UpdatePolygon(void);										// ポリゴンの更新処理
void DrawPolygon(void);											// ポリゴンの描画処理

void SetPolygon(D3DXVECTOR3 pos, float fWidth, float fDepth);	// ポリゴンの設置
bool CollisionFloor(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);				// 床の当たり判定
Floor *Getpolygon(void);										// ポリゴン情報の取得

#endif