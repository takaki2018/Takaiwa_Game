//-----------------------------------------------------------------
//
// UI (ui.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "ui.h"
#include "score.h"
#include "score_bg.h"
#include "hidden_score.h"
#include "player_marker.h"
#include "point.h"


//-----------------------------------------------------------------
// UI�̏���������
//-----------------------------------------------------------------
HRESULT InitUI(void)
{
	// �|�C���g�̏���������
	InitPoint();

	// �v���C���[�}�[�J�[�̏���������
	InitPlayerMarker();

	// �X�R�A�w�i�̏���������
	InitScoreBg();

	// �X�R�A�̏���������
	InitScore();
	
	// �͂Ă˃}�[�N�̏���������
	InitHiddenScore();

	return S_OK;
}

//-----------------------------------------------------------------
// UI�̏I������
//-----------------------------------------------------------------
void UninitUI(void)
{
	// �͂Ăȃ}�[�N�̏I������
	UninitHiddenScore();

	// �X�R�A�̏I������
	UninitScore();

	// �X�R�A�w�i�̏I������
	UninitScoreBg();

	// �v���C���[�}�[�J�[�̏I������
	UninitPlayerMarker();

	// �|�C���g�̏I������
	UninitPoint();
}

//-----------------------------------------------------------------
// UI�̍X�V����
//-----------------------------------------------------------------
void UpdateUI(void)
{
	// �|�C���g�̍X�V����
	UpdatePoint();

	// �v���C���[�}�[�J�[�̍X�V����
	UpdatePlayerMarker();

	// �X�R�A�̍X�V����
	UpdateScore();

	// �͂Ăȃ}�[�N�̍X�V����
	UpdateHiddenScore();
}

//-----------------------------------------------------------------
// UI�̕`�揈��
//-----------------------------------------------------------------
void DrawUI(void)
{
	// �|�C���g�̕`�揈��
	DrawPoint();

	// �v���C���[�}�[�J�[�̕`�揈��
	DrawPlayerMarker();

	// �X�R�A�w�i�̕`�揈��
	DrawScoreBg();

	// �X�R�A�̕`�揈��
	DrawScore();

	// �͂Ăȃ}�[�N�̕`�揈��
	DrawHiddenScore();
}