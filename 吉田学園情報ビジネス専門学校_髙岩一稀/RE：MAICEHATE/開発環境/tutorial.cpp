//==============================================================================================================
//
// �`���[�g���A����� (tutorial.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial.h"
#include "tutorial_bg.h"
#include "tutorial_image.h"
#include "tutorial_marker.h"
#include "tutorial_press_ok.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A����ʂ̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- �e�������������ł������ǂ���
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorial(void)
{
	// �`���[�g���A���w�i�̏���������
	InitTutorialBg();

	// �`���[�g���A���摜�̏���������
	InitTutorialImage();

	// �`���[�g���A���}�[�J�[�̏���������
	InitTutorialMarker();

	// �`���[�g���A���pPressOK�̏���������
	InitTutorialPressOK();

	// �T�E���h
	PlaySound(SOUND_LABEL_OE);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A����ʂ̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitTutorial(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// �`���[�g���A���pPressOK�̏I������
	UninitTutorialPressOK();

	// �`���[�g���A���}�[�J�[�̏I������
	UninitTutorialMarker();

	// �`���[�g���A���摜�̏I������
	UninitTutorialImage();

	// �`���[�g���A���w�i�̏I������
	UninitTutorialBg();
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A����ʂ̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorial(void)
{
	// �Q�[����ʂɑJ�ڂ��邩�ǂ���
	TutorialPressOK tutorialPressOK = GetPressOKInfo();

	// �`���[�g���A���w�i�̍X�V����
	UpdateTutorialBg();

	// �`���[�g���A���pPressOK�̍X�V����
	UpdateTutorialPressOK();

	// �Q�[����ʂɑJ�ڂ��邩�ǂ���
	if (tutorialPressOK.bGameTrasition == true)
	{
		SetFade(FADE_OUT,MODE_GAME);
	}
	else if (tutorialPressOK.bPressOK == false)
	{
		// �`���[�g���A���摜�̍X�V����
		UpdateTutorialImage();

		// �`���[�g���A���}�[�J�[�̍X�V����
		UpdateTutorialMarker();
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A����ʂ̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawTutorial(void)
{
	// �`���[�g���A���w�i�̕`�揈��
	DrawTutorialBg();

	// �`���[�g���A���摜�̕`�揈��
	DrawTutorialImage();

	// �`���[�g���A���}�[�J�[�̕`�揈��
	DrawTutorialMarker();

	// �`���[�g���A���pPressOK�̕`�揈��
	DrawTutorialPressOK();
}