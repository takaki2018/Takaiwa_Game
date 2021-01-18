//-------------------------------------------------------------------
//
// �|�C���g�w�b�_�[ (point.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _POINT_H_
#define _POINT_H_

#include "main.h"

//-------------------------------------------------------------------
// �񋓌^
//-------------------------------------------------------------------
typedef enum
{
	POINTTYPE_PLUS_ONE = 0,		// �v���X1���S
	POINTTYPE_PLUS_FIVE,	// �v���X5���S
	POINTTYPE_DECREASE_ONE,		// �}�C�i�X1���S
	POINTTYPE_DECREASE_FIVE,	// �}�C�i�X5���S
	POINTTYPE_MAX,				// �l���|�C���g���S�̍ő吔
}POINTTYPE;						// �l���|�C���g

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fWidth;			// ��
	float fHeight;			// ����
	POINTTYPE type;			// ���S�̃^�C�v
	bool bUse;				// �g�p���Ă��邩�ǂ���
}POINTLOGO;					// �l���|�C���g�̍\����

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitPoint(void);											// �|�C���g�̏���������
void UninitPoint(void);												// �|�C���g�̏I������
void UpdatePoint(void);												// �|�C���g�̍X�V����
void DrawPoint(void);												// �|�C���g�̕`�揈��
void SetPoint(D3DXVECTOR3 pos, D3DXVECTOR3 move, POINTTYPE type);	// �|�C���g�̔z�u
void SetVertexPoint(int nIdx);										// ���_���W�̐ݒ�

#endif