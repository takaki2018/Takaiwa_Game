//==============================================================================================================
//
// AIヘッダー (AI.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// AIクラス(基本クラス)
//--------------------------------------------------------------------------------------------------------------
class CAi
{
public:
	CAi();
	~CAi();

	typedef enum
	{
		fast = 1,
		normal,
		slow,
	}Decelerrion;	// スピード

	static D3DXVECTOR3 Seek(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos,float fMove);					// 探索行動
	static D3DXVECTOR3 Flee(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);								// 逃避行動
	static D3DXVECTOR3 Arrive(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, Decelerrion decelerrion);		// 到着行動
	static D3DXVECTOR3 AimShoot(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);					// 弾の自動発射
};

#endif