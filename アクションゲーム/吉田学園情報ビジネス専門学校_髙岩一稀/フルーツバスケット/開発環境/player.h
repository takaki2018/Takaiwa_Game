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
// �}�N����`
//-------------------------------------------------------------------
#define PLAYER_SIZE_WIDTH	(45)		// �v���C���[�̃T�C�Y(��)
#define PLAYER_SIZE_HEIGHT	(80)		// �v���C���[�̃T�C�Y(����)

//-------------------------------------------------------------------
// �v���C���[�̏��
//-------------------------------------------------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,	// �ʏ���	
	PLAYERSTATE_FLASHING,	// �_��
	PLAYERSTATE_SHRINK,		// �k��ł�����
	PLAYERSTATE_MAX,
}PLAYERSTATE;

//-------------------------------------------------------------------
// �v���C���[�̍\����
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 posOld;	// �O�̈ʒu
	D3DXVECTOR3 move;	// �ړ���
	D3DXCOLOR col;		// �F
	float fWidth;		// ��
	float fHeight;		// ����
	int nCntAnim;		// �A�j���[�V�����J�E���g
	int nPatternAnim;	// �A�j���[�V�����p�^�[��
	float fV;			// V�l�̈ړ�
	bool bMove;			// �ړ������ǂ���
	bool bJump;			// �W�����v�������ǂ���
	PLAYERSTATE state;	// ���
	int nCounterState;	// ��ԃJ�E���^
	int nCounterDisp;	// �\���J�E���^
	int nCounterSize;	// �T�C�Y��߂��J�E���g
} PLAYER;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitPlayer(void);						// �v���C���[�̏���������
void UninitPlayer(void);						// �v���C���[�̏I������
void UpdatePlayer(void);						// �v���C���[�̍X�V����
void DrawPlayer(void);							// �v���C���[�̕`�揈��

void SingleModePlayer(void);					// �V���O�����[�h�̃v���C���[����
void MultiModePlayer(void);						// �}���`���[�h�̃v���C���[����

void SingleMovePlayer(void);					// �V���O�����[�h�̃v���C���[�ړ�����
void MultiMovePlayer(int nCntPlayer);			// �}���`���[�h�̃v���C���[�ړ�����

void AnimationPlayer(int nCntPlayer);			// �e�N�X�`���A�j���[�V�����̐ݒ�
void HitBlock(int nCntPlayer);					// �u���b�N�̓����蔻��
void HitPlayer(int nCntPlayer,float fAngle);	// �v���C���[�̓����蔻��
void CollisionPlayer(int nCntPlayer);			// �v���C���[���m�̓����蔻��

void SetVertexPlayer(int nIdx);					// �v���C���[�̒��_���W�̐ݒ�

void PlayerNormalProcessing(int nCntPlayer);	// �v���C���[��Ԃ��ʏ�̂Ƃ�
void PlayerStateFlashing(int nCntPlayer);		// �v���C���[��Ԃ��_�ł̂Ƃ�
void PlayerStateShrink(int nCntPlayer);			// �v���C���[��Ԃ��k��ł���Ƃ�
void ResultPlayer(int nCntPlayer);				// ���U���g��ʂ̏���

PLAYER *GetPlayer(void);						// �v���C���[���̎擾
int GetPlayerCnt(void);							// �v���C���[���̊l��

#endif