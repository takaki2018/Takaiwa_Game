#//-------------------------------------------------------------------
//
// �Ռ��g�w�b�_�[ (shockwave.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SHOCKWAVE_H_
#define _SHOCKWAVE_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitShockwave(void);										// �Ռ��g�̏���������
void UninitShockwave(void);											// �Ռ��g�̏I������
void UpdateShockwave(void);											// �Ռ��g�̍X�V����
void DrawShockwave(void);											// �Ռ��g�̕`�揈��
void SetShockwave(D3DXVECTOR3 pos,D3DXVECTOR3 size ,D3DXCOLOR col, 
	float fSize,float fNB, float fNB_MAG,float nCntAlpha);			// �Ռ��g�̐ݒ�
void SetVertexShockwave(int nIdx);									// �Ռ��g�̒��_���W�̐ݒ�
float ShockwaveMove(void);											// �m�b�N�o�b�N�l�̎擾								

#endif

