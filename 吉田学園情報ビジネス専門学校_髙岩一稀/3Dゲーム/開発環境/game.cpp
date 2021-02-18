//-----------------------------------------------------------------
//
// �Q�[�����C�� (game.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "game.h"
#include "polygon.h"
#include "model.h"
#include "motion.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "ui.h"
#include "enemy.h"
#include "shadow.h"
#include "wall.h"
#include "coin.h"
#include "model_set.h"
#include "billboard.h"
#include "meshfield.h"
#include "effect.h"
#include "setparticle.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "input.h"
#include "gamepad.h"
#include "cross_product.h"

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
bool g_bPause = false;

//-----------------------------------------------------------------
// �x�[�X�̏���������
//-----------------------------------------------------------------
HRESULT InitGame(void)
{
	// �|���S���̏���������
	InitPolygon();

	// �e�̏���������
	InitShadow();

	// ���f���̏���������
	//InitModel();

	// �v���C���[�̏���������
	InitPlayer();

	// �G�̏���������
	InitEnemy();

	// �ǂ̏���������
	InitWall();

	// �I�u�W�F�N�g�̏���������
	InitCoin();

	// ���f���z�u�̏���������
	InitModelSet();

	// �r���{�[�h�̏���������
	InitBillboard();

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// �e�̏���������
	InitBullet();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �����̏���������
	InitExplosion();

	// UI�̏���������
	InitUi();

	return S_OK;
}

//-----------------------------------------------------------------
// �x�[�X�̏I������
//-----------------------------------------------------------------
void UninitGame(void)
{
	// UI�̏I������
	UninitUi();

	// �����̏I������
	UninitExplosion();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �e�̏I������
	UninitBullet();

	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// �r���{�[�h�̏I������
	UninitBillboard();

	// ���f���z�u�̏I������
	UninitModelSet();

	// �I�u�W�F�N�g�̏I������
	UninitCoin();

	// �ǂ̏I������
	UninitWall();

	// �e�̏I������
	UninitShadow();

	// �G�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	// ���f���̏I������
	//UninitModel();

	// �|���S���̏I������
	UninitPolygon();
}

//-----------------------------------------------------------------
// �x�[�X�̍X�V����
//-----------------------------------------------------------------
void UpdateGame(void)
{
	// �v���C���[�����擾
	Player *pPlayer = GetPlayer();

	if (GetKeyboardTrigger(KEYINFO_PAUSE) == true || GetJoypadTrigger(PLAYER_1,JPINFO_PAUSE))
	{
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == false && pPlayer->bFall == false)
	{
		// �e�X�g
		//SetParticle();

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �|���S���̍X�V����
		UpdatePolygon();

		// ���f���̍X�V����
		//UpdateModel();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �G�̍X�V����
		UpdateEnemy();

		// �e�̍X�V����
		UpdateShadow();

		// �ǂ̍X�V����
		UpdateWall();

		// �I�u�W�F�N�g�̍X�V����
		UpdateCoin();

		// ���f���z�u�̍X�V����
		UpdateModelSet();

		// �r���{�[�h�̍X�V����
		UpdateBillboard();

		// ���b�V���t�B�[���h�̍X�V����
		UpdateMeshField();

		// �e�̍X�V����
		UpdateBullet();

		// �����̍X�V����
		UpdateExplosion();
		
		// UI�̍X�V����
		UpdateUi();

		// �O�ς�p���������蔻��̏�����
		CrossProductTest();
	}

	if (pPlayer->bFall == true)
	{// �v���C���[���������珈��
		SetFade(FADE_OUT,MODE_RESULT);
	}
}

//-----------------------------------------------------------------
// �x�[�X�̕`�揈��
//-----------------------------------------------------------------
void DrawGame(void)
{
	// �|���S���̕`�揈��
	//DrawPolygon();

	// ���f���̕`�揈��
	//DrawModel();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �G�̕`�揈��
	DrawEnemy();

	// �ǂ̕`�揈��
	//DrawWall();

	// �I�u�W�F�N�g�̕`�揈��
	DrawCoin();

	// ���f���z�u�̕`�揈��
	DrawModelSet();

	// �r���{�[�h�̕`�揈��
	//DrawBillboard();

	// ���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// �e�̕`�揈��
	DrawBullet();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �����̕`�揈��
	DrawExplosion();

	// �e�̕`�揈��
	DrawShadow();

	// UI�̕`�揈��
	DrawUi();
}

//-------------------------------------------------------------------
// �|�[�Y�̈ڂ�ւ��
//-------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}