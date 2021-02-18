//-------------------------------------------------------------------
//
// モデルヘッダー (model.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMesh;			// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;		// マテリアル(材質情報)へのポインタ
	DWORD nNumMat = 0;			// マテリアルの数
	D3DXVECTOR3 pos;			// 中心座標
	D3DXVECTOR3 posOld;			// 前の位置
	D3DXVECTOR3 rot;			// 回転角
	D3DXVECTOR3 rotDest;		// 目的の回転角
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 move;			// 移動量
	int nIdx;					// 何番目の影か
}TestModel;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitModel(void);			// モデルの初期化処理
void UninitModel(void);				// モデルの終了処理
void UpdateModel(void);				// モデルの更新処理
void DrawModel(void);				// モデルの描画処理
TestModel *GetModel(void);		// モデルの位置情報の取得

#endif