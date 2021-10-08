//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : 
//
//=============================================================================
#include "result.h"
#include "bg.h"
#include "result_logo.h"
#include "score.h"
#include "manager.h"
#include "input_keyboard.h"
#include "fade.h"
#include "logo.h"
#include "gamedata_keep_object.h"
#include "input_joypad.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CScore *CResult::m_pScore = NULL;
CScore *CResult::m_pMaxCombo = NULL;
CScore *CResult::m_pKillEnemy = NULL;

//=============================================================================
// CGame�̃R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// CManager�̃f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �w�i�̐���
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		CBg::TEXTURETYPE_GAME);

	// ���U���g���S�̐���
	CResultLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 100.0f, 0.0f), D3DXVECTOR2(500.0f, 100.0f));

	// ���S���̎擾
	CLogo **pResultLogo = CResultLogo::GetResultLogo();

	// �X�R�A�̐���
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, pResultLogo[CResultLogo::LOGOTYPE_YOURSCORE]->GetPosition().y + 100.0f,0.0f),
		D3DXVECTOR2(40.0f,80.0f),
		40.0f,
		CScore::SCOREPLACEMENT_RIGHT);

	// �X�R�A�̐ݒ�
	m_pScore->SetScore(CGameDataKeepObject::GetScore());

	// BGM
	CManager::GetSound()->Play(CSound::SOUND_LABEL_RESULT);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	// �T�E���h��~
	CManager::GetSound()->Stop();

	// �폜
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	// CInputKeyboard�C���X�^���X�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	// ��ʑJ��
	if ((pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK)  || pJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_SHOT_DOWN))&& 
		CFade::GetFade() == CFade::FADE_NONE)
	{
		CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}