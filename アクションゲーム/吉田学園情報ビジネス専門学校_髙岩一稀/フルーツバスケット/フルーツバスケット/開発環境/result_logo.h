//-------------------------------------------------------------------
//
// ���U���g���S�w�b�_�[ (result_logo.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include "main.h"

//-------------------------------------------------------------------
// ���ʂ̃e�N�X�`����
//-------------------------------------------------------------------
typedef enum
{
	RANK_FIRST = 0,	// ���
	RANK_SECOND,	// ���
	RANK_THIRD,		// �O��
	RANK_FOURTH,	// �l��
	RANK_MAX,
}RANK;

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
	RANK nTexType;		// �e�N�X�`���̎��
	float fHeight;		// �Q�[�W�̍���
	bool bUse;			// �g�p���Ă��邩�ǂ���
}RESULTLOGO;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitResultLogo(void);										// ���U���g���S�̏���������
void UninitResultLogo(void);										// ���U���g���S�̏I������
void UpdateResultLogo(void);										// ���U���g���S�̍X�V����
void DrawResultLogo(void);											// ���U���g���S�̕`�揈��
void SetVErtexResultLogo(int nCntResultLogo);						// ���U���g���S�̒��_���W�̐ݒ�
void SetResultLogo(D3DXVECTOR3 pos,float fHeight, RANK nTexType);	// ���S�̐ݒu

#endif