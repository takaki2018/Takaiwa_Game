//-------------------------------------------------------------------
//
// �Z�b�g�p�[�e�B�N�� (setparticle.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "setparticle.h"
#include "effect.h"

//-------------------------------------------------------------------
// �ړ����̋O��
//-------------------------------------------------------------------
void SetMoveOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 move, SETPARTICLE type)
{
	switch (type)
	{// ���@�̋O��
	case SETPARTICLE_MOVE_PLAYER:
		if (move.x < 0.0f)
		{// �p�x�̐ݒ�
			float fAngle = (float)(rand() % 32 - 16) / 100.0f;

			// �G�t�F�N�g�̐ݒ�
			SetEffect(pos,
				D3DXVECTOR3(sinf(fAngle) * (float)(rand() % 2 + 1), cosf(fAngle) * (float)(rand() % 2 + 1), 0.0f),
				D3DXCOLOR(0.95f, 0.4f, 0.1f, 1.0f),
				0.99f,
				5,
				0.015f,
				10);
		}
		else if (move.x > 0.0f)
		{// �p�x�̐ݒ�
			float fAngle = (float)(rand() % 32 - 16) / 100.0f;

			// �G�t�F�N�g�̐ݒ�
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