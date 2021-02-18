//-------------------------------------------------------------------
//
// �|���S���w�b�_�[ (polygon.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;			// �ǂ̒��S���W
	D3DXVECTOR3 rot;			// �ǂ̌���
	float fWidth;				// ��
	float fDepth;				// ���s
	bool bUse;					// �g�p���Ă��邩�ǂ���
}Floor;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitPolygon(void);										// �|���S���̏���������
void UninitPolygon(void);										// �|���S���̏I������
void UpdatePolygon(void);										// �|���S���̍X�V����
void DrawPolygon(void);											// �|���S���̕`�揈��

void SetPolygon(D3DXVECTOR3 pos, float fWidth, float fDepth);	// �|���S���̐ݒu
bool CollisionFloor(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);				// ���̓����蔻��
Floor *Getpolygon(void);										// �|���S�����̎擾

#endif