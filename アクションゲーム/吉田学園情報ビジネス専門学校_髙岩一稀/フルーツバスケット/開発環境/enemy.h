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
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posOld;	// 前の位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 回転
	int nCntBound;		// 跳ね返った回数
	float fWidth;		// 幅
	float fHeight;		// 高さ
	int nType;			// タイプ
	bool bUse;			// 使用しているかどうか
}ENEMY;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitEnemy(void);										// 敵の初期化処理
void UninitEnemy(void);											// 敵の終了処理
void UpdateEnemy(void);											// 敵の更新処理
void DrawEnemy(void);											// 敵の描画処理
void HitEnemy(int nCntEnemy);									// 敵の当たり判定
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int type);		// 敵の配置
void SetVertexEnemy(int nIdx);									// 頂点座標の設定
ENEMY *GetEnemy(void);

#endif

