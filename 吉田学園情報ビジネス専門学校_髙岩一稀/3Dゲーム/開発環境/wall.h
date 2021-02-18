//-------------------------------------------------------------------
//
// �ǃw�b�_�[ (wall.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_WALL		(128)		// �p�ӂ���ǃf�[�^�̃T�C�Y

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;			// �ǂ̒��S���W
	D3DXVECTOR3 rot;			// �ǂ̌���
	int nCntWallX;				// �ǂ̐�(��)
	int nCntWallZ;				// �ǂ̐�(�s)
	int nCntVecX;				// ��̒��_��
	int nCntVecZ;				// �s�̒��_��
	float fWidth;				// ��
	float fDepth;				// ���s
	float fsizeWidth;			// ���̃T�C�Y
	float fsizeDepth;			// ���s�̃T�C�Y
	D3DXVECTOR3 aPos;			// ������4�_
	D3DXVECTOR3 aVecA;			// �����̃x�N�g��
	D3DXVECTOR3 aVecB;			// �����̃x�N�g��
	float fVec;					// ���ς̒l
	bool bUse;					// �g�p���Ă��邩�ǂ���
}Wall;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitWall(void);								// �ǂ̏���������
void UninitWall(void);								// �ǂ̏I������
void UpdateWall(void);								// �ǂ̍X�V����
void DrawWall(void);								// �ǂ̕`�揈��

void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// �ǂ̔z�u
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, 
	D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);					// �ǂƂ̓����蔻��
Wall *GetWall(void);									// �Ǐ��̎擾

#endif