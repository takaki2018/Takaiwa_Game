//=============================================================================
//
// UI���� [UI.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "UI.h"

#include "score.h"
#include "number.h"
#include "life_ui.h"
#include "hiscore.h"
#include "game_logo.h"
#include "UI_bg.h"
#include "bg.h"
#include "count_down.h"
#include "gamedata_keep_object.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CScore *CUI::m_pScore = NULL;			// �X�R�A���̃|�C���^
CHiScore *CUI::m_pHiScore = NULL;		// �n�C�X�R�A���̃|�C���^
CLifeUI *CUI::m_pLife = NULL;				// ���C�t���̃|�C���^
CUIBg *CUI::m_pLogoBg = NULL;			// ���S�w�i���
CCountDown *CUI::m_pCountDown = NULL;	// �J�E���g�_�E�����̃|�C���^

//=============================================================================
// CUI�̃R���X�g���N�^
//=============================================================================
CUI::CUI()
{

}

//=============================================================================
// CUI�̃f�X�g���N�^
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CUI::Init(CManager::MODE mode)
{
	// ���C�t�̃e�N�X�`�����[�h
	CLifeUI::Load();

	// UI�w�i�̃e�N�X�`�����[�h
	CUIBg::Load();

	// UI�w�i�̐���
	CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 40.0f, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH, 80.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		CUIBg::TEXTURETYPE_UI);

	// �Q�[����ʂɕK�v��UI���S�̐���
	CGameLogo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));

	// ���C�t�̐���
	m_pLife = CLifeUI::Create(D3DXVECTOR3(600.0f, 40.0f, 0.0f), D3DXVECTOR2(LIFE_SIZE, LIFE_SIZE));

	// �X�R�A�̐���
	m_pScore = CScore::Create(D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR2(20.0f, 40.0f),20.0f,CScore::SCOREPLACEMENT_LEFT);
	
	// �n�C�X�R�A�̐���
	m_pHiScore = CHiScore::Create(D3DXVECTOR3(1240.0f, 50.0f, 0.0f), D3DXVECTOR2(20.0f, 40.0f), 20.0f,CScore::SCOREPLACEMENT_RIGHT);

	// �w�i���̏�����
	m_pLogoBg =  CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH, 80.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),
		CUIBg::TEXTURETYPE_LOGO);

	// �J�E���g�_�E���̐���
	m_pCountDown = CCountDown::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(40.0f, 80.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUI::Uninit(void)
{
	// ���C�t�̃e�N�X�`���A�����[�h
	CLifeUI::Unload();

	// UI�w�i�̃e�N�X�`���A�����[�h
	CUIBg::Unload();

	// �X�R�A�̕ۑ�
	CGameDataKeepObject::SetScore(m_pScore->GetScore());
}

//=============================================================================
// �X�V����
//=============================================================================
void CUI::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CUI::Draw(void)
{

}