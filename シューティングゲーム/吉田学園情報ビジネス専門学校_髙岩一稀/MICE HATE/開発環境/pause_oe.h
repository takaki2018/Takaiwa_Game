//-------------------------------------------------------------------
//
// �|�[�YOE�w�b�_�[ (pause_oe.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PAUSEOE_H_
#define _PAUSEOE_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitPauseOe(void);			// �|�[�YOE�̏���������
void UninitPauseOe(void);			// �|�[�YOE�̏I������
void UpdatePauseOe(void);			// �|�[�YOE�̍X�V����
void DrawPauseOe(void);				// �|�[�YOE�̕`�揈��
void SetVertexPauseOe(int nCnt);	// �|�[�YOE�̒��_���W�̐ݒ�
void PauseOeImage(int nCnt);		// �|�[�YOE�̉摜
void PauseOeSelect(int nCnt);		// �|�[�YOE�̃Z���N�g

#endif
