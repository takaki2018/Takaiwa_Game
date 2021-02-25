//-------------------------------------------------------------------
//
// �v���C���[�w�b�_�[ (player.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "motion.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_FILENAME (64)		// X�t�@�C���l�[���̍ő啶����

//-------------------------------------------------------------------
// �v���C���[���(�񋓌^)
//-------------------------------------------------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		// �ʏ�
	PLAYERSTATE_FLOATING,		// ���V
	PLAYERSTATE_DAMAGE,			// �_���[�W
	PLAYERSTATE_MAX,			// ��Ԃ̍ő吔
}PLAYERSTATE;

typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// �j���[�g�������[�V����
	MOTIONTYPE_MOVE,		// �ړ����[�V����
	MOTIONTYPE_JUMP,		// �W�����v���[�V����
	MOTIONTYPE_LAND,		// ���n���[�V����
	MOTIONTYPE_DAMAGE,		// �̂����胂�[�V����
	MOTYONTYPE_MAX,			// ���[�V�����̍ő�l
}MOTIONTYPE;				// ���[�V�����̎��

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMesh;				// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;			// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD nNumMat = 0;				// �}�e���A���̐�
	LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`���ւ̃|�C���^
	char aFileName[MAX_FILENAME];	// �t�@�C���̖��O
	D3DXVECTOR3 pos;				// ���S���W
	D3DXVECTOR3 rot;				// ��]�p
	D3DXMATRIX mtxWorld;			// ���[���h�}�g���b�N�X
	int nIdxModelParent;			// �e���f���̃C���f�b�N�X
	D3DXVECTOR3 minVecModel;		// ���_�̍ŏ��l
	D3DXVECTOR3 maxVecModel;		// ���_�̍ő�l
}PlayerModel;

typedef struct
{
	D3DXVECTOR3 pos;							// ���S���W
	D3DXVECTOR3 posOld;							// �O�̒��S���W
	D3DXVECTOR3 rot;							// ��]�p
	D3DXVECTOR3 rotDest;						// �ړI�̉�]�p
	D3DXMATRIX mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 move;							// �ړ���
	bool bJump;									// �W�����v�������ǂ���
	PLAYERSTATE state;							// �v���C���[�̏��
	int nStateCounter;							// ��ԃJ�E���^
	PlayerModel aModel[10];						// ���f��
	int nNumModel;								// ���f����
	D3DXVECTOR3 minVecPlayer;					// ���_�̍ŏ��l
	D3DXVECTOR3 maxVecPlayer;					// ���_�̍ő�l
	int nIdx;									// ���Ԗڂ̉e��
	D3DXVECTOR3 posParent;						// �e�̍��W
	MOTION_INFO aMotionInfo[MOTYONTYPE_MAX];	// ���[�V�������
	int nNumMotion;								// ���[�V������
	MOTIONTYPE MotionType;						// ���[�V�����^�C�v
	MOTIONTYPE MotionTypeOld;					// �O��̃��[�V�����^�C�v
	bool bLoopMotion;							// ���[�v���邩�ǂ���
	int nNumKey;								// �L�[��
	int nKey;									// �L�[No.
	int nCounterMotion;							// ���[�V�����J�E���^
	bool bFall;									// ���������ǂ���
}Player;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitPlayer(void);			// �v���C���[�̏���������
void UninitPlayer(void);			// �v���C���[�̏I������
void UpdatePlayer(void);			// �v���C���[�̍X�V����
void DrawPlayer(void);				// �v���C���[�̕`�揈��

Player *GetPlayer(void);			// �v���C���[���̎擾

void StateManagerPlayer(void);		// �v���C���[��Ԃ̔���
void PlayerStateNormal(void);		// �v���C���[��Ԃ��ʏ�̎�
void PlayerStateFloating(void);		// �v���C���[��Ԃ����V�̎�
void PlayerStateDamage(void);		// �v���C���[���_���[�W��Ԃ̂Ƃ�

void MovePlayer(void);				// �v���C���[�̈ړ�����
void MovePlayerKeyboard(void);		// �v���C���[�̈ړ�����(�L�[�{�[�h)
void MovePlayerGamepad(void);		// �v���C���[�̈ړ�����(�Q�[���p�b�h)

void VecPlayer(void);				// �v���C���[�̒��_���W�̕␳

void PlayerMotion(void);			// �v���C�����[�V�����̐ݒ�

#endif