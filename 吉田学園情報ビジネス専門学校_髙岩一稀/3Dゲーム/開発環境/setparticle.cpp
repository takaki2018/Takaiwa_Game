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
#define Appear		(1)				// �o����
#define MOVE		(3)				// �ړ���
#define HEIGHT		(10)			// ����
#define RCOL		(0.9f)			// R����
#define GCOL		(0.7f)			// G����
#define BCOL		(0.2f)			// B����
#define RANDIUS		(5)				// ���a
#define ALPHA		(0.008f)		// ���l�̌�����
#define GRAVITY		(0.8f)			// �d��
#define BOUND		(0.8f)			// ���˕Ԃ�W��

//-------------------------------------------------------------------
// �O���[�o���ϐ�
//-------------------------------------------------------------------
int g_nCnt = 0;

//-------------------------------------------------------------------
// �e�X�g
//-------------------------------------------------------------------
void SetParticle(void)
{
	g_nCnt++;

	if (g_nCnt % 1 == 0)
	{
		for (int nCnt = 0; nCnt < Appear; nCnt++)
		{// �p�x�̐ݒ�
			float fAngle = ((float)(rand() % 628 - 314)) / 100.0f;
			float fmove = (float)(rand() % MOVE + 1);

			// �G�t�F�N�g�̐ݒ�
			SetEffect(D3DXVECTOR3(0.0f, 50.0f, 0.0f),
				D3DXVECTOR3(sinf(fAngle) * fmove, HEIGHT, cosf(fAngle) * fmove),
				D3DXCOLOR(RCOL, GCOL, BCOL, 1.0f),
				1.0f,
				(float)(rand() % 10 + RANDIUS),
				ALPHA,
				GRAVITY,
				BOUND);
		}
	}
}

//-------------------------------------------------------------------
// �e�̋O��
//-------------------------------------------------------------------
void SetOrbitBullet(D3DXVECTOR3 pos)
{

}