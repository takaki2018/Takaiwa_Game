//=============================================================================
//
// �Z�b�g�p�[�e�B�N�� (setparticle.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#include "setparticle.h"
#include "effect.h"

//=============================================================================
// ����
//=============================================================================
void CParticle::SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR col, float fRemoveAlpha, D3DXVECTOR2 sizeSplit)
{
	// �ϐ��錾
	float fSizeX = size.x / sizeSplit.x;
	float fSizeY = size.y / sizeSplit.y;
	float fAngle;
	float fmove;

	for (int nCnt = 0; nCnt < (int)sizeSplit.y; nCnt++)
	{
		for (int nCntX = 0; nCntX < (int)sizeSplit.x; nCntX++)
		{
			// �ړ��ʂƊp�x�̐ݒ�
			fmove = (float)(rand() % 10 + 5) * 0.5f;
			fAngle = (float)(rand() % 628) / 100.0f;

			CEffect::Create(D3DXVECTOR3(pos.x + fSizeX * (float)((sizeSplit.x / 2 - nCntX) * 2 - 1), pos.y - size.y + fSizeY * (float)((nCnt + 1) * 2 - 1), 0.0f),
				D3DXVECTOR3(sinf(fAngle) * fmove, cosf(fAngle) * fmove, 0.0f),
				col,
				1.0f,
				D3DXVECTOR2(fSizeX * 2.0f, fSizeY * 2.0f),
				fRemoveAlpha,
				1000,
				CEffect::EFFECT_TYPE_TILE,
				0);
		}
	}
}

//=============================================================================
// �^�C�c�̔j��
//=============================================================================
void CParticle::SetFillTile(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR col, BulletMove bulletMove, D3DXVECTOR2 sizeSplit)
{
	// �ϐ��錾
	float fSizeX = size.x / sizeSplit.x;
	float fSizeY = size.y / sizeSplit.y;
	float fAngle;
	float fmove;

	for (int nCnt = 0; nCnt < (int)sizeSplit.y; nCnt++)
	{
		for (int nCntX = 0; nCntX < (int)sizeSplit.x; nCntX++)
		{
			// �ړ��ʂ̐ݒ�
			fmove = (float)(rand() % 10 + 5) * 0.5f;

			// �e�����������ɂ���ď��������
			switch (bulletMove)
			{
				// �ォ��e�������Ƃ�
			case BULLETMOVE_UP:
				fAngle = ((float)((rand() % 314) - (float)(628 / 2 - 157))) / 100.0f;

				CEffect::Create(D3DXVECTOR3(pos.x + fSizeX * (float)((sizeSplit.x / 2 - nCntX) * 2 - 1), pos.y - size.y + fSizeY * (float)((nCnt + 1) * 2 - 1), 0.0f),
					D3DXVECTOR3(sinf(fAngle) * fmove, cosf(fAngle) * fmove, 0.0f),
					col,
					1.0f,
					D3DXVECTOR2(fSizeX * 2.0f, fSizeY * 2.0f),
					0.03f,
					1000,
					CEffect::EFFECT_TYPE_TILE,
					0);
				break;

				// ������e�������Ƃ�
			case BULLETMOVE_DOWN:
				fAngle = ((float)((rand() % 314) - (float)(628 / 2 - 471))) / 100.0f;

				CEffect::Create(D3DXVECTOR3(pos.x + fSizeX * (float)((sizeSplit.x / 2 - nCntX) * 2 - 1), pos.y + size.y - fSizeY * (float)((nCnt + 1) * 2 - 1), 0.0f),
					D3DXVECTOR3(sinf(fAngle) * fmove, cosf(fAngle) * fmove, 0.0f),
					col,
					1.0f,
					D3DXVECTOR2(fSizeX * 2.0f, fSizeY * 2.0f),
					0.03f,
					1000,
					CEffect::EFFECT_TYPE_TILE,
					0);
				break;

				// �E����e�������Ƃ�
			case BULLETMOVE_RIGHT:
				fAngle = ((float)((rand() % 314) - (float)(628 / 2 - 0))) / 100.0f;

				CEffect::Create(D3DXVECTOR3(pos.x + size.x - fSizeX * (float)((nCntX + 1) * 2 - 1), pos.y + fSizeY * (float)((sizeSplit.y / 2 - nCnt) * 2 - 1), 0.0f),
					D3DXVECTOR3(sinf(fAngle) * fmove, cosf(fAngle) * fmove, 0.0f),
					col,
					1.0f,
					D3DXVECTOR2(fSizeX * 2.0f, fSizeY * 2.0f),
					0.03f,
					1000,
					CEffect::EFFECT_TYPE_TILE,
					0);
				break;

				// ������e�������Ƃ�
			case BULLETMOVE_LEFT:
				fAngle = ((float)((rand() % 314) - (float)(628 / 2 - 314))) / 100.0f;

				CEffect::Create(D3DXVECTOR3(pos.x - size.x + fSizeX * (float)((nCntX + 1) * 2 - 1), pos.y + fSizeY * (float)((sizeSplit.y / 2 - nCnt) * 2 - 1), 0.0f),
					D3DXVECTOR3(sinf(fAngle) * fmove, cosf(fAngle) * fmove, 0.0f),
					col,
					1.0f,
					D3DXVECTOR2(fSizeX * 2.0f, fSizeY * 2.0f),
					0.03f,
					1000,
					CEffect::EFFECT_TYPE_TILE,
					0);
				break;
			}
		}
	}
}