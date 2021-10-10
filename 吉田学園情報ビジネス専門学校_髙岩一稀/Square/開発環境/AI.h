//=============================================================================
//
// AI�w�b�_�[ (AI.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"

//*****************************************************************************
// AI�N���X(��{�N���X)
//*****************************************************************************
class CAi
{
public:
	CAi();
	~CAi();

	static D3DXVECTOR3 Seek(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos,float fMove);			// �T���s��
	static D3DXVECTOR3 Flee(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);						// �����s��
	static D3DXVECTOR3 AimShoot(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);					// �e�̎�������
};

#endif