//==============================================================================================================
//
// 3D�G�t�F�N�g�w�b�_�[ (3Deffect.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _3DEFFECT_H_
#define _3DEFFECT_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------------------------------------------------------------------
HRESULT Init3DEffect(void);											// �G�t�F�N�g�̏���������
void Uninit3DEffect(void);											// �G�t�F�N�g�̏I������
void Update3DEffect(void);											// �G�t�F�N�g�̍X�V����
void Draw3DEffect(void);												// �G�t�F�N�g�̕`�揈
void Set3DEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fRemove,float fRandius,float fCntAlpha, float fGravity,
	float fBound);	// �G�t�F�N�g�̐ݒ�

#endif
