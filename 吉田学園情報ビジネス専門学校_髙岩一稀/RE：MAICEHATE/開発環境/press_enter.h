//==============================================================================================================
//
// �v���X�G���^�[�w�b�_�[ (title.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _PRESSENTER_H_
#define _PRESSENTER_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------------------------------------------------------------------
HRESULT InitPressEnter(void);		// �v���X�G���^�[�̏���������
void UninitPressEnter(void);		// �v���X�G���^�[�̏I������
void UpdatePressEnter(void);		// �v���X�G���^�[�̍X�V����
void DrawPressEnter(void);			// �v���X�G���^�[�̕`�揈��

void BeforePressingEnter(void);		// �G���^�[�L�[�������O�̏���
void AfterPreesingEnter(void);		// �G���^�[�L�[����������̏���

void SetVertexPressEnter(void);		// �v���X�G���^�[�̒��_���W�̐ݒ�
bool bGetTutorialTransition(void);	// �`���[�g���A����ʂɑJ�ڂ��邩�ǂ����n��
#endif
