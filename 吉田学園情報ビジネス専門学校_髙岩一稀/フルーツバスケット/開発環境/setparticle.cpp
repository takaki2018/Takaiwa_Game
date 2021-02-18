//-------------------------------------------------------------------
//
// セットパーティクル (setparticle.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "setparticle.h"
#include "effect.h"

//-------------------------------------------------------------------
// 移動時の軌道
//-------------------------------------------------------------------
void SetMoveOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 move, SETPARTICLE type)
{
	switch (type)
	{// 自機の軌道
	case SETPARTICLE_MOVE_PLAYER:
		if (move.x < 0.0f)
		{// 角度の設定
			float fAngle = (float)(rand() % 32 - 16) / 100.0f;

			// エフェクトの設定
			SetEffect(pos,
				D3DXVECTOR3(sinf(fAngle) * (float)(rand() % 2 + 1), cosf(fAngle) * (float)(rand() % 2 + 1), 0.0f),
				D3DXCOLOR(0.95f, 0.4f, 0.1f, 1.0f),
				0.99f,
				5,
				0.015f,
				10);
		}
		else if (move.x > 0.0f)
		{// 角度の設定
			float fAngle = (float)(rand() % 32 - 16) / 100.0f;

			// エフェクトの設定
			SetEffect(pos,
				D3DXVECTOR3(sinf(fAngle) * (float)(rand() % 2 + 1), cosf(fAngle) * (float)(rand() % 2 + 1), 0.0f),
				D3DXCOLOR(0.95f, 0.4f, 0.1f, 1.0f),
				0.99f,
				5,
				0.015f,
				10);
		}
		break;
	}
}