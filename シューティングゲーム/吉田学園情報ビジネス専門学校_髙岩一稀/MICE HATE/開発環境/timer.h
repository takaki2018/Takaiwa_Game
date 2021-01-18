//-------------------------------------------------------------------
//
// �^�C�}�[�w�b�_�[ (timer.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitTimer(void);					// �^�C�}�[�̏���������
void UninitTimer(void);						// �^�C�}�[�̏I������
void UpdateTimer(void);						// �^�C�}�[�̍X�V����
void DrawTimer(void);						// �^�C�}�[�̕`�揈��
int GetTimer(void);							// �^�C�}�[���̎擾
int GetCntSetEnemy(void);					// �G��z�u����Ԋu�̎擾

#endif

