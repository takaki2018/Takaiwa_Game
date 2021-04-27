//==============================================================================================================
//
// コインヘッダー (coin.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _COIN_H_
#define _COIN_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_COIN			(8)

#define MAX_TEXTURE_NAME	(128)		// テクスチャ名
#define MAX_TEXTURE			(128)		// テクスチャの数

//--------------------------------------------------------------------------------------------------------------
// 構造体の定義
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMeshModel;									// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMatModel;								// マテリアル(材質情報)へのポインタ
	DWORD nNumMatModel = 0;									// マテリアルの数
	LPDIRECT3DTEXTURE9 apTextureModelSet[MAX_TEXTURE] = {};	// テクスチャへのポインタ
	char aFileName[MAX_TEXTURE_NAME];						// テクスチャ名
	D3DXVECTOR3 vtxMinModel;								// 座標の最小値
	D3DXVECTOR3 vtxMaxModel;								// 座標の最大値
}Coin;

typedef struct
{
	D3DXMATRIX mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 pos;				// 中心座標
	D3DXVECTOR3 rot;				// 回転角
	D3DXVECTOR3 vtxMinModel;		// 座標の最小値
	D3DXVECTOR3 vtxMaxModel;		// 座標の最大値
	D3DXVECTOR3 aPos[4];			// 線分の4点
	D3DXVECTOR3 aVecA[4];			// 線分のベクトル
	D3DXVECTOR3 aVecB[4];			// 線分のベクトル
	float fVec[4];					// 外積の値
	int nIdx;						// モデル識別番号
	int nIdxShadow;					// 何番目の影か
	bool bUse;						// 使用してるかどうか
}CoinSet;

typedef struct
{
	int nNumModel;					// モデルの総数
	CoinSet coinSet[MAX_COIN];		// モデル設置
	Coin coin[MAX_COIN];			// モデル情報
	int nCntCoin;					// 配置するコインの数
}CoinInfo;

//--------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------
HRESULT InitCoin(void);										// コインの初期化処理
void UninitCoin(void);										// コインの終了処理
void UpdateCoin(void);										// コインの更新処理
void DrawCoin(void);										// コインの描画処理

void TexCoin(int nCntCoin);									// コインのテクスチャ
void VecCoin(int nCntCoin);									// コインの頂点座標
void VecCoinSet(int nCntCoin);								// モデルの頂点座標の補正

bool CollisionCoin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);			// 当たり判定

void LoadCoinSetData(void);										// コイン配置情報の読み込み
bool GetbNoCoin(void);											// コインがあるかないか

#endif