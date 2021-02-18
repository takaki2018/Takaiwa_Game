//-------------------------------------------------------------------
//
// �o���b�g�w�b�_�[ (bullet.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//-----------------------------------------------------------------
// �e�̏��
//-----------------------------------------------------------------
typedef enum
{
	BULLETCOLOR_WHITE = 0,	// ��
	BULLETCOLOR_RED,		// ��
	BULLETCOLOR_BLUE,		// ��
	BULLETCOLOR_MAX,
}BULLETCOLOR;

typedef enum
{
	BULLETTYPE_PLAYER = 0,	// �v���C���[���g�p
	BULLETTYPE_ENEMY,		// �G���g�p
	BULLETTYPE_MAX,
}BULLETTYPE;
//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	int nColType;			// �F�̃^�C�v
	BULLETTYPE type;		// ���
	bool bUse;				// �g�p���Ă��邩�ǂ���
} BULLET;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitBullet(void);																		// �e�̏���������
void UninitBullet(void);																		// �e�̏I������
void UpdateBullet(void);																		// �e�̍X�V����
void DrawBullet(void);																			// �e�̕`�揈
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,int nBulletColor,int nLife,BULLETTYPE type);	// �e�̐ݒ�
void SetVertexBullet(int nIdx);																	// �e�̒��_���W
BULLET *GetBullet(void);																		// �e���̎擾
int GetBulletcol(void);																			// ���݂̒e�̐F

#endif