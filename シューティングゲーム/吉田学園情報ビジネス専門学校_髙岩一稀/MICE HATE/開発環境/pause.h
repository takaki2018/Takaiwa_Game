//-------------------------------------------------------------------
//
// �|�[�Y�w�b�_�[ (pause.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//-------------------------------------------------------------------
// �|�[�Y�̏��
//-------------------------------------------------------------------
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,	// �R���e�B�j���[
	PAUSE_MENU_RETRY,			// ���g���C
	PAUSE_MENU_QUIT,			// �I��
	PAUSE_MENU_MAX,
}PAUSE_MENU;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitPause(void);				// �|�[�Y�̏���������
void UninitPause(void);					// �|�[�Y�̏I������
void UpdatePause(void);					// �|�[�Y�̍X�V����
void DrawPause(void);					// �|�[�Y�̕`�揈��
void SetVertexPause(int nCntPause);		// �|�[�Y��ʂ̒��_���W
void PauseMenu(int nCnt);				// �|�[�Y���j���[

#endif