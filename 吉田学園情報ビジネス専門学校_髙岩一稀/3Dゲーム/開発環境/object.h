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
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMeshModel;			// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMatModel;		// マテリアル(材質情報)へのポインタ
	DWORD nNumMatModel = 0;			// マテリアルの数
	D3DXMATRIX mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 pos;				// 中心座標
	D3DXVECTOR3 rot;				// 回転角
	D3DXVECTOR3 rotDest;			// 目的の回転角
	D3DXVECTOR3 move;				// 移動量
	D3DXVECTOR3 moverot;			// 回転量
	int nIdx;						// 何番目の影か
	bool bMove;						// 移動したかどうか
}Object;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitObject(void);		// オブジェクトの初期化処理
void UninitObject(void);		// オブジェクトの終了処理
void UpdateObject(void);		// オブジェクトの更新処理
void DrawObject(void);			// オブジェクトの描画処理

void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);					// 当たり判定

#endif