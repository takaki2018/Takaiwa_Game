//=============================================================================
//
// �Q�[�����S���� [game_logo.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "game_logo.h"
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "input_keyboard.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CGameLogo::bTransition = false;		// �J�ڂ��邩�ǂ���

//=============================================================================
// CGameLogo�̃R���X�g���N�^
//=============================================================================
CGameLogo::CGameLogo(int nPriority) : CScene(nPriority)
{
	// �ϐ��̏�����
	for (int nCntTitleLogo = 0; nCntTitleLogo < LOGOTYPE_MAX; nCntTitleLogo++)
	{
		m_apLogo[nCntTitleLogo] = NULL;
	}
}

//=============================================================================
// CGameLogo�̃f�X�g���N�^
//=============================================================================
CGameLogo::~CGameLogo()
{

}

//=============================================================================
// ��������
//=============================================================================
CGameLogo *CGameLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CGameLogo *pGameLogo;
	pGameLogo = new CGameLogo(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pGameLogo != NULL)
	{
		// ����������
		pGameLogo->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pGameLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pGameLogo;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGameLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ���S�̏���������
	m_apLogo[LOGOTYPE_SCORE] = CLogo::Create(D3DXVECTOR3(80.0f, 20.0f, 0.0f), D3DXVECTOR2(80.0f, 20.0f), CLogo::TEXTURETYPE_SCORE);
	m_apLogo[LOGOTYPE_HIGHSCORE] = CLogo::Create(D3DXVECTOR3(1170.0f, 20.0f, 0.0f), D3DXVECTOR2(160.0f, 20.0f), CLogo::TEXTURETYPE_HIGHSCORE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGameLogo::Uninit(void)
{
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			// ���S�̏I������
			m_apLogo[nCnttitleLogo]->Uninit();
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGameLogo::Update(void)
{
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			// ���S�̍X�V����
			m_apLogo[nCnttitleLogo]->Update();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGameLogo::Draw(void)
{
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			// ���S�̕`�揈��
			m_apLogo[nCnttitleLogo]->Draw();
		}
	}
}