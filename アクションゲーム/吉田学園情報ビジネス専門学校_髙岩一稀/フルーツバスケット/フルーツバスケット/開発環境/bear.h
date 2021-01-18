//-------------------------------------------------------------------
//
// �N�}�w�b�_�[ (bear.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _BEAR_H_
#define _BEAR_H_

#include "main.h"

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 posOld;	// �O��̈ʒu
	D3DXVECTOR3 move;	// �ړ���
	D3DXVECTOR3 rot;	// ��]
	float fWidth;		// ��
	float fHeight;		// ����
	float fTexU;		// �e�N�X�`�����WU�̒l
	float fTexV;		// �e�N�X�`�����WV�̒l
	int nAppTime;		// ����鎞��
	int nJampCnt;		// �W�����v����܂ł̃J�E���g
	int nTurnCnt;		// �^�[������܂ł̃J�E���g
	bool bJump;			// �W�����v���Ă��邩�ǂ���
	bool bLand;			// �u���b�N�ɏ���Ă��邩�ǂ���
	bool bEvent;		// �C�x���g���ǂ���
	bool bAppear;		// �o�ꂵ�����ǂ���
	bool bUse;			// �g�p���Ă��邩�ǂ���
}BEAR;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitBear(void);										// �N�}�̏���������
void UninitBear(void);										// �N�}�̏I������
void UpdateBear(void);										// �N�}�̍X�V����
void DrawBear(void);										// �N�}�̕`�揈��

void TitleBear(void);										// �^�C�g����ʂ̃N�}
void GameBear(void);										// �Q�[����ʂ̃N�}
void HitBear(void);											// �N�}�̓����蔻��
void SetBear(D3DXVECTOR3 pos, D3DXVECTOR3 move);			// �N�}�̔z�u
void SetVertexBear(void);									// ���_���W�̐ݒ�

float GetShaking(void);										// ��ʗh����̎擾

#endif