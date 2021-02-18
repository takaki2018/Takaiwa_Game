//-------------------------------------------------------------------
//
// �o���b�g�w�b�_�[ (bullet.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitBullet(void);										// �e�̏���������
void UninitBullet(void);										// �e�̏I������
void UpdateBullet(void);										// �e�̍X�V����
void DrawBullet(void);											// �e�̕`�揈��

void SetVertexBullet(int nIdx);									// �e�̒��_���W	
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight);	// �e�̔z�u

#endif