//-------------------------------------------------------------------
//
// �Q�[���w�b�_�[ (enemy.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _z_H_
#define _GAME_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitGame(void);			// �Q�[����ʂ̏���������
void UninitGame(void);			// �Q�[����ʂ̏I������
void UpdateGame(void);			// �Q�[����ʂ̍X�V����
void DrawGame(void);			// �Q�[����ʂ̕`�揈��
void NormalMode(void);			// �m�[�}�����[�h�̏���
void EndlessMode(void);			// �G���h���X���[�h�̏���
void SetPause(bool bPause);		// �|�[�Y���Ă邩�ǂ���

#endif
