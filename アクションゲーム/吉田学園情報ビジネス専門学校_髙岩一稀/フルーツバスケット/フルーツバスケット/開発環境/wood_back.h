//-------------------------------------------------------------------
//
// �؂̗��w�b�_�[ (wood.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _WOODBACK_H_
#define _WOODBACK_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitWoodBack(void);			// �؂̗��̏���������
void UninitWoodBack(void);			// �؂̗��̏I������
void UpdateWoodBack(void);			// �؂̗��̍X�V����
void DrawWoodBack(void);			// �؂̗��̕`�揈��
void SetVertexWoodBack(int nCnt);	// ���_���W�̐ݒ�

#endif