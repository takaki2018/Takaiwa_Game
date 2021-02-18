//-------------------------------------------------------------------
//
// �R�C���w�b�_�[ (coin.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _COIN_H_
#define _COIN_H_

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
	D3DXVECTOR3 vtxMinModel;			// ���W�̍ŏ��l
	D3DXVECTOR3 vtxMaxModel;			// ���W�̍ő�l
	int nIdx;						// ���Ԗڂ̉e��
	D3DXVECTOR3 aPos[4];			// ������4�_
	D3DXVECTOR3 aVecA[4];			// �����̃x�N�g��
	D3DXVECTOR3 aVecB[4];			// �����̃x�N�g��
	float fVec[4];					// �O�ς̒l
	bool bUse;						// �g�p���Ă��邩�ǂ���
}Coin;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitCoin(void);		// �R�C���̏���������
void UninitCoin(void);		// �R�C���̏I������
void UpdateCoin(void);		// �R�C���̍X�V����
void DrawCoin(void);			// �R�C���̕`�揈��

void SetCoin(D3DXVECTOR3 pos);			// �R�C���̐ݒu
void TexCoin(int nCntCoin);				// �R�C���̃e�N�X�`��
void VecCoin(int nCntCoin);				// �R�C���̒��_���W
void CrossProductCoin(int nCntCoin);	// �O�ς�p�����R�C���̓����蔻��

bool CollisionCoin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);		// �����蔻��

#endif