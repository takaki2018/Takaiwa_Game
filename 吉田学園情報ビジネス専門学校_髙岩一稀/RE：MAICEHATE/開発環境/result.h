//-------------------------------------------------------------------
//
// ���U���g�w�b�_�[ (title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RESULT_H_
#define _RESuLT_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitResult(void);				// ���U���g�̏���������
void UninitResult(void);				// ���U���g�̏I������
void UpdateResult(void);				// ���U���g�̍X�V����
void DrawResult(void);					// ���U���g�̕`�揈��
void SetVertexResult(int nCntResult);	// ���U���g��ʂɓ\��e�N�X�`���̐ݒ�

#endif

