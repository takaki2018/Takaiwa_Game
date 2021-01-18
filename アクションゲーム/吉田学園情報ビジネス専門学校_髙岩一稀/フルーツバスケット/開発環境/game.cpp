//-------------------------------------------------------------------
//
// �Q�[����ʏ��� (game.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "game.h"
#include "player.h"
#include "bg.h"
#include "block.h"
#include "fruits.h"
#include "enemy.h"
#include "crow.h"
#include "rat.h"
#include "wood.h"
#include "wood_back.h"
#include "timer.h"
#include "bear.h"
#include "ui.h"
#include "pause.h"
#include "count_down.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "effect.h"
#include "sound.h"

//-------------------------------------------------------------------
// �O���[�o���ϐ�
//-------------------------------------------------------------------
bool g_bPause = false;	// �|�[�Y
int g_nCounterItem;		// �A�C�e���̌��ʎ���
int g_nCntResult;		// ���U���g��ʂ܂ł̃J�E���g

//-------------------------------------------------------------------
// �Q�[����ʂ̏���������
//-------------------------------------------------------------------
HRESULT InitGame(void)
{
	// �ϐ��̏�����
	g_nCntResult = 0;

	// �w�i�̏�����
	InitBg();

	// �v���C���[�̏�����
	InitPlayer();

	// �u���b�N�̏���������
	InitBlock();

	// �t���[�c�̏���������
	InitFruits();

	// �G�̏���������
	InitEnemy();

	// �J���X�̏���������
	InitCrow();

	// �l�Y�~�̏���������
	InitRat();

	// �؂̏���������
	InitWood();

	// �؂̗��̏���������
	InitWoodBack();

	// �^�C�}�[�̏���������
	InitTimer();

	// �N�}�̏���������
	InitBear();

	// �|�[�Y�̏���������
	InitPause();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// UI�̏���������
	InitUI();

	// �J�E���g�_�E���̏���������
	InitCountDown();

	// �u���b�N�̔z�u
	SetBlockInfo(MODESTATE_GAME_FOREST);

	return S_OK;
}

//-------------------------------------------------------------------
// �Q�[����ʂ̏I������
//-------------------------------------------------------------------
void UninitGame(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// �J�E���g�_�E���̏I������
	UninitCountDown();

	// UI�̏I������
	UninitUI();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �|�[�Y�̏I����
	UninitPause();

	// �N�}�̏I������
	UninitBear();

	// �^�C�}�[�̏I������
	UninitTimer();

	// �؂̗��̏I������
	UninitWoodBack();

	// �؂̏I������
	UninitWood();

	// �l�Y�~�̏I������
	UninitRat();

	// �J���X�̏I������
	UninitCrow();

	// �G�̏I������
	UninitEnemy();

	// �t���[�c�̏I������
	UninitFruits();

	// �u���b�N�̏I������
	UninitBlock();

	// �v���C���[�̏I������
	UninitPlayer();

	// �w�i�̏I������
	UninitBg();
}

//-------------------------------------------------------------------
// �Q�[����ʂ̍X�V����
//-------------------------------------------------------------------
void UpdateGame(void)
{
	// �ϐ��錾
	int nCountDown = GetCountDown();	// �J�E���g�_�E���̎擾
	int nTimer = GetTimer();			// �c�莞�Ԃ̎擾
	FADE fade = GetFade();				// �t�F�[�h���

	if (nTimer != 0)
	{
		// �|�[�Y�̏���
		if ((GetJoypadTrigger(PLAYER_1, JPINFO_PAUSE) == true || 
			GetKeyboardTrigger(KEYINFO_PAUSE) == true) &&
			fade == FADE_NONE && nCountDown == 0)
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

		if (g_bPause == false)
		{// �w�i�̍X�V����
			UpdateBg();

			// �u���b�N�̍X�V����
			UpdateBlock();

			if (nCountDown == 0)
			{// �t���[�c�̍X�V����
				UpdateFruits();

				// �G�̍X�V����
				UpdateEnemy();

				// �J���X�̍X�V����
				UpdateCrow();

				// �l�Y�~�̍X�V����
				UpdateRat();

				//// �v���C���[�̍X�V����
				UpdatePlayer();

				// �؂̍X�V����
				UpdateWood();

				// �؂̗��̍X�V����
				UpdateWoodBack();

				// �N�}�̍X�V����
				UpdateBear();

				// �G�t�F�N�g�̍X�V����
				UpdateEffect();

				// UI�̍X�V����
				UpdateUI();
			}
		}
		else if (g_bPause == true && fade == FADE_NONE)
		{// �|�[�Y�̍X�V����
			UpdatePause();
		}
	}
	if (g_bPause == false && nCountDown == 0)
	{// �^�C�}�[�̍X�V����
		UpdateTimer();
	}
	// �J�E���g�_�E���̍X�V����
	UpdateCountDown();

	// ��ʑJ��
	if (nTimer == 0)
	{
		g_nCntResult++;

		if (g_nCntResult == 60)
		{// ��ʃ��[�h�̏���������

			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//-------------------------------------------------------------------
// �Q�[����ʂ̕`�揈��
//-------------------------------------------------------------------
void DrawGame(void)
{
	// �w�i�̕`�揈��
	DrawBg();

	// �J���X�̕`�揈��
	DrawCrow();

	// �l�Y�~�̕`�揈��
	DrawRat();

	// �؂̗��̕`�揈��
	DrawWoodBack();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �t���[�c�̕`�揈��
	DrawFruits();

	// �G�̕`�揈��
	DrawEnemy();

	// �u���b�N�̕`�揈��
	DrawBlock();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �N�}�̕`�揈��
	DrawBear();

	// �؂̕`�揈��
	DrawWood();

	// �^�C�}�[�̕`�揈��
	DrawTimer();

	// UI�̕`�揈��
	DrawUI();

	// �J�E���g�_�E���̕`�揈��
	DrawCountDown();

	if (g_bPause == true)
	{// �|�[�Y�̕`�揈��
		DrawPause();
	}
}

//-------------------------------------------------------------------
// �|�[�Y�̈ڂ�ւ��
//-------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}