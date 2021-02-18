//-------------------------------------------------------------------
//
// �r���{�[�h�w�b�_�[ (billboard.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitBillboard(void);						// �r���{�[�h�̏���������
void UninitBillboard(void);							// �r���{�[�h�̏I������
void UpdateBillboard(void);							// �r���{�[�h�̍X�V����
void DrawBillboard(void);							// �r���{�[�h�̕`�揈��
void SetVertexBillboard(int nCntBillboard);			// �r���{�[�h�̒��_���W�̐ݒ�

void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fDepth);					// �r���{�[�h�̔z�u

#endif