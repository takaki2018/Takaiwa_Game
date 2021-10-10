//=============================================================================
//
// �J�E���g�_�E������ [count_down.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "count_down.h"
#include "number.h"
#include "logo.h"
#include "ui_bg.h"
#include "UI.h"
#include "manager.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CCountDown::m_bFinishCountDown = false;	// �J�E���g�_�E�����I��������ǂ���

//=============================================================================
// CCountDown�̃R���X�g���N�^
//=============================================================================
CCountDown::CCountDown(int nPriority) : CScene(nPriority)
{
	// �����o�ϐ��̏�����
	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber] = NULL;
	}
	m_pLogo = NULL;
	m_bFinishCountDown = false;
	m_nCntCountDown = 0;
	m_nNumCountDown = MAX_NUMBER;
}

//=============================================================================
// CCountDown�̃f�X�g���N�^
//=============================================================================
CCountDown::~CCountDown()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CCountDown::Load(void)
{
	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CCountDown::Unload(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CCountDown *CCountDown::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CCountDown *pCountDown;
	pCountDown = new CCountDown(CScene::PRIORITY_COUNTDOWN);

	// NULL�`�F�b�N
	if (pCountDown != NULL)
	{
		// ����������
		pCountDown->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pCountDown->SetObjType(OBJTYPE_COUNTDOWN);
	}

	return pCountDown;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCountDown::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �������̏�����
	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create(pos,size);

		// �����̃Z�b�g
		m_apNumber[nCntNumber]->SetNumber(nCntNumber + 1);

		// �F�̏�����
		if (nCntNumber + 1 == m_nNumCountDown)
		{
			m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
	// ���S���̏�����
	m_pLogo = CLogo::Create(pos,D3DXVECTOR2(400.0f,70.0f),CLogo::TEXTURETYPE_GAMESTART);

	// �F�̏�����
	m_pLogo->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCountDown::Uninit(void)
{
	// �������̏I������
	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber]->Uninit();
	}

	// ���S���̏I������
	m_pLogo->Uninit();

	// ���S�w�i�̏I������
	if (m_bFinishCountDown == true)
	{
		CUI::GetLogoBg()->Uninit();
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCountDown::Update(void)
{
	// �J�E���g�_�E���̍X�V
	if (m_nCntCountDown % 60 == 0)
	{
		//�@�J�E���g�_�E���J�E���g�����炷
		m_nNumCountDown--;

		// �J�E���g�_�E����0�ȏ�̂Ƃ�
		if (m_nNumCountDown > 0)
		{
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				// �J�E���g����v���Ă����炻�̐�����������悤�ɂ���
				if ((nCntNumber + 1) == m_nNumCountDown)
				{
					m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{
					m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
			}
			// ���ʉ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (m_nNumCountDown == 0)
		{
			// �����Ȃ�����
			m_apNumber[m_nNumCountDown]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			// 0�̂Ƃ����S��������悤�ɂ���
			m_pLogo->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ���ʉ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_START);
		}
		else
		{
			// aBGM
			CManager::GetSound()->Play(CSound::SOUND_LABEL_GAME);

			// �J�E���g�_�E�����I�������Ƃ�`����
			m_bFinishCountDown = true;

			// �I������
			Uninit();
		}
	}
	// �J�E���g�̑���
	m_nCntCountDown++;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCountDown::Draw(void)
{
	// �������̕`�揈��
	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber]->Draw();
	}

	// ���S���̕`�揈��
	m_pLogo->Draw();
}