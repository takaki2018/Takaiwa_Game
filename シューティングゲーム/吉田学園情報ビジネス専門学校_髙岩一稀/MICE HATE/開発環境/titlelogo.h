//-------------------------------------------------------------------
//
// �^�C�g�����S�w�b�_�[ (titlelogo.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitTitlelogo(void);				// �^�C�g�����S�̏���������
void UninitTitlelogo(void);					// �^�C�g�����S�̏I������
void UpdateTitlelogo(void);					// �^�C�g�����S�̍X�V����
void DrawTitlelogo(void);					// �^�C�g�����S�̕`�揈��
bool GetFall(void);							// �^�C�g�����S�̏��̎擾

#endif