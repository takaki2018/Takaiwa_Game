//-------------------------------------------------------------------
//
// �I�u�W�F�N�g�w�b�_�[ (object.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitObject(void);		// �|���S���̏���������
void UninitObject(void);		// �|���S���̏I������
void UpdateObject(void);		// �|���S���̍X�V����
void DrawObject(void);			// �|���S���̕`�揈��
D3DXVECTOR3 GetposObject(void);	// �I�u�W�F�N�g�̍��W���̎擾

#endif
