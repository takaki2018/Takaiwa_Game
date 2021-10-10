//==============================================================================================================
//
// AI (AI.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "AI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define AI_MOVE				(0.5f)	// �ړ���
#define AI_MOVE_BULLET		(4.0f)	// �e�̈ړ���

//=============================================================================
// CAI�̃R���X�g���N�^
//=============================================================================
CAi::CAi()
{

}

//=============================================================================
// CAI�̃f�X�g���N�^
//=============================================================================
CAi::~CAi()
{

}

//=============================================================================
// �T���s��
// ����		: pos			- �T���s���s���I�u�W�F�N�g�̈ʒu
//			: targetPos		- �ǂ���������I�u�W�F�N�g�̈ʒu
//			: fMove			- �ړ���
// �Ԃ�l	: D3DXVECTOR3	- �ړ��ʂ�Ԃ�
//=============================================================================
D3DXVECTOR3 CAi::Seek(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fMove)
{
	// �ϐ��錾
	D3DXVECTOR3 move;		// �ۑ��p

	// �ړI�̕��������߂�
	D3DXVECTOR3 Vec = targetPos - pos;

	// �p�x�����߂�
	float fAngle = atan2f(Vec.x, Vec.y);

	// �v���C���[��ǔ�����
	move.x = fMove * sinf(fAngle);
	move.y = fMove * cosf(fAngle);

	return move;
}

//=============================================================================
// �����s��
// ����		: pos			- �����s�����s���I�u�W�F�N�g�̈ʒu
//			: targetPos		- ��������I�u�W�F�N�g�̈ʒu
// �Ԃ�l	: D3DXVECTOR3	- �ړ��ʂ�Ԃ�
//=============================================================================
D3DXVECTOR3 CAi::Flee(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos)
{
	// �ϐ��錾
	D3DXVECTOR3 move;		// �ۑ��p

	// �ړI�̕��������߂�
	D3DXVECTOR3 Vec = pos - targetPos;

	// �p�x�����߂�
	float fAngle = atan2f(Vec.x, Vec.y);

	// ���@�ƒ��S�_�̑Ίp���̒���
	float fLength = sqrtf((Vec.x * Vec.x) + (Vec.y * Vec.y));

	if (fLength < 200.0f)
	{
		// �v���C���[���瓦����
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
// �e�̎�������
// ����		: pos			- �e�����I�u�W�F�N�g�̈ʒu
//			: targetPos		- �e�𓖂Ă����I�u�W�F�N�g�̈ʒu
// �Ԃ�l	: D3DXVECTOR3	- �e�̈ړ��ʂ�Ԃ�
//=============================================================================
D3DXVECTOR3 CAi::AimShoot(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos)
{
	// �ϐ��錾
	D3DXVECTOR3 move;		// �ۑ��p

	// �ړI�̕��������߂�
	D3DXVECTOR3 Vec = targetPos - pos;

	// �p�x�����߂�
	float fAngle = atan2f(Vec.x, Vec.y);

	// �v���C���[��_��
	move.x = AI_MOVE_BULLET * sinf(fAngle);
	move.y = AI_MOVE_BULLET * cosf(fAngle);

	return move;
}