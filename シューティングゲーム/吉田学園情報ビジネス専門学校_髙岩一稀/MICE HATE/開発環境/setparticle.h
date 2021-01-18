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
	SETPARTICLE_HIT_PLAYER,			// ���@���U�����󂯂��Ƃ�
	SETPARTICLE_HIT_ENEMY,			// ���F�̒e�����������Ƃ�
	SETPARTICLE_ANYHIT_ENEMY,		// �ِF�̒e�����������Ƃ�
	SETPARTICLE_PLAYER_BULLET,		// ���@�̒e�̋O��
	SETPARTICLE_ENEMY_BULLET,		// �G�̒e�̋O��
	SETPARTICLE_MAX,
}SETPARTICLE;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
void SetBom(D3DXVECTOR3 pos,SETPARTICLE type);											// ����
void SetBulletOrbit(D3DXVECTOR3 pos,D3DXCOLOR col, SETPARTICLE type);					// �e�̋O��
void SetMoveOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, SETPARTICLE type);	// �L�����N�^�[�̋O��

#endif
