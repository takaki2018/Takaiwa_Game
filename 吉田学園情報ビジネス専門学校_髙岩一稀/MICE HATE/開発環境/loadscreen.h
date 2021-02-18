//-------------------------------------------------------------------
//
// ���[�h��ʃw�b�_�[ (loadscreen.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _LOADSCREEN_H_
#define _LOADSCREEN_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitLoadScreen(void);			// ���[�h��ʂ̏���������
void UninitLoadScreen(void);			// ���[�h��ʂ̏I������
void UpdateLoadScreen(void);			// ���[�h��ʂ̍X�V����
void DrawLoadScreen(void);				// ���[�h��ʂ̕`�揈��
void SetVertexLoadScreen(int nCnt);		// ���[�h��ʂ̒��_���W�̐ݒ�
void LoadScreenBg(int nCnt);			// ���[�h��ʂ̔w�i
void LoadScreenImage(int nCnt);			// ���[�h��ʂ̉摜
void LoadScreenSelect(int nCnt);		// ���[�h��ʂ̃Z���N�g
void LoadScreenPressOK(int nCnt);		// ���[�h��ʂ̃v���XOK

#endif
