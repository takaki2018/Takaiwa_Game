//-------------------------------------------------------------------
//
// �^�C�g���w�b�_�[ (title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitTitle(void);				// �^�C�g���̏���������
void UninitTitle(void);					// �^�C�g���̏I������
void UpdateTitle(void);					// �^�C�g���̍X�V����
void DrawTitle(void);					// �^�C�g���̕`�揈��
void SetVertexTexture(int nCntTitle);	// �^�C�g����ʂɓ\��e�N�X�`���̎w��

#endif
