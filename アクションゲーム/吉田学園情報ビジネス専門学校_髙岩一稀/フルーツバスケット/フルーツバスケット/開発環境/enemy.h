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
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 posOld;	// �O�̈ʒu
	D3DXVECTOR3 move;	// �ړ���
	D3DXVECTOR3 rot;	// ��]
	int nCntBound;		// ���˕Ԃ�����
	float fWidth;		// ��
	float fHeight;		// ����
	int nType;			// �^�C�v
	bool bUse;			// �g�p���Ă��邩�ǂ���
}ENEMY;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitEnemy(void);										// �G�̏���������
void UninitEnemy(void);											// �G�̏I������
void UpdateEnemy(void);											// �G�̍X�V����
void DrawEnemy(void);											// �G�̕`�揈��
void HitEnemy(int nCntEnemy);									// �G�̓����蔻��
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int type);		// �G�̔z�u
void SetVertexEnemy(int nIdx);									// ���_���W�̐ݒ�
ENEMY *GetEnemy(void);

#endif

