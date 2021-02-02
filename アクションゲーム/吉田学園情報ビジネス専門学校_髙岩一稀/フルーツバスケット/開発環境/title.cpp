//-----------------------------------------------------------------
//
// �^�C�g�� (title.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "title.h"
#include "title_bg.h"
#include "titlelogo.h"
#include "selectmode.h"
#include "bear.h"
#include "wood.h"
#include "fruits.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
bool g_bSkipTitle;				// �^�C�g�����΂��Ƃ��Ɏg�p

//-----------------------------------------------------------------
// �^�C�g���̏���������
//-----------------------------------------------------------------
HRESULT InitTitle(void)
{
	// �ϐ��̏�����
	g_bSkipTitle = false;

	// �^�C�g���w�i�̕`�揈��
	InitTitleBg();

	// �^�C�g�����S�̏���������
	InitTitlelogo();

	// �Z���N�g���[�h�̏���������
	InitSelectmode();

	// �N�}�̏���������
	InitBear();

	// �t���[�c�̏���������
	InitFruits();

	// �؂̏���������
	InitWood();

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

	// �؂̏I������
	UninitWood();

	// �t���[�c�̏I������
	UninitFruits();

	// �N�}�̏I������
	UninitBear();

	// �Z���N�g���[�h�̏I������
	UninitSelectmode();

	// �^�C�g�����S�̏I������
	UninitTitlelogo();

	// �^�C�g���w�i�̏I������
	UninitTitleBg();
}

//-----------------------------------------------------------------
// �^�C�g���̍X�V����
//-----------------------------------------------------------------
void UpdateTitle(void)
{
	// �ϐ��錾
	bool bFall = GetFall();
	bool bTitleFruits = GetbTitleFruits();
	int nTitleCnt = GetTitleFruitsCnt();

	if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJAMP) == true || 
		GetKeyboardTrigger(KEYINFO_OK) == true)
	{// �G���^�[�L�[�܂���A�{�^������������X�L�b�v
		g_bSkipTitle = true;
	}
	// �^�C�g���w�i�̍X�V����
	UpdateTitleBg();

	if ((bTitleFruits == true && nTitleCnt < 0) || g_bSkipTitle == true)
	{
		// �^�C�g�����S�̍X�V����
		UpdateTitlelogo();

		// �^�C�g�����S�����S�ɗ������Ƃ�����
		if (bFall == true)
		{// �Z���N�g���[�h�̍X�V����
			UpdateSelectmode();
		}
	}
	// �N�}�̍X�V����
	UpdateBear();

	// �t���[�c�̍X�V����
	UpdateFruits();

	// �؂̍X�V����
	UpdateWood();
}

//-----------------------------------------------------------------
// �^�C�g���̕`�揈��
//-----------------------------------------------------------------
void DrawTitle(void)
{
	// �ϐ��錾
	bool bFall = GetFall();
	bool bTitleFruits = GetbTitleFruits();
	int nTitleCnt = GetTitleFruitsCnt();

	// �^�C�g���w�i�̕`�揈��
	DrawTitleBg();

	if (g_bSkipTitle != true)
	{
		// �N�}�̕`�揈��
		DrawBear();
	}
	// �؂̕`�揈��
	DrawWood();

	if ((bTitleFruits == true && nTitleCnt < 0) || g_bSkipTitle == true)
	{
		// �^�C�g�����S�̕`�揈��
		DrawTitlelogo();

		// �^�C�g�����S�����S�ɗ������Ƃ�����
		if (bFall == true)
		{// �Z���N�g���[�h�̕`�揈��
			DrawSelectmode();
		}
	}

	if (g_bSkipTitle != true)
	{
		// �t���[�c�̕`�揈��
		DrawFruits();
	}
}

//-----------------------------------------------------------------
// �^�C�g���̉��o���΂������ǂ���
//-----------------------------------------------------------------
bool GetSkipTitle(void)
{
	return g_bSkipTitle;
}