//-------------------------------------------------------------------
//
// ���b�V���t�B�[���h�w�b�_�[ (meshfield.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;			// �ǂ̒��S���W
	D3DXVECTOR3 rot;			// �ǂ̌���
	int nCntBlockX;				// �u���b�N��(��)
	int nCntBlockZ;				// �u���b�N��(�s)
	int nCntVecX;				// ��̒��_��
	int nCntVecZ;				// �s�̒��_��
	float fWidth;				// ��
	float fDepth;				// ���s
	float fsizeWidth;			// ���̃T�C�Y
	float fsizeDepth;			// ���s�̃T�C�Y
	bool bUse;					// �g�p���Ă��邩�ǂ���
}MeshField;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitMeshField(void);									// ���b�V���t�B�[���h�̏���������
void UninitMeshField(void);										// ���b�V���t�B�[���h�̏I������
void UpdateMeshField(void);										// ���b�V���t�B�[���h�̍X�V����
void DrawMeshField(void);										// ���b�V���t�B�[���h�̕`�揈��
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, int nIdxShadow);// �����蔻��
void SetMeshField(D3DXVECTOR3 pos, float fWidth, float fDepth);	// ���b�V���t�B�[���h�̐ݒu

#endif