//-------------------------------------------------------------------
//
// �Z�b�g�p�[�e�B�N�� (setparticle.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "setparticle.h"
#include "effect.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define Appear		(30)				// �o����
#define MOVE		(2)				// �ړ���
#define HEIGHT		(3)			// ����
#define RCOL		(0.9f)			// R����
#define GCOL		(0.7f)			// G����
#define BCOL		(0.2f)			// B����
#define RANDIUS		(5)				// ���a
#define ALPHA		(0.02f)		// ���l�̌�����
#define GRAVITY		(0.15f)			// �d��
#define BOUND		(0.8f)			// ���˕Ԃ�W��

//-------------------------------------------------------------------
// �O���[�o���ϐ�
//-------------------------------------------------------------------
int g_nCnt = 0;

//-------------------------------------------------------------------
// �e�X�g
//-------------------------------------------------------------------
void CoinParticle(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < Appear; nCnt++)
	{// �p�x�̐ݒ�
		float fAngle = ((float)(rand() % 628 - 314)) / 100.0f;
		float fmove = (float)(rand() % MOVE + 1);

		// �G�t�F�N�g�̐ݒ�
		SetEffect(D3DXVECTOR3(pos.x, pos.y, pos.z),
			D3DXVECTOR3(sinf(fAngle) * fmove, (float)(rand() % HEIGHT + 3), cosf(fAngle) * fmove),
			D3DXCOLOR(RCOL, GCOL, BCOL, 1.0f),
			1.0f,
			(float)(rand() % 10 + RANDIUS),
			ALPHA,
			GRAVITY,
			BOUND);
	}
}

//-------------------------------------------------------------------
// ���n��
//-------------------------------------------------------------------
void SetLand(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < 30; nCnt++)
	{// �p�x�̐ݒ�
		float fAngle = ((float)(rand() % 628 - 314)) / 100.0f;
		float fmove = (float)(rand() % 5 + 2);

		// �G�t�F�N�g�̐ݒ�
		SetEffect(D3DXVECTOR3(pos.x, pos.y, pos.z),
			D3DXVECTOR3(sinf(fAngle) * fmove, (float)(rand() % 2), cosf(fAngle) * fmove),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			0.9f,
			(float)(rand() % 5 + RANDIUS),
			0.02f,
			0.05f,
			BOUND);
	}
}