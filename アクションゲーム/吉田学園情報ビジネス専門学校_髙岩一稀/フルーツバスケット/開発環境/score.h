//-------------------------------------------------------------------
//
// �X�R�A�w�b�_�[ (score.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitScore(void);								// �X�R�A�̏���������
void UninitScore(void);									// �X�R�A�̏I������
void UpdateScore(void);									// �X�R�A�̍X�V����
void DrawScore(void);									// �X�R�A�̕`�揈��

void HiddenScore(void);									// �X�R�A���B������
void AddScore(int nCntPlayer,int nValue);				// �X�R�A�̉��Z
int GetScore(int nCntScore);							// �X�R�A�̎擾

#endif
