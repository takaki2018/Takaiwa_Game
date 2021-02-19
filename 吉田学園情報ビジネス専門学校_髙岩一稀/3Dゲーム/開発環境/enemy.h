//-------------------------------------------------------------------
//
// エネミーヘッダー (enemy.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
	D3DXVECTOR3 pos;				// 座標
	D3DXVECTOR3 posOld;				// 前回の座標
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
}Enemy;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitEnemy(void);									// 敵の初期化処理
void UninitEnemy(void);										// 敵の終了処理
void UpdateEnemy(void);										// 敵の更新処理
void DrawEnemy(void);										// 敵の描画処理

void MoveEnemy(int nCntEnemy);								// 敵の移動処理
void SetEnemy(D3DXVECTOR3 pos);								// 敵の設置
void TexEnemy(int nCntCoin);								// 敵のテクスチャ
void VecEnemy(int nCntCoin);								// 敵の頂点座標
void CrossProductEnemy(int nCntCoin);						// 外積に必要なベクトルの計算

void DeleteEnemy(int nCntEnemy);							// 敵を消去する処理

bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pMove,D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);			// 当たり判定

#endif