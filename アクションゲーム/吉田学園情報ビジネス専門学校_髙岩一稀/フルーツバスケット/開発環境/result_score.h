//-------------------------------------------------------------------
//
// ���U���g�X�R�A�w�b�_�[ (score.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitResultScore(void);								// ���U���g�X�R�A�̏���������
void UninitResultScore(void);								// ���U���g�X�R�A�̏I������
void UpdateResultScore(void);								// ���U���g�X�R�A�̍X�V����
void DrawResultScore(void);									// ���U���g�X�R�A�̕`�揈��
void AddResultScore(int nCntPlayer, int nValue);			// ���U���g�X�R�A�̉��Z

#endif
