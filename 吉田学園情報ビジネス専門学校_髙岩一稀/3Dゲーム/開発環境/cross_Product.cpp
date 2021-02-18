//-----------------------------------------------------------------
//
// 外積を用いた当たり判定 (cross_product.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "cross_product.h"
#include "player.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define POS_0_X			(50.0f)		// Pos[0]のX座標
#define POS_0_Y			(0.0f)			// Pos[0]のY座標
#define POS_0_Z			(50.0f)			// Pos[0]のZ座標

#define POS_1_X			(-50.0f)		// Pos[1]のX座標
#define POS_1_Y			(0.0f)			// Pos[1]のY座標
#define POS_1_Z			(0.0f)		// Pos[1]のZ座標

#define POS_X			(120.0f)			// posのX座標
#define POS_Y			(0.0f)			// posのY座標
#define POS_Z			(100.0f)			// posのZ座標

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
CrossProduct g_CrossProduct;			// テスト用

//-----------------------------------------------------------------
// 外積を用いた当たり判定テスト
//-----------------------------------------------------------------
void CrossProductTest(void)
{
	// 構造体のポインタ変数
	Player *pPlayer = GetPlayer();

	// 変数の初期化
	g_CrossProduct.aPos[0] = D3DXVECTOR3(POS_0_X, POS_0_Y, POS_0_Z);
	g_CrossProduct.aPos[1] = D3DXVECTOR3(POS_1_X, POS_1_Y, POS_1_Z);
	g_CrossProduct.pos = pPlayer->pos;
	g_CrossProduct.vecA = D3DXVECTOR3(POS_0_X - POS_1_X, POS_0_Y - POS_1_Y, POS_0_Z - POS_1_Z);
	g_CrossProduct.vecB = D3DXVECTOR3(POS_0_X - pPlayer->pos.x, POS_0_Y - pPlayer->pos.y, POS_0_Z - pPlayer->pos.z);
	g_CrossProduct.fVec = (g_CrossProduct.vecA.z * g_CrossProduct.vecB.x) - (g_CrossProduct.vecA.x * g_CrossProduct.vecB.z);
}

//-----------------------------------------------------------------
// 情報の取得
//-----------------------------------------------------------------
CrossProduct *GetCrossProduct(void)
{
	return &g_CrossProduct;
}