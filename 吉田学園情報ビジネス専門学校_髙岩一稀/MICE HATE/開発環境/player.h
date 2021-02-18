//-------------------------------------------------------------------
//
// �v���C���[�w�b�_�[ (player.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���C���[�̏��
//-------------------------------------------------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,	// �ʏ���	
	PLAYERSTATE_HIT,		// �v���C���[���G�ɓ����������
	PLAYERSTATE_FLASHING,	// �_��
	PLAYERSTATE_MAX,
}PLAYERSTATE;

//-------------------------------------------------------------------
// �v���C���[�̍\����
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// �ʒu
	D3DXVECTOR3 move;	// �ړ���
	PLAYERSTATE state;	// ���
	int nCounterState;	// ��ԃJ�E���^
	int nCounterDisp;	// �\���J�E���^
	bool bDisp;			// �\��ON/OFF 
} PLAYER;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitPlayer(void);						// �v���C���[�̏���������
void UninitPlayer(void);						// �v���C���[�̏I������
void UpdatePlayer(void);						// �v���C���[�̍X�V����
void DrawPlayer(void);							// �v���C���[�̕`�揈��
void MovePlayer(void);							// �v���C���[�̈ړ�����
void PlayerStateNormal(void);					// �v���C���[��Ԃ��ʏ�̂Ƃ�
void PlayerStateHit(void);						// �v���C���[��Ԃ��U�����󂯂��Ƃ�
void PlayerStateFlashing(void);					// �v���C���[��Ԃ��_�ł̂Ƃ�
bool HitPlayer(int nIdx);						// �G�ɓ����������ǂ���
PLAYER *GetPlayer(void);						// �v���C���[���̎擾

#endif