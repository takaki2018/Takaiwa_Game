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
#define Appear		(30)				// 出現量
#define MOVE		(2)				// 移動量
#define HEIGHT		(3)			// 高さ
#define RCOL		(0.9f)			// R成分
#define GCOL		(0.7f)			// G成分
#define BCOL		(0.2f)			// B成分
#define RANDIUS		(5)				// 半径
#define ALPHA		(0.02f)		// α値の減少量
#define GRAVITY		(0.15f)			// 重力
#define BOUND		(0.8f)			// 跳ね返り係数

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
int g_nCnt = 0;

//-------------------------------------------------------------------
// テスト
//-------------------------------------------------------------------
void CoinParticle(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < Appear; nCnt++)
	{// 角度の設定
		float fAngle = ((float)(rand() % 628 - 314)) / 100.0f;
		float fmove = (float)(rand() % MOVE + 1);

		// エフェクトの設定
		SetEffect(D3DXVECTOR3(pos.x, pos.y, pos.z),
			D3DXVECTOR3(sinf(fAngle) * fmove, (float)(rand() % HEIGHT + 3), cosf(fAngle) * fmove),
			D3DXCOLOR(RCOL, GCOL, BCOL, 1.0f),
			1.0f,
			(float)(rand() % 10 + RANDIUS),
			ALPHA,
			GRAVITY,
			BOUND);
	}
}

//-------------------------------------------------------------------
// 着地時
//-------------------------------------------------------------------
void SetLand(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < 30; nCnt++)
	{// 角度の設定
		float fAngle = ((float)(rand() % 628 - 314)) / 100.0f;
		float fmove = (float)(rand() % 5 + 2);

		// エフェクトの設定
		SetEffect(D3DXVECTOR3(pos.x, pos.y, pos.z),
			D3DXVECTOR3(sinf(fAngle) * fmove, (float)(rand() % 2), cosf(fAngle) * fmove),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			0.9f,
			(float)(rand() % 5 + RANDIUS),
			0.02f,
			0.05f,
			BOUND);
	}
}