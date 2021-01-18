//-------------------------------------------------------------------
//
// �l�Y�~�w�b�_�[ (crow.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RAT_H_
#define _RAT_H_

#include "main.h"

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 move;	// �ړ���
	float fWidth;		// ��
	float fHeight;		// ����
	int nCntAnim;		// �A�j���[�V�����J�E���g
	int nPatternAnim;	// �A�j���[�V�����p�^�[��
	bool bUse;			// �g�p���Ă��邩�ǂ���
}RAT;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitRat(void);							// �l�Y�~�̏���������
void UninitRat(void);							// �l�Y�~�̏I������
void UpdateRat(void);							// �l�Y�~�̍X�V����
void DrawRat(void);								// �l�Y�~�̕`�揈��
void HitRat(void);								// �l�Y�~�̓����蔻��
void SetVertexRat(int nCnt);					// ���_���W�̐ݒ�
void SetRat(D3DXVECTOR3 pos, D3DXVECTOR3 move);	// �l�Y�~�̔z�u

#endif