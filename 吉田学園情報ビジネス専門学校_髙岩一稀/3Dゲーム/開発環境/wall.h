//-------------------------------------------------------------------
//
// 壁ヘッダー (wall.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_WALL		(128)		// 用意する壁データのサイズ

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 pos;			// 壁の中心座標
	D3DXVECTOR3 rot;			// 壁の向き
	int nCntWallX;				// 壁の数(列)
	int nCntWallZ;				// 壁の数(行)
	int nCntVecX;				// 列の頂点数
	int nCntVecZ;				// 行の頂点数
	float fWidth;				// 幅
	float fDepth;				// 奥行
	float fsizeWidth;			// 幅のサイズ
	float fsizeDepth;			// 奥行のサイズ
	D3DXVECTOR3 aPos;			// 線分の4点
	D3DXVECTOR3 aVecA;			// 線分のベクトル
	D3DXVECTOR3 aVecB;			// 線分のベクトル
	float fVec;					// 内積の値
	bool bUse;					// 使用しているかどうか
}Wall;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitWall(void);								// 壁の初期化処理
void UninitWall(void);								// 壁の終了処理
void UpdateWall(void);								// 壁の更新処理
void DrawWall(void);								// 壁の描画処理

void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 壁の配置
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, 
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);					// 壁との当たり判定
Wall *GetWall(void);									// 壁情報の取得

#endif