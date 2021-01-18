//-------------------------------------------------------------------
//
// �t���[�c�w�b�_�[ (fruits.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _FRUITS_H_
#define _FRUITS_H_

#include "main.h"

//-------------------------------------------------------------------
// �񋓌^
//-------------------------------------------------------------------
typedef enum
{
	FRUITSTYPE_APPLE = 0,	// �����S
	FRUITSTYPE_BLUEAPPLE,	// �����S
	FRUITSTYPE_PEACH,		// �s�[�`
	FRUITSTYPE_ORANGE,		// �I�����W
	FRUITSTYPE_GOLDAPPLE,	// �������S
	FRUITSTYPE_MAX,			// �t���[�c�̂̎�ސ�
}FRUITSTYPE;				// �t���[�c�̎��

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
	FRUITSTYPE nType;	// �^�C�v
	bool bLand;			// �u���b�N�ɏ�������ǂ���
	bool bUse;			// �g�p���Ă��邩�ǂ���
}FRUITS;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitFruits(void);											// �t���[�c�̏���������
void UninitFruits(void);											// �t���[�c�̏I������
void UpdateFruits(void);											// �t���[�c�̍X�V����
void DrawFruits(void);												// �t���[�c�̕`�揈��

void TitleFruits(void);												// �^�C�g����ʂ̃t���[�c
void GameFruits(void);												// �Q�[����ʂ̃t���[�c

void SetFruitsTitle(void);											// �^�C�g����ʂ̔z�u���
void SetFruitsGame(void);											// �Q�[����ʂ̃t���[�c�̔z�u���

void HitFruits(int nCntFruits);										// �t���[�c�̓����蔻��

void SetFruits(D3DXVECTOR3 pos, D3DXVECTOR3 move, FRUITSTYPE type);	// �t���[�c�̔z�u
void SetVertexFruits(int nIdx);										// ���_���W�̐ݒ�
FRUITS *GetFruits(void);											// �t���[�c���̎擾
bool GetbTitleFruits(void);											// �^�C�g����ʂ̃t���[�c�J�E���g�̎擾
int GetTitleFruitsCnt(void);										// �^�C�g����ʂ̃t���[�c�J�E���g�̎擾

#endif
