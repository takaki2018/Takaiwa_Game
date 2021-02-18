//-------------------------------------------------------------------
//
// コインヘッダー (coin.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _COIN_H_
#define _COIN_H_

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
	D3DXVECTOR3 vtxMinModel;			// 座標の最小値
	D3DXVECTOR3 vtxMaxModel;			// 座標の最大値
	int nIdx;						// 何番目の影か
	D3DXVECTOR3 aPos[4];			// 線分の4点
	D3DXVECTOR3 aVecA[4];			// 線分のベクトル
	D3DXVECTOR3 aVecB[4];			// 線分のベクトル
	float fVec[4];					// 外積の値
	bool bUse;						// 使用しているかどうか
}Coin;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitCoin(void);		// コインの初期化処理
void UninitCoin(void);		// コインの終了処理
void UpdateCoin(void);		// コインの更新処理
void DrawCoin(void);			// コインの描画処理

void SetCoin(D3DXVECTOR3 pos);			// コインの設置
void TexCoin(int nCntCoin);				// コインのテクスチャ
void VecCoin(int nCntCoin);				// コインの頂点座標
void CrossProductCoin(int nCntCoin);	// 外積を用いたコインの当たり判定

bool CollisionCoin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);		// 当たり判定

#endif