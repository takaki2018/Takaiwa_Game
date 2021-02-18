//-----------------------------------------------------------------
//
// ���U���g�w�i (result_bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_bg.h"
#include "bg.h"
#include "block.h"
#include "crow.h"
#include "wood.h"
#include "wood_back.h"
#include "result_score.h"
#include "result_score_bg.h"

//-----------------------------------------------------------------
// ���U���g�w�i�̏���������
//-----------------------------------------------------------------
HRESULT InitResultBg(void)
{
	// �w�i�̏���������
	InitBg();

	// �J���X�̏���������
	InitCrow();

	// �؂̗��̏���������
	InitWoodBack();

	// �؂̏���������
	InitWood();

	// �u���b�N�̏���������
	InitBlock();

	// ���U���g�X�R�A�w�i�̏���������
	InitResultScoreBg();

	// ���U���g�X�R�A�̏���������
	InitResultScore();

	// �u���b�N�̔z�u
	SetBlockInfo(MODESTATE_RESULT);

	return S_OK;
}

//-----------------------------------------------------------------
// ���U���g�w�i�̏I������
//-----------------------------------------------------------------
void UninitResultBg(void)
{
	// ���U���g�X�R�A�̏I������
	UninitResultScore();

	// ���U���g�X�R�A�w�i�̏I������
	UninitResultScoreBg();

	// �u���b�N�̏I������
	UninitBlock();

	// �؂̏I������
	UninitWood();

	// �؂̗��̏I������
	UninitWoodBack();

	// �J���X�̏I������
	UninitCrow();

	// �w�i�̏I������
	UninitBg();
}

//-----------------------------------------------------------------
// ���U���g�X�V�̍X�V����
//-----------------------------------------------------------------
void UpdateResultBg(void)
{
	// �w�i�̍X�V����
	UpdateBg();

	// �J���X�̍X�V����
	UpdateCrow();

	// �؂̗��̍X�V����
	UpdateWoodBack();

	// �؂̍X�V����
	UpdateWood();

	// �u���b�N�̍X�V����
	UpdateBlock();

	// ���U���g�X�R�A�w�i�̍X�V����
	UpdateResultScoreBg();

	// ���U���g�X�R�A�̍X�V����
	UpdateResultScore();
}

//-----------------------------------------------------------------
// ���U���g�w�i�̕`�揈��
//-----------------------------------------------------------------
void DrawResultBg(void)
{
	// �w�i�̕`�揈��
	DrawBg();

	// �J���X�̕`�揈��
	DrawCrow();

	// �؂̗��̕`�揈��
	DrawWoodBack();

	// �u���b�N�̕`�揈��
	DrawBlock();

	// �؂̕`�揈��
	DrawWood();

	// ���U���g�X�R�A�w�i�̕`�揈��
	DrawResultScoreBg();

	// ���U���g�X�R�A�̕`�揈��
	DrawResultScore();
}