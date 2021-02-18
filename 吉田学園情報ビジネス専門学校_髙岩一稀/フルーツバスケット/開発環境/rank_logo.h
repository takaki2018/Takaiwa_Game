//-------------------------------------------------------------------
//
// �����N���S�w�b�_�[ (rank_logo.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RANKLOGO_H_
#define _RANKLOGO_H_

#include "main.h"

//-------------------------------------------------------------------
// �����L���O����
//-------------------------------------------------------------------
typedef enum
{
	RANK_1 = 0,		// 1��
	RANK_2,			// 2��
	RANK_3,			// 3��
	RANK_4,			// 4��
	RANK_5,			// 5��
	RANK_MAX,
}RANK;

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	RANK nTexType;		// �e�N�X�`���̎��
	float fWidth;		// ��
	float fHeight;		// ����
	bool bUse;			// �g�p���Ă��邩�ǂ���
}RANKLOGO;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitRankLogo(void);														// �����N���S�̏���������
void UninitRankLogo(void);														// �����N���S�̏I������
void UpdateRankLogo(void);														// �����N���S�̍X�V����
void DrawRankLogo(void);														// �����N���S�̕`�揈��
void SetVErtexRankLogo(int nCntRankLogo);										// �����N���S�̒��_���W�̐ݒ�
void SetRankLogo(D3DXVECTOR3 pos, float fWidth,float fHeight, RANK nTexType);	// ���S�̐ݒu
void CangeRankLogo(int nCntRankLogo, RANK nTexType);							// �����N���S�̕ύX

#endif