//=============================================================================
//
// 当たり判定処理 [collision.h]
// Author : 
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//-----------------------------------------------------------------------------
// 当たり判定クラス(基本クラス)
//-----------------------------------------------------------------------------
class CCollision
{
public:
	CCollision();
	~CCollision();
	static bool OffScreen(D3DXVECTOR3 *pPos,D3DXVECTOR2 size);
};
#endif