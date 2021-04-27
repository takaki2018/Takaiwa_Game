//==============================================================================================================
//
// �Q�[�����C�� (game.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "game.h"
#include "motion.h"
#include "player.h"
#include "ui.h"
#include "enemy.h"
#include "shadow.h"
#include "coin.h"
#include "model_set.h"
#include "meshfield.h"
#include "3Deffect.h"
#include "setparticle.h"
#include "pause.h"
#include "timer.h"
#include "coin_sheets.h"
#include "score.h"
#include "fade.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"
#include "camera.h"

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
bool g_bPause = false;
int g_nCntResult;						// ���U���g�܂ł̃J�E���g

//--------------------------------------------------------------------------------------------------------------
// �x�[�X�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitGame(void)
{
	// �ϐ��̏�����
	g_nCntResult = 0;

	// �Q�[���J�����̏�����
	InitGameCamera();

	// �e�̏���������
	InitShadow();

	// �v���C���[�̏���������
	InitPlayer();

	// �G�̏���������
	InitEnemy();

	// �I�u�W�F�N�g�̏���������
	InitCoin();

	// ���f���z�u�̏���������
	InitModelSet();

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// �G�t�F�N�g�̏���������
	Init3DEffect();

	// UI�̏���������
	InitUi();

	// �R�C���l���󋵂̏�����
	InitCoinSheets();

	// �|�[�Y��ʂ̏���������
	InitPause();

	// �T�E���h�̒ǉ�
	PlaySound(SOUND_LABEL_BATTLE);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �x�[�X�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitGame(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// �|�[�Y��ʂ̏I������
	UninitPause();

	// �R�C���l���󋵂̏I������
	UninitCoinSheets();

	// UI�̏I������
	UninitUi();

	// �G�t�F�N�g�̏I������
	Uninit3DEffect();

	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// ���f���z�u�̏I������
	UninitModelSet();

	// �I�u�W�F�N�g�̏I������
	UninitCoin();

	// �e�̏I������
	UninitShadow();

	// �G�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();
}

//--------------------------------------------------------------------------------------------------------------
// �x�[�X�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateGame(void)
{
	// �v���C���[�����擾
	Player *pPlayer = GetPlayer();
	FADE fade = GetFade();				// �t�F�[�h���
	bool bNocoin = GetbNoCoin();
	int nTimer = GetTimer();

	if ((GetKeyboardTrigger(KEYINFO_PAUSE) == true || GetJoypadTrigger(PLAYER_1,JPINFO_PAUSE)) && 
		pPlayer->bFall == false && fade == FADE_NONE && bNocoin == false && nTimer != 0)
	{
		g_bPause = g_bPause ? false : true;

		if (g_bPause == true)
		{// �|�[�Y��ʂ��J�����Ƃ�
		 // SE
			PlaySound(SOUND_LABEL_SE_PAUSE_IN);
		}
		else if (g_bPause == false)
		{// �|�[�Y��ʂ�����Ƃ�
		 // SE
			PlaySound(SOUND_LABEL_SE_PAUSE_OUT);
		}
	}
	if (g_bPause == false && pPlayer->bFall == false && bNocoin == false && nTimer != 0)
	{
		// �G�t�F�N�g�̍X�V����
		Update3DEffect();

		// �I�u�W�F�N�g�̍X�V����
		UpdateCoin();

		// ���f���z�u�̍X�V����
		UpdateModelSet();

		// ���b�V���t�B�[���h�̍X�V����
		UpdateMeshField();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �G�̍X�V����
		UpdateEnemy();

		// �e�̍X�V����
		UpdateShadow();
	}
	else if (g_bPause == true && pPlayer->bFall == false && bNocoin == false && nTimer != 0)
	{
		// �|�[�Y��ʂ̍X�V����
		UpdatePause();
	}
	if (g_bPause == false  && pPlayer->bFall == false && bNocoin == false)
	{
		// UI�̍X�V����
		UpdateUi();
	}
	if (g_bPause == false && pPlayer->bFall == false && nTimer != 0)
	{
		// �R�C���l���󋵂̍X�V����
		UpdateCoinSheets();
	}

	if (pPlayer->bFall == true || bNocoin == true || nTimer == 0)
	{// ���U���g��ʂ܂ł̃J�E���g
		g_nCntResult++;

		if (g_nCntResult == 60)
		{
			if (nTimer != 0 && pPlayer->bFall == false)
			{
				// �X�R�A�̉��Z
				AddScore(100 * nTimer);
			}

			// ��ʃ��[�h�̏���������
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �x�[�X�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawGame(void)
{
	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �G�̕`�揈��
	DrawEnemy();

	// �I�u�W�F�N�g�̕`�揈��
	DrawCoin();

	// ���f���z�u�̕`�揈��
	DrawModelSet();

	// ���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// �e�̕`�揈��
	DrawShadow();

	// UI�̕`�揈��
	DrawUi();

	// �R�C���l���󋵂̕`�揈��
	DrawCoinSheets();

	if (g_bPause == true)
	{
		// �|�[�Y��ʂ̕`�揈��
		//DrawPause();
	}
	// �G�t�F�N�g�̕`�揈��
	Draw3DEffect();
}

//----------------------------------------------------------------------------------------------------------------
// �|�[�Y�̈ڂ�ւ��
// ����		�FbPause	- �|�[�Y���Ă邩�ǂ���
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//----------------------------------------------------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}