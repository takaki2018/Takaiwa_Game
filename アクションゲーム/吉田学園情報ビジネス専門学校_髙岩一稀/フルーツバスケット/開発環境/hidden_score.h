//-------------------------------------------------------------------
//
// �͂Ăȃ}�[�N�w�b�_�[ (hidden_score.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _HIDDENSCORE_H_
#define _HIDDENSCORE_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitHiddenScore(void);								// �͂Ăȃ}�[�N�̏���������
void UninitHiddenScore(void);								// �͂Ăȃ}�[�N�̏I������
void UpdateHiddenScore(void);								// �͂Ăȃ}�[�N�̍X�V����
void DrawHiddenScore(void);									// �͂Ăȃ}�[�N�̕`�揈��

void DisplayQuestionMark(void);								// �͂Ăȃ}�[�N��\�����鏈��

#endif
