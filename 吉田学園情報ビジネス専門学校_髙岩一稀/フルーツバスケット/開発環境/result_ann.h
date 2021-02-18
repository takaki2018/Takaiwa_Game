//-------------------------------------------------------------------
//
// ���ʔ��\�w�b�_�[ (result_ann.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RESULTANN_H_
#define _RESULTANN_H_

#include "main.h"

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
	float fWidth;		// ��
	float fHeight;		// ����
	int nCounterState;	// ��ԃJ�E���^
	bool bTurn;			// �ړ��������ǂ���
	bool bSound;		// �T�E���h���g�p�������ǂ���
	bool bUse;			// �g�p���Ă��邩�ǂ���
}RESULTANN;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitResultAnn(void);						// ���ʔ��\�̏���������
void UninitResultAnn(void);							// ���ʔ��\�̏I������
void UpdateResultAnn(void);							// ���ʔ��\�̍X�V����
void DrawResultAnn(void);							// ���ʔ��\�̕`�揈��
void SetVErtexResultAnn(void);						// ���ʔ��\�̒��_���W�̐ݒ�
bool GetResultAnn(void);							// ���ʔ��\�̏��̎擾

#endif