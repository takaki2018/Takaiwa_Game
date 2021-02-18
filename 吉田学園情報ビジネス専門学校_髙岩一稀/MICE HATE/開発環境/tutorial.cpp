//-----------------------------------------------------------------
//
// �`���[�g���A�� (tutorial.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "tutorial.h"
#include "bg.h"
#include "oe.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "effect.h"
#include "shockwave.h"
#include "object.h"
#include "ui.h"
#include "timer.h"
#include "score.h"
#include "pause.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"

//-----------------------------------------------------------------
// �`���[�g���A���̏���������
//-----------------------------------------------------------------
HRESULT InitTutorial(void)
{
	// �w�i�̏���������
	InitBg();

	// ��������̏���������
	InitOe();

	// �v���C���[�̏���������
	InitPlayer();

	// �e�̏���������
	InitBullet();

	// �G�̏���������
	InitEnemy();

	// �I�u�W�F�N�g�̏���������
	InitObject();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �Ռ��g�̏���������
	InitShockwave();

	// UI�̏���������
	InitUi();

	// �X�R�A�̏�����
	InitScore();

	// �^�C�}�[�̏�����
	InitTimer();

	return S_OK;
}

//-----------------------------------------------------------------
// �`���[�g���A���̏I������
//-----------------------------------------------------------------
void UninitTutorial(void)
{
	// �^�C�}�[�̏I������
	UninitTimer();

	// �X�R�A�̏I������
	UninitScore();

	// UI�̏I������
	UninitUi();

	// �Ռ��g�̏I������
	UninitShockwave();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �I�u�W�F�N�g�̏I������
	UninitObject();

	// �G�̏I������
	UninitEnemy();

	// �e�̏I������
	UninitBullet();

	// �|���S���̏I������
	UninitPlayer();

	// ��������̏I������
	UninitOe();

	// �w�i�̏I������
	UninitBg();
}

//-----------------------------------------------------------------
// �`���[�g���A���̍X�V����
//-----------------------------------------------------------------
void UpdateTutorial(void)
{
	// �w�i�̍X�V����
	UpdateBg();

	// ��������̍X�V��������
	UpdateOe();

	// �|���S���̍X�V����
	UpdatePlayer();

	// �e�̍X�V����
	UpdateBullet();

	// �I�u�W�F�N�g�̍X�V����
	UpdateObject();

	// �G�t�F�N�g�̍X�V����
	UpdateEffect();

	// �Ռ��g�̍X�V����
	UpdateShockwave();

	// �X�R�A�̍X�V����
	UpdateScore();

	// UI�̍X�V����
	UpdateUi();

	// �G�̍X�V����
	//UpdateEnemy();

	// �^�C�g����ʂɑJ��
	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(JPINFO_OK) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//-----------------------------------------------------------------
// �`���[�g���A���̕`�揈��
//-----------------------------------------------------------------
void DrawTutorial(void)
{
	// �w�i�̕`�揈��
	DrawBg();

	// ��������̕`�揈��
	DrawOe();

	// �e�̕`��
	DrawBullet();

	// �I�u�W�F�N�g�̕`�揈��
	DrawObject();

	// �G�t�F�N�g�̕`��
	DrawEffect();

	// �|���S���̕`�揈��
	DrawPlayer();

	// �G�̕`��
	DrawEnemy();

	// �Ռ��g�̕`�揈��
	DrawShockwave();

	// UI�̕`�揈��
	DrawUi();

	// �X�R�A�̕`�揈��
	DrawScore();

	// �^�C�}�[�̕`�揈��
	DrawTimer();
}