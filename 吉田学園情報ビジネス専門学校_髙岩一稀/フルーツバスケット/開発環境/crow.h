//-------------------------------------------------------------------
//
// �J���X�w�b�_�[ (crow.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _CROW_H_
#define _CROW_H_

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
}CROW;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitCrow(void);								// �J���X�̏���������
void UninitCrow(void);								// �J���X�̏I������
void UpdateCrow(void);								// �J���X�̍X�V����
void DrawCrow(void);								// �J���X�̕`�揈��
void SetVertexCrow(int nCnt);						// ���_���W�̐ݒ�
void SetCrow(D3DXVECTOR3 pos, D3DXVECTOR3 move);	// �J���X�̔z�u

CROW *GetCrow(void);									// �J���X���̎擾

#endif