//-----------------------------------------------------------------
//
// �^�C�g�� (title.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "title.h"
#include "bg.h"
#include "titlelogo.h"
#include "selectmode.h"
#include "sound.h"
#include "setparticle.h"

//-----------------------------------------------------------------
// �^�C�g���̏���������
//-----------------------------------------------------------------
HRESULT InitTitle(void)
{
	// �w�i�̏���������
	InitBg();

	// �^�C�g�����S�̏���������
	InitTitlelogo();

	// �Z���N�g���[�h�̏���������
	InitSelectmode();

	// �T�E���h�̊J�n
	PlaySound(SOUND_LABEL_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------
// �^�C�g���̏I������
//-----------------------------------------------------------------
void UninitTitle(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// �w�i�̏I������
	UninitBg();

	// �^�C�g�����S�̏I������
	UninitTitlelogo();

	// �Z���N�g���[�h�̏I������
	UninitSelectmode();
}

//-----------------------------------------------------------------
// �^�C�g���̍X�V����
//-----------------------------------------------------------------
void UpdateTitle(void)
{
	// �ϐ��錾
	bool bFall = GetFall();

	// �w�i�̍X�V����
	UpdateBg();

	// �^�C�g�����S�̍X�V����
	UpdateTitlelogo();

	// �^�C�g�����S�����S�ɗ������Ƃ�����
	if (bFall == true)
	{// �Z���N�g���[�h�̍X�V����
		UpdateSelectmode();
	}
}

//-----------------------------------------------------------------
// �^�C�g���̕`�揈��
//-----------------------------------------------------------------
void DrawTitle(void)
{
	// �ϐ��錾
	bool bFall = GetFall();

	// �w�i�̕`�揈��
	DrawBg();

	// �^�C�g�����S�̕`�揈��
	DrawTitlelogo();

	// �^�C�g�����S�����S�ɗ������Ƃ�����
	if (bFall == true)
	{// �Z���N�g���[�h�̕`�揈��
		DrawSelectmode();
	}
}