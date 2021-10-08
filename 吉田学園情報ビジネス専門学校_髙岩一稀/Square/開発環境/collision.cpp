//=============================================================================
//
// 当たり判定処理 [collision.cpp]
// Author : 
//
//=============================================================================
#include "collision.h"

//=============================================================================
// CCollisionのコンストラクタ
//=============================================================================
CCollision::CCollision()
{
	
}

//=============================================================================
// CCollisionのデストラクタ
//=============================================================================
CCollision::~CCollision()
{

}

//=============================================================================
// 画面外判定処理
//=============================================================================
bool CCollision::OffScreen(D3DXVECTOR3 *pPos, D3DXVECTOR2 size)
{
	// 変数宣言
	bool bCollision = false;

	if (pPos->x - size.x <= 0.0f)
	{
		pPos->x = size.x;

		bCollision = true;
	}
	if (pPos->x + size.x >= SCREEN_WIDTH)
	{
		pPos->x = SCREEN_WIDTH - size.x;

		bCollision = true;
	}
	if (pPos->y - size.y <= 0.0f)
	{
		pPos->y = size.y;

		bCollision = true;
	}
	if (pPos->y + size.y >= SCREEN_HEIGHT)
	{
		pPos->y = SCREEN_HEIGHT - size.y;

		bCollision = true;
	}

	return bCollision;
}