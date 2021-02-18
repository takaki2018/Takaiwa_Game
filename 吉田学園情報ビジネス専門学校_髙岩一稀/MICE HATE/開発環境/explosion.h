//-------------------------------------------------------------------
//
// �����w�b�_�[ (explotion.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitExplosion(void);										// �e�̏���������
void UninitExplosion(void);											// �e�̏I������
void UpdateExplosion(void);											// �e�̍X�V����
void DrawExplosion(void);											// �e�̕`�揈
void SetExplosion(D3DXVECTOR3 pos,D3DCOLOR col);						// �e�̐ݒ�
void SetVertexExplosion(int nIdx);									// �e�̒��_���W

#endif