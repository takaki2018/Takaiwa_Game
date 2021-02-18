//-------------------------------------------------------------------
//
// セットパーティクル (setparticle.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "setparticle.h"
#include "effect.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define Appear		(1)				// 出現量
#define MOVE		(3)				// 移動量
#define HEIGHT		(10)			// 高さ
#define RCOL		(0.9f)			// R成分
#define GCOL		(0.7f)			// G成分
#define BCOL		(0.2f)			// B成分
#define RANDIUS		(5)				// 半径
#define ALPHA		(0.008f)		// α値の減少量
#define GRAVITY		(0.8f)			// 重力
#define BOUND		(0.8f)			// 跳ね返り係数

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
int g_nCnt = 0;

//-------------------------------------------------------------------
// テスト
//-------------------------------------------------------------------
void SetParticle(void)
{
	g_nCnt++;

	if (g_nCnt % 1 == 0)
	{
		for (int nCnt = 0; nCnt < Appear; nCnt++)
		{// 角度の設定
			float fAngle = ((float)(rand() % 628 - 314)) / 100.0f;
			float fmove = (float)(rand() % MOVE + 1);

			// エフェクトの設定
			SetEffect(D3DXVECTOR3(0.0f, 50.0f, 0.0f),
				D3DXVECTOR3(sinf(fAngle) * fmove, HEIGHT, cosf(fAngle) * fmove),
				D3DXCOLOR(RCOL, GCOL, BCOL, 1.0f),
				1.0f,
				(float)(rand() % 10 + RANDIUS),
				ALPHA,
				GRAVITY,
				BOUND);
		}
	}
}

//-------------------------------------------------------------------
// 弾の軌道
//-------------------------------------------------------------------
void SetOrbitBullet(D3DXVECTOR3 pos)
{

}