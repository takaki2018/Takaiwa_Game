//==============================================================================================================
//
// �`���[�g���A���pPressOK�w�b�_�[ (tutorial_press_ok.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _TUTORIALPRESSOK_H_
#define _TUTORIALPRESSOK_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	float fWidth;			// ��
	float fHeight;			// ����
	float fMoveAlpha;		// ���l�̈ړ�
	int nFlashCnt;			// �_�ŃJ�E���g
	bool bDisPlay;			// �\�����邩�ǂ���
	bool bPressOK;			// ���������ǂ���
	bool bGameTrasition;	// �Q�[����ʂɑJ�ڂ��邩�ǂ���
}TutorialPressOK;

//----------------------------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//----------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialPressOK(void);		// �`���[�g���A���pPressOK�̏���������
void UninitTutorialPressOK(void);		// �`���[�g���A���pPressOK�̏I������
void UpdateTutorialPressOK(void);		// �`���[�g���A���pPressOK�̍X�V����
void DrawTutorialPressOK(void);			// �`���[�g���A���pPressOK�̕`�揈��

void BeforePressingOK(void);			// �`���[�g���A���pPressOK�������O�̏���
void AfterPreesingOK(void);				// �`���[�g���A���pPressOK����������̏���

void SetColTutorialPressOK(void);		// �`���[�g���A���pPressOK�̐F�̐ݒ�
TutorialPressOK GetPressOKInfo(void);	// �`���[�g���A���pPressOK���̎擾

#endif
