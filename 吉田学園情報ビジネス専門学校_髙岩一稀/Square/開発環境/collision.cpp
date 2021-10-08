//=============================================================================
//
// �����蔻�菈�� [collision.cpp]
// Author : 
//
//=============================================================================
#include "collision.h"

//=============================================================================
// CCollision�̃R���X�g���N�^
//=============================================================================
CCollision::CCollision()
{
	
}

//=============================================================================
// CCollision�̃f�X�g���N�^
//=============================================================================
CCollision::~CCollision()
{

}

//=============================================================================
// ��ʊO���菈��
//=============================================================================
bool CCollision::OffScreen(D3DXVECTOR3 *pPos, D3DXVECTOR2 size)
{
	// �ϐ��錾
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