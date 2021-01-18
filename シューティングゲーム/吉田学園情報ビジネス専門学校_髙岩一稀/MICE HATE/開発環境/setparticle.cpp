//-------------------------------------------------------------------
//
// セットパーティクル (setparticle.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "setparticle.h"
#include "effect.h"

//-------------------------------------------------------------------
// 爆発
//-------------------------------------------------------------------
void SetBom(D3DXVECTOR3 pos,SETPARTICLE type)
{
	switch (type)
	{// 自機が攻撃を受けたとき
	case SETPARTICLE_HIT_PLAYER:
		for (int nCnt = 0; nCnt < 100; nCnt++)
		{
			// 角度の設定
			float fAngle = (float)(rand() % 628 - 314) / 100.0f;

			// エフェクトの設定
			SetEffect(pos,
				D3DXVECTOR3(sinf(fAngle) * (float)(rand() % 15 + 1), cosf(fAngle) * (float)(rand() % 15 + 1), 0.0f),
				D3DXCOLOR(0.95f, 0.4f, 0.1f, 1.0f),
				0.99f,
				(float)(rand() % 10 + 10),
				0.015f,
				60);
		}
		break;
		
		// 同色の弾が敵に当たったとき
	case SETPARTICLE_HIT_ENEMY:
		for (int nCnt = 0; nCnt < 50; nCnt++)
		{
			// 角度の設定
			float fAngle = (float)(rand() % 628 - 314) / 100.0f;

			// エフェクトの設定
			SetEffect(pos,
				D3DXVECTOR3(sinf(fAngle) * (float)(rand() % 10 + 3), cosf(fAngle) * (float)(rand() % 10 + 3), 0.0f),
				D3DXCOLOR(0.95f, 0.4f, 0.1f, 1.0f),
				0.93f,
				(float)(rand() % 10 + 5),
				0.015f,
				60);
		}
		break;

	case SETPARTICLE_ANYHIT_ENEMY:
		for (int nCnt = 0; nCnt < 20; nCnt++)
		{
			// 角度の設定
			float fAngle = (float)(rand() % 628 - 314) / 100.0f;

			// エフェクトの設定
			SetEffect(pos,
				D3DXVECTOR3(sinf(fAngle) * (float)(rand() % 5 + 1), cosf(fAngle) * (float)(rand() % 5 + 1), 0.0f),
				D3DXCOLOR(0.3f, 0.5f, 0.3f, 1.0f),
				0.93f,
				(float)(rand() % 10 + 5),
				0.015f,
				100);
		}
		break;
	default:
		break;
	}
}

//-------------------------------------------------------------------
// 弾の軌道
//-------------------------------------------------------------------
void SetBulletOrbit(D3DXVECTOR3 pos,D3DXCOLOR col, SETPARTICLE type)
{
	switch (type)
	{// 自機の弾の軌道
	case SETPARTICLE_PLAYER_BULLET:
		SetEffect(pos,D3DXVECTOR3(0.0f,0.0f, 0.0f),col,1.0f,8,0.06f,100);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), col,1.0f, 7, 0.06f, 100);
		break;
		// 敵の弾の軌道
	case SETPARTICLE_ENEMY_BULLET:
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.2f, 0.01f, 0.6f, 1.0f),1.0f,8, 0.05f,100);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 1.0f, 6, 0.05f,100);
		break;
	default:
		break;
	}

}

//-------------------------------------------------------------------
// 移動時の軌道
//-------------------------------------------------------------------
void SetMoveOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, SETPARTICLE type)
{
	switch (type)
	{// 自機の軌道
	case SETPARTICLE_MOVE_PLAYER:
		SetEffect(pos,move, col, 0.7f, 20, 0.1f, 50);
		break;
		// 敵の軌道
	case SETPARTICLE_MOVE_ENEMY:
		SetEffect(pos,move,col, 1.0f, 10, 0.01f, 100);
		break;
	default:
		break;
	}
}