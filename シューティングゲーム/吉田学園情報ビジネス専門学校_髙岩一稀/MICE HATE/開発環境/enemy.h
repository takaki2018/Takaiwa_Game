//-------------------------------------------------------------------
//
// �G�l�~�[�w�b�_�[ (enemy.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_ENEMY				(256)									// �G�̍ő吔
#define ENEMY_X					(15.0f)									// ���S���W��X���W
#define ENEMY_Y					(15.0f)									// ���S���W��Y���W

//-------------------------------------------------------------------
// �񋓌^
//-------------------------------------------------------------------
// �G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	// �ʏ���
	ENEMYSTATE_ANYHIT,		// �Ⴄ�e�������������
	ENEMYSTATE_ITEMEFFECT,	// �A�C�e���̌��ʂ��󂯂Ă�����
	ENEMTSTATE_HITOBJECT,	// �G���I�u�W�F�N�g�ɓ����������
	ENEMYSTATE_MAX,
}ENEMYSTATE;

// �G�̎��
typedef enum
{
	ENEMYTYPE_WHITE = 0,	// ��
	ENEMYTYPE_RED,			// ��
	ENEMYTYPE_BLUE,			// ��
	ENEMYTYPE_MAX,
}ENEMYTYPE;

//-------------------------------------------------------------------
// �G�̍\����
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 move;	// �ړ���
	ENEMYTYPE nType;	// ���
	D3DXCOLOR col;		// �F
	ENEMYSTATE state;	// ���
	float fAngle;		// �I�u�W�F�N�g�܂ł̊p�x
	int nCounterState;	// ��ԃJ�E���^
	int nCntBullet;		// �e�����J�E���g
	int nLife;			// �̗�
	int nCntFlash;		// �x���̉�
	int nFlashCounter;	// �t���b�V���J�E���^�[
	bool bUse;			// �g�p���Ă��邩�ǂ���
} ENEMY;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitEnemy(void);														// �G�̏���������
void UninitEnemy(void);															// �G�̏I������
void UpdateEnemy(void);															// �G�̍X�V����
void DrawEnemy(void);															// �G�̕`�揈��

void EnemyInfoWhite(int nCntEnemy);												// �G���(��)
void EnemyInfoRed(int nCntEnemy);												// �G���(��)
void EnemyInfoBlue(int nCntEnemy);												// �G���(��)
void EnemyState(int nCntEnemy);													// �G�̏��

bool HitEnemy(int nIdx, int nCntBullet);										// �G�̓����蔻�肪true�̂Ƃ�
bool GetHitObject(void);														// �I�u�W�F�N�g�̓����蔻��̎擾

void SetEnemyRandam(void);														// �G�������_���ɔz�u
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, float angle, ENEMYTYPE nType);	// �G�̐ݒ�
void SetVertexEnemy(int nIdx);													// �G�̒��_���W
ENEMY *GetEnemy(void);															// �G���̎擾
int GetEnemyIn(void);															// �G���͈͓��ɗ������̎擾

#endif