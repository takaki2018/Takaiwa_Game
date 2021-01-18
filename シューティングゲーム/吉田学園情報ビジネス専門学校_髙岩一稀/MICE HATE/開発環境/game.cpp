//-------------------------------------------------------------------
//
// �Q�[����ʏ��� (game.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "game.h"
#include "selectmode.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "gamepad.h"
#include "bullet.h"
#include "object.h"
#include "enemy.h"
#include "effect.h"
#include "shockwave.h"
#include "item.h"
#include "ui.h"
#include "score.h"
#include "timer.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"
#include "pause_oe.h"

//-------------------------------------------------------------------
// �O���[�o���ϐ�
//-------------------------------------------------------------------
bool g_bPause = false;	// �|�[�Y
int g_nCounterItem;		// �A�C�e���̌��ʎ���
int g_nCntResult;		// ���U���g��ʂ܂ł̃J�E���g
int g_Timer;			// �^�C�}�[
bool g_bHit;		// �I�u�W�F�N�g�ƓG�������������ǂ���

//-------------------------------------------------------------------
// �Q�[����ʂ̏���������
//-------------------------------------------------------------------
HRESULT InitGame(void)
{
	// �ϐ��̏�����
	g_nCounterItem = 0;
	g_nCntResult = 0;

	// �w�i�̏���������
	InitBg();

	// �|���S���̏���������
	InitPlayer();

	// �I�u�W�F�N�g�̏���������
	InitObject();

	// �e�̏���������
	InitBullet();

	// �G�̏���������
	InitEnemy();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �Ռ��g�̏���������
	InitShockwave();

	// UI�̏���������
	InitUi();

	// �^�C�}�[�̏�����
	InitTimer();

	// �|�[�Y�̏���������
	InitPause();

	// �|�[�YOE�̏���������
	InitPauseOe();

	// BGM
	PlaySound(SOUND_LABEL_BATTLE);

	return S_OK;
}

//-------------------------------------------------------------------
// �Q�[����ʂ̏I������
//-------------------------------------------------------------------
void UninitGame(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// �|�[�YOE�̏I������
	UninitPauseOe();

	// �|�[�Y�̏I������
	UninitPause();

	// �^�C�}�[�̏I������
	UninitTimer();

	// UI�̏I������
	UninitUi();

	// �Ռ��g�̏I������
	UninitShockwave();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �G�̏I������
	UninitEnemy();

	// �e�̏I������
	UninitBullet();

	// �I�u�W�F�N�g�̏I������
	UninitObject();

	// �|���S���̏I������
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
	g_Timer = GetTimer();				// �^�C�}�[
	g_bHit = GetHitObject();			// �G�ƃI�u�W�F�N�g�̓����蔻��
	int SelectMode = GetSelectMode();	// �Z���N�g���[�h
	FADE fade = GetFade();				// �t�F�[�h���

	// �|�[�Y�̏���
	if ((GetKeyboardTrigger(KEYINFO_PAUSE) == true || GetJoypadTrigger(JPINFO_PAUSE) == true) && fade == FADE_NONE)
	{
		g_bPause = g_bPause ? false : true;

		if (g_bPause == true)
		{// �T�E���h
			PlaySound(SOUND_LABEL_SE_PAUSE_IN);
		}
		else if (g_bPause == false)
		{// �T�E���h
			PlaySound(SOUND_LABEL_SE_PAUSE_OUT);
		}
	}
	// �I���������[�h�ɉ����ď���
	switch (SelectMode)
	{// �m�[�}�����[�h�̏���
	case SELECTMODE_NORMAL:
		NormalMode();
		break;

		// �G���h���X���[�h�̏���
	case SELECTMODE_ENDLESS:
		EndlessMode();
		break;
	}

	if (g_bHit == false && g_bPause == false)
	{// �^�C�}�[�̍X�V����
		UpdateTimer();
	}
}

//-------------------------------------------------------------------
// �Q�[����ʂ̕`�揈��
//-------------------------------------------------------------------
void DrawGame(void)
{
	// �ϐ��錾
	FADE fade = GetFade();

	// �w�i�̕`�揈��
	DrawBg();

	// �e�̕`��
	DrawBullet();

	// �G�t�F�N�g�̕`��
	DrawEffect();

	// �|���S���̕`�揈��
	DrawPlayer();

	// �I�u�W�F�N�g�̕`�揈��
	DrawObject();

	// �G�̕`��
	DrawEnemy();

	// �Ռ��g�̕`�揈��
	DrawShockwave();

	// UI�̕`�揈��
	DrawUi();

	// �^�C�}�[�̕`�揈��
	DrawTimer();

	if (g_bPause == true)
	{// �|�[�Y�̕`�揈��
		DrawPause();

		// �|�[�YOE�̏���������
		DrawPauseOe();
	}
}

//-------------------------------------------------------------------
// �m�[�}�����[�h�̏���
//-------------------------------------------------------------------
void NormalMode(void)
{
	if (g_bPause == false && g_Timer != 0)
	{// �G�̍X�V����
		UpdateEnemy();

		if (g_bHit == false)
		{// �w�i�̍X�V����
			UpdateBg();

			// �|���S���̍X�V����
			UpdatePlayer();

			// �I�u�W�F�N�g�̍X�V����
			UpdateObject();

			// �e�̍X�V����
			UpdateBullet();

			// �G�t�F�N�g�̍X�V����
			UpdateEffect();

			// �Ռ��g�̍X�V����
			UpdateShockwave();

			// UI�̍X�V����
			UpdateUi();
		}
	}
	else if (g_bPause == true)
	{// �|�[�Y�̍X�V����
		UpdatePause();

		// �|�[�YOE�̍X�V����
		UpdatePauseOe();
	}
	// �G���I�u�W�F�N�g�ɓ��������烊�U���g��ʂɑJ��
	if ((g_Timer == 0) | (g_bHit == true))
	{// ���U���g��ʂ܂ł̃J�E���g
		g_nCntResult++;

		if (g_nCntResult == 60)
		{// ��ʃ��[�h�̏���������
			int nCntIn = GetEnemyIn();
			AddScore(nCntIn * 100);
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
	
}

//-------------------------------------------------------------------
// �G���h���X���[�h�̏���
//-------------------------------------------------------------------
void EndlessMode(void)
{
	if (g_bPause == false)
	{// �G�̍X�V����
		UpdateEnemy();

		if (g_bHit == false)
		{// �w�i�̍X�V����
			UpdateBg();

			// �|���S���̍X�V����
			UpdatePlayer();

			// �I�u�W�F�N�g�̍X�V����
			UpdateObject();

			// �e�̍X�V����
			UpdateBullet();

			// �G�t�F�N�g�̍X�V����
			UpdateEffect();

			// �Ռ��g�̍X�V����
			UpdateShockwave();

			// UI�̍X�V����
			UpdateUi();
		}
	}
	else if (g_bPause == true)
	{// �|�[�Y�̍X�V����
		UpdatePause();

		// �|�[�YOE�̍X�V����
		UpdatePauseOe();
	}
	// �G���I�u�W�F�N�g�ɓ��������烊�U���g��ʂɑJ��
	if (g_bHit == true)
	{// ���U���g��ʂ܂ł̃J�E���g
		g_nCntResult++;

		if (g_nCntResult == 60)
		{// ��ʃ��[�h�̏���������
			int nCntIn = GetEnemyIn();
			int nTimer = GetTimer();
			AddScore(nCntIn * 100);
			AddScore(nTimer * 100);
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//-------------------------------------------------------------------
// �|�[�Y�̈ڂ�ւ��
//-------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}