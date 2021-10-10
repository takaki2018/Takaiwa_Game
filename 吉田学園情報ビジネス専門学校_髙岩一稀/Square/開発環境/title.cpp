//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "title.h"
#include "player.h"
#include "manager.h"
#include "input_keyboard.h"
#include "fade.h"
#include "logo.h"
#include "title_logo.h"
#include "bg.h"
#include "UI_bg.h"
#include "sound.h"
#include "fade.h"
#include "model.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CTitle::m_bPressEnter = false;				// �G���^�[�����������ǂ���
CUIBg *CTitle::m_apLogoBg[MAX_LOGOBG] = {};		// ���S�w�i���
												
//=============================================================================
// CTitle�̃R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// CTitle�̃f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// UI�w�i�e�N�X�`���̃��[�h
	CUIBg::Load();

	// �w�i�̐���
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f),
		CBg::TEXTURETYPE_GAME);

	// UI�w�i�̐���
	CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 370.0f, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH, 320.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		CUIBg::TEXTURETYPE_UI);

	// ���S�w�i�̏�����
	for (int nCntLogoBg = 0; nCntLogoBg < MAX_LOGOBG; nCntLogoBg++)
	{
		m_apLogoBg[nCntLogoBg] = CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 370.0f + nCntLogoBg * 45.0f, 0.0f),
			D3DXVECTOR2(1280.0f, 35.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CUIBg::TEXTURETYPE_LOGO);
	}

	// ���S�̐���
	CTitleLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 290.0f, 0.0f), D3DXVECTOR2(600.0f, 100.0f));

	// BGM
	CManager::GetSound()->Play(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	// �T�E���h��~
	CManager::GetSound()->Stop();

	// UI�w�i�e�N�X�`���̃A�����[�h
	CUIBg::Unload();

	// �폜
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	// ��ʑJ��
	if (CTitleLogo::GetTransition() && CFade::GetFade() == CFade::FADE_NONE)
	{
		// �Q�[���ɑJ��
		CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}