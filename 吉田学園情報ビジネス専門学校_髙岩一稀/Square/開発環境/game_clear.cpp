//=============================================================================
//
// �Q�[���N���A���� [game_clear.cpp]
// Author : 
//
//=============================================================================
#include "game_clear.h"
#include "logo.h"
#include "ui_bg.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CGameClear::m_bTransition = false;
int CGameClear::m_nCntInterval = 0;
CGameClear::GameTransition CGameClear::m_gameTransition = CGameClear::GAMETRANSITION_CLEAR;

//=============================================================================
// CGameClear�̃R���X�g���N�^
//=============================================================================
CGameClear::CGameClear(int nPriority) : CScene(nPriority)
{
	// �����o�ϐ��̏�����
	m_bTransition = false;
	m_nCntInterval = 0;
	m_pLogo = NULL;
}

//=============================================================================
// CGameClear�̃f�X�g���N�^
//=============================================================================
CGameClear::~CGameClear()
{

}

//=============================================================================
// ��������
//=============================================================================
CGameClear *CGameClear::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, GameTransition gameTransition)
{
	// �|�C���^�ϐ��̐���
	CGameClear *pGameClear;
	pGameClear = new CGameClear(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pGameClear != NULL)
	{
		// �J�ڏ����̕ۑ�
		m_gameTransition = gameTransition;

		// ����������
		pGameClear->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pGameClear->SetObjType(OBJTYPE_GAMECLEAR);
	}

	return pGameClear;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGameClear::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ���S���̏�����
	switch (m_gameTransition)
	{
	case GAMETRANSITION_CLEAR:
		m_pLogo = CLogo::Create(pos, D3DXVECTOR2(400.0f, 70.0f), CLogo::TEXTURETYPE_GAMECLEAR);
		break;

	case GAMETRANSITION_OVER:
		m_pLogo = CLogo::Create(pos, D3DXVECTOR2(400.0f, 70.0f), CLogo::TEXTURETYPE_GAMEOVER);
		break;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGameClear::Uninit(void)
{
	// ���S���̏I������
	m_pLogo->Uninit();

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGameClear::Update(void)
{
	// ���S���̍X�V����
	m_pLogo->Update();

	// �J�E���g�̑���
	m_nCntInterval++;

	// �J�E���g��������x�i�񂾂�J�ڂ�������
	if (m_nCntInterval % 120 == 0)
	{
		m_bTransition = true;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGameClear::Draw(void)
{
	// ���S���̕`�揈��
	m_pLogo->Draw();
}