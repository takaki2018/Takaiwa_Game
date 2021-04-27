//-------------------------------------------------------------------
//
// �J�E���g�_�E���w�b�_�[ (count_down.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include "main.h"

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
	int nTExType;		// �e�N�X�`���̎��
	int nCntAlufa;		// ���l�̈ړ��l
	float fCntSize;		// �T�C�Y�̈ړ��l
	bool bUse;			// �g�p���Ă��邩�ǂ���
}COUNTDOWN;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitCountDown(void);						// �J�E���g�_�E���̏���������
void UninitCountDown(void);							// �J�E���g�_�E���̏I������
void UpdateCountDown(void);							// �J�E���g�_�E���̍X�V����
void DrawCountDown(void);							// �J�E���g�_�E���̕`�揈��

void CountDownNumber(void);							// �J�E���g�_�E���̐�������
void CountDownStart(void);							// �J�E���g�_�E���̃X�^�[�g����

void SetVertexCountDown(void);						// �J�E���g�_�E���̒��_���W�̐ݒ�
void SetVertexCountDownNumber(VERTEX_2D *pVtx);		// �J�E���g�_�E���̐����̒��_���W�̐ݒ�
void SetVertexCountDownStart(VERTEX_2D *pVtx);		// �J�E���g�_�E���̃X�^�[�g�̒��_���W�̐ݒ�
bool GetFinishCountDown(void);						// �J�E���g�_�E�����I��������ǂ����擾

#endif