//-----------------------------------------------------------------
//
// ���U���g (result.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result.h"
#include "result_bg.h"
#include "selectmode.h"
#include "player.h"
#include "gauge.h"
#include "result_logo.h"
#include "result_ann.h"
#include "ranking.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
int g_nSkipCnt;					// ���U���g����^�C�g���ɑJ�ڂł���܂ł̃J�E���g
PLAYMODE g_Resultplaymode;		//�@�v���C���[�h

//-----------------------------------------------------------------
// ���U���g�̏���������
//-----------------------------------------------------------------
HRESULT InitResult(void)
{
	// �ϐ��̏�����
	g_nSkipCnt = 0;
	g_Resultplaymode = GetPlayMode();

	// ���U���g�w�i�̏���������
	InitResultBg();

	// �v���C���[�̏���������
	InitPlayer();

	// �Q�[�W�̏���������
	InitGauge();

	// ���U���g���S�̏���������
	InitResultLogo();

	if (g_Resultplaymode == PLAYMODE_SINGLE)
	{// �����L���O�̏���������
		InitRanking();
	}

	// ���ʔ��\�̏���������
	InitResultAnn();

	return S_OK;
}

//-----------------------------------------------------------------
// ���U���g�̏I������
//-----------------------------------------------------------------
void UninitResult(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// ���ʔ��\�̏I������
	UninitResultAnn();

	if (g_Resultplaymode == PLAYMODE_SINGLE)
	{// �����L���O�̏I������
		UninitRanking();
	}

	// ���U���g���S�̏I���
	UninitResultLogo();

	// �Q�[�W�̏I������
	UninitGauge();

	// �v���C���[�̏I������
	UninitPlayer();

	// ���U���g�w�i�̏I������
	UninitResultBg();
}

//-----------------------------------------------------------------
// ���U���g�̍X�V����
//-----------------------------------------------------------------
void UpdateResult(void)
{
	// �ϐ��錾
	bool bRAUse = GetResultAnn();
	bool bSound = GetbGauge();

	// ���U���g�w�i�̍X�V����
	UpdateResultBg();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// ���U���g���S�̍X�V����
	UpdateResultLogo();

	// ���ʔ��\�̍X�V����
	UpdateResultAnn();

	if (bRAUse == false)
	{
		// �Q�[�W�̍X�V����
		UpdateGauge();
	}

	if (bSound == true)
	{
		// �J�E���g�̍X�V
		g_nSkipCnt++;

		if (g_Resultplaymode == PLAYMODE_SINGLE)
		{// �����L���O�̍X�V����
			UpdateRanking();
		}
	}
	if (g_nSkipCnt > 60)
	{
		if (GetJoypadTrigger(0, JPINFO_OKorJAMP) == true ||
			GetKeyboardTrigger(KEYINFO_OK) == true)
		{// ��ʃ��[�h�̏���������
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
}

//-----------------------------------------------------------------
// ���U���g�̕`�揈��
//-----------------------------------------------------------------
void DrawResult(void)
{
	// ���U���g�w�i�̕`�揈��
	DrawResultBg();

	// �Q�[�W�̕`�揈��
	DrawGauge();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// ���U���g���S�̕`�揈��
	DrawResultLogo();

	if (g_Resultplaymode == PLAYMODE_SINGLE)
	{// �����L���O�̕`�揈��
		DrawRanking();
	}

	// ���ʔ��\�̕`�揈��
	DrawResultAnn();
}