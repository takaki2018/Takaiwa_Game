//==============================================================================================================
//
// 2D�G�t�F�N�g�w�b�_�[ (2Deffect.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _2DEFFECT_H_
#define _2DEFFECT_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------------------------------------------------------------------
HRESULT Init2DEffect(void);											// �G�t�F�N�g�̏���������
void Uninit2DEffect(void);											// �G�t�F�N�g�̏I������
void Update2DEffect(void);											// �G�t�F�N�g�̍X�V����
void Draw2DEffect(void);												// �G�t�F�N�g�̕`�揈
void Set2DEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fRemove, float fRandius, float fCntAlpha, float fGravity,
	float fBound);	// �G�t�F�N�g�̐ݒ�

#endif