//-------------------------------------------------------------------
//
// ���[�V�����w�b�_�[ (motion.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	float fPosX;			// �ʒuX
	float fPosY;			// �ʒuY
	float fPosZ;			// �ʒuZ
	float fRotX;			// ����X
	float fRotY;			// ����Y
	float fRotZ;			// ����Z
}KEY;						// �L�[�̍\����

typedef struct
{
	int nFrame;				// �t���[����
	KEY aKey[10];			// �e���f���̃L�[�v�f
}KEY_INFO;					// �L�[�̍\����

typedef struct
{
	bool bLoop;				// ���[�v���邩�ǂ���
	int nNumKey;			// �L�[�̑���
	KEY_INFO aKeyInfo[5];	// �L�[���
}MOTION_INFO;				// ���[�V�����̍\����

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
void SetMotion(void);				// ���[�V�����̌��菈��
void NeutralMotion(void);			// �j���[�g�������[�V����

void LoadPlayerdata(void);				// �v���C���[���̓ǂݍ���

#endif