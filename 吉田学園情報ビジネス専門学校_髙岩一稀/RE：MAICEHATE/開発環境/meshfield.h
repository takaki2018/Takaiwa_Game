//-------------------------------------------------------------------
//
// メッシュフィールドヘッダー (meshfield.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 pos;			// 壁の中心座標
	D3DXVECTOR3 rot;			// 壁の向き
	int nCntBlockX;				// ブロック数(列)
	int nCntBlockZ;				// ブロック数(行)
	int nCntVecX;				// 列の頂点数
	int nCntVecZ;				// 行の頂点数
	float fWidth;				// 幅
	float fDepth;				// 奥行
	float fsizeWidth;			// 幅のサイズ
	float fsizeDepth;			// 奥行のサイズ
	bool bUse;					// 使用しているかどうか
}MeshField;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitMeshField(void);									// メッシュフィールドの初期化処理
void UninitMeshField(void);										// メッシュフィールドの終了処理
void UpdateMeshField(void);										// メッシュフィールドの更新処理
void DrawMeshField(void);										// メッシュフィールドの描画処理
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, int nIdxShadow);// 当たり判定
void SetMeshField(D3DXVECTOR3 pos, float fWidth, float fDepth);	// メッシュフィールドの設置

#endif