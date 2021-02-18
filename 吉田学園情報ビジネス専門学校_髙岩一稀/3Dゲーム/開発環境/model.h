//-------------------------------------------------------------------
//
// ���f���w�b�_�[ (model.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMesh;			// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;		// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD nNumMat = 0;			// �}�e���A���̐�
	D3DXVECTOR3 pos;			// ���S���W
	D3DXVECTOR3 posOld;			// �O�̈ʒu
	D3DXVECTOR3 rot;			// ��]�p
	D3DXVECTOR3 rotDest;		// �ړI�̉�]�p
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 move;			// �ړ���
	int nIdx;					// ���Ԗڂ̉e��
}TestModel;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitModel(void);			// ���f���̏���������
void UninitModel(void);				// ���f���̏I������
void UpdateModel(void);				// ���f���̍X�V����
void DrawModel(void);				// ���f���̕`�揈��
TestModel *GetModel(void);		// ���f���̈ʒu���̎擾

#endif