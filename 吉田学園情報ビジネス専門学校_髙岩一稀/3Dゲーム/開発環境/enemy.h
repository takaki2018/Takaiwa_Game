//-------------------------------------------------------------------
//
// �G�l�~�[�w�b�_�[ (enemy.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
	D3DXVECTOR3 pos;				// ���W
	D3DXVECTOR3 posOld;				// �O��̍��W
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
}Enemy;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitEnemy(void);									// �G�̏���������
void UninitEnemy(void);										// �G�̏I������
void UpdateEnemy(void);										// �G�̍X�V����
void DrawEnemy(void);										// �G�̕`�揈��

void MoveEnemy(int nCntEnemy);								// �G�̈ړ�����
void SetEnemy(D3DXVECTOR3 pos);								// �G�̐ݒu
void TexEnemy(int nCntCoin);								// �G�̃e�N�X�`��
void VecEnemy(int nCntCoin);								// �G�̒��_���W
void CrossProductEnemy(int nCntCoin);						// �O�ςɕK�v�ȃx�N�g���̌v�Z

void DeleteEnemy(int nCntEnemy);							// �G���������鏈��

bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pMove,D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);			// �����蔻��

#endif