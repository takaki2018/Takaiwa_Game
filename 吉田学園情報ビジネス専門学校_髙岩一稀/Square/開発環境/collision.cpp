//=============================================================================
//
// 当たり判定処理 [collision.cpp]
// Author : itsuki takaiwa
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
// 引数		: *pPos	- オブジェクトの位置情報ポインタ
//			: size	- サイズ情報
// 返り値	: bool	- 当たったかどうか返す
//=============================================================================
bool CCollision::OffScreen(D3DXVECTOR3 *pPos, D3DXVECTOR2 size)
{
	// 変数宣言
	bool bCollision = false;	// 当たったかどうか
		
	// 画面左
	if (pPos->x - size.x <= 0.0f)
	{
		pPos->x = size.x;

		bCollision = true;
	}

	// 画面右
	if (pPos->x + size.x >= SCREEN_WIDTH)
	{
		pPos->x = SCREEN_WIDTH - size.x;

		bCollision = true;
	}

	// 画面上
	if (pPos->y - size.y <= 0.0f)
	{
		pPos->y = size.y;

		bCollision = true;
	}

	// 画面下
	if (pPos->y + size.y >= SCREEN_HEIGHT)
	{
		pPos->y = SCREEN_HEIGHT - size.y;

		bCollision = true;
	}

	return bCollision;
}