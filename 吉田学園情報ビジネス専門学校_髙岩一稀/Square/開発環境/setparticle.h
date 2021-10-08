//-------------------------------------------------------------------
//
// セットパーティクルヘッダー (setparticle.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETPARTICLE_H_
#define _SETPARTICLE_H_

#include "main.h"

//-------------------------------------------------------------------
// パーティクルクラス（基本クラス）
//-------------------------------------------------------------------
class CParticle
{
public:
	typedef enum
	{
		BULLETMOVE_UP = 0,
		BULLETMOVE_DOWN,
		BULLETMOVE_RIGHT,
		BULLETMOVE_LEFT,
		BULLETMOVE_MAX,
	}BulletMove;

	static void SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR col,float fRemoveAlpha, D3DXVECTOR2 sizeSplit);			// 爆発
	static void SetFillTile(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR2 size,D3DXCOLOR col, BulletMove bulletMove,D3DXVECTOR2 sizeSplit);			// タイルの破壊
	static void SetRevivalTile(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR col, BulletMove bulletMove, D3DXVECTOR2 sizeSplit);	// タイルの復活
	static void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR col);														// 軌跡
};

#endif
