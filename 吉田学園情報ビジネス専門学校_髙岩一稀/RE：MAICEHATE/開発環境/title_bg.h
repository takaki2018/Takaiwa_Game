//-------------------------------------------------------------------
//
// �^�C�g���w�i�w�b�_�[ (title_bg.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _TITLEBG_H_
#define _TITLEBG_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitTitleBg(void);		 // �|���S���̏���������
void UninitTitleBg(void);		 // �|���S���̏I������
void UpdateTitleBg(void);		// �|���S���̍X�V����
void DrawTitleBg(void);			// �|���S���̕`�揈��
void SetVertexTitleBg(int nCnt); // ���_���W�̐ݒ�

#endif