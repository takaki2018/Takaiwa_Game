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
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMeshModel;			// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMatModel;		// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD nNumMatModel = 0;			// �}�e���A���̐�
	D3DXMATRIX mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;				// ���S���W
	D3DXVECTOR3 rot;				// ��]�p
	D3DXVECTOR3 rotDest;			// �ړI�̉�]�p
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 moverot;			// ��]��
	int nIdx;						// ���Ԗڂ̉e��
	bool bMove;						// �ړ��������ǂ���
}Object;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitObject(void);		// �I�u�W�F�N�g�̏���������
void UninitObject(void);		// �I�u�W�F�N�g�̏I������
void UpdateObject(void);		// �I�u�W�F�N�g�̍X�V����
void DrawObject(void);			// �I�u�W�F�N�g�̕`�揈��

void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);					// �����蔻��

#endif