//-------------------------------------------------------------------
//
// �G�t�F�N�g�w�b�_�[ (effect.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitEffect(void);											// �G�t�F�N�g�̏���������
void UninitEffect(void);											// �G�t�F�N�g�̏I������
void UpdateEffect(void);											// �G�t�F�N�g�̍X�V����
void DrawEffect(void);												// �G�t�F�N�g�̕`�揈
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fRemove,float fRandius,float fCntAlpha, int nLife);		// �G�t�F�N�g�̐ݒ�

#endif
