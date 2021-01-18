//-------------------------------------------------------------------
//
// �Z�b�g�p�[�e�B�N���w�b�_�[ (setparticle.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETPARTICLE_H_
#define _SETPARTICLE_H_

#include "main.h"

//-------------------------------------------------------------------
// �Z�b�g�p�[�e�B�N���̎��
//-------------------------------------------------------------------
typedef enum
{
	SETPARTICLE_MOVE_PLAYER = 0,	// ���@�̓���
	SETPARTICLE_MOVE_ENEMY,			// �G�̓���
	SETPARTICLE_MAX,
}SETPARTICLE;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
void SetMoveOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 move, SETPARTICLE type);	// �L�����N�^�[�̋O��

#endif
