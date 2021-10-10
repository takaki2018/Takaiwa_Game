//==============================================================================================================
//
// AI (AI.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "AI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define AI_MOVE				(0.5f)	// 移動量
#define AI_MOVE_BULLET		(4.0f)	// 弾の移動量

//=============================================================================
// CAIのコンストラクタ
//=============================================================================
CAi::CAi()
{

}

//=============================================================================
// CAIのデストラクタ
//=============================================================================
CAi::~CAi()
{

}

//=============================================================================
// 探索行動
// 引数		: pos			- 探索行動行うオブジェクトの位置
//			: targetPos		- 追いかけられるオブジェクトの位置
//			: fMove			- 移動量
// 返り値	: D3DXVECTOR3	- 移動量を返す
//=============================================================================
D3DXVECTOR3 CAi::Seek(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fMove)
{
	// 変数宣言
	D3DXVECTOR3 move;		// 保存用

	// 目的の方向を求める
	D3DXVECTOR3 Vec = targetPos - pos;

	// 角度を求める
	float fAngle = atan2f(Vec.x, Vec.y);

	// プレイヤーを追尾する
	move.x = fMove * sinf(fAngle);
	move.y = fMove * cosf(fAngle);

	return move;
}

//=============================================================================
// 逃避行動
// 引数		: pos			- 逃避行動を行うオブジェクトの位置
//			: targetPos		- 避けられるオブジェクトの位置
// 返り値	: D3DXVECTOR3	- 移動量を返す
//=============================================================================
D3DXVECTOR3 CAi::Flee(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos)
{
	// 変数宣言
	D3DXVECTOR3 move;		// 保存用

	// 目的の方向を求める
	D3DXVECTOR3 Vec = pos - targetPos;

	// 角度を求める
	float fAngle = atan2f(Vec.x, Vec.y);

	// 自機と中心点の対角線の長さ
	float fLength = sqrtf((Vec.x * Vec.x) + (Vec.y * Vec.y));

	if (fLength < 200.0f)
	{
		// プレイヤーから逃げる
		move.x = AI_MOVE * sinf(fAngle);
		move.y = AI_MOVE * cosf(fAngle);
	}
	else
	{
		move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return move;
}

//=============================================================================
// 弾の自動発射
// 引数		: pos			- 弾を撃つオブジェクトの位置
//			: targetPos		- 弾を当てたいオブジェクトの位置
// 返り値	: D3DXVECTOR3	- 弾の移動量を返す
//=============================================================================
D3DXVECTOR3 CAi::AimShoot(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos)
{
	// 変数宣言
	D3DXVECTOR3 move;		// 保存用

	// 目的の方向を求める
	D3DXVECTOR3 Vec = targetPos - pos;

	// 角度を求める
	float fAngle = atan2f(Vec.x, Vec.y);

	// プレイヤーを狙う
	move.x = AI_MOVE_BULLET * sinf(fAngle);
	move.y = AI_MOVE_BULLET * cosf(fAngle);

	return move;
}