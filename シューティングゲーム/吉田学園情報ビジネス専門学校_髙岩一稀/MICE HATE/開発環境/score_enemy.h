//-------------------------------------------------------------------
//
// �G�X�R�A�w�b�_�[ (score_enemy.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SCOREENEMY_H_
#define _SCOREENEMY_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitScoreEnemy(void);							// �X�R�A�̏���������
void UninitScoreEnemy(void);							// �X�R�A�̏I������
void UpdateScoreEnemy(void);							// �X�R�A�̍X�V����
void DrawScoreEnemy(void);								// �X�R�A�̕`�揈��
void SetScoreEnemy(D3DXVECTOR3 pos, D3DXCOLOR col);		// �X�R�A�̃Z�b�g
void SetVertexScoreEnemy(int nCnt);						// ���_���W�̐ݒ�

#endif
