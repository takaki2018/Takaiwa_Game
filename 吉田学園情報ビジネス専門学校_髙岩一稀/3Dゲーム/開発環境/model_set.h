//-------------------------------------------------------------------
//
// モデル設置ヘッダー (model_set.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _MODELSET_H_
#define _MODELSET_H_

#include "main.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_MODEL			(256)

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMeshModel;									// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMatModel;								// マテリアル(材質情報)へのポインタ
	DWORD nNumMatModel = 0;									// マテリアルの数
	LPDIRECT3DTEXTURE9 apTextureModelSet[MAX_MODEL] = {};	// テクスチャへのポインタ
	char aFileName[128];									// テクスチャ名
	D3DXVECTOR3 vtxMinModel;								// 座標の最小値
	D3DXVECTOR3 vtxMaxModel;								// 座標の最大値
}ModelInfo;

typedef struct
{
	D3DXMATRIX mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 pos;				// 中心座標
	D3DXVECTOR3 rot;				// 回転角
	D3DXVECTOR3 rotDest;			// 目的の回転角
	D3DXVECTOR3 vtxMinModel;		// 座標の最小値
	D3DXVECTOR3 vtxMaxModel;		// 座標の最大値
	D3DXVECTOR3 aPos[4];			// 線分の4点
	D3DXVECTOR3 aVecA[4];			// 線分のベクトル
	D3DXVECTOR3 aVecB[4];			// 線分のベクトル
	float fVec[4];					// 外積の値
	int nIdx;						// モデル識別番号
	int nIdxShadow;					// 何番目の影か
	bool bUse;						// 使用してるかどうか
}ModelSet;

typedef struct
{
	int nNumModel;					// モデルの総数
	ModelInfo modelInfo[20];		// モデル情報
	ModelSet modelSet[MAX_MODEL];	// モデル設置
	int nCntModel;					// 配置するモデルの数
}Model;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitModelSet(void);									// モデルの初期化処理
void UninitModelSet(void);									// モデルの終了処理
void UpdateModelSet(void);									// モデルの更新処理
void DrawModelSet(void);									// モデルの描画処理

void TexModel(int Model);									// モデルのテクスチャ
void VecModel(int Model);									// モデルの頂点座標

void SetModelSet(int nIdx,D3DXVECTOR3 pos, D3DXVECTOR3 rot);// モデルの設置
void VecModelSet(int nNumModelSet);							// モデルの頂点座標
void CrossProductModelSet(int Model);						// 外積を用いたコインの当たり判定

bool CollisionModelSet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);			// 当たり判定

void LoadModelSet(void);									// モデル配置情報の読み込み

#endif