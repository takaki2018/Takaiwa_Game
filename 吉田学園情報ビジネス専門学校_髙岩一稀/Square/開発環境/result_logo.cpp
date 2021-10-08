//=============================================================================
//
// ���U���g���S���� [result_logo.cpp]
// Author : 
//
//=============================================================================
#include "result_logo.h"
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "input_keyboard.h"
#include "ui_bg.h"
#include "title.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CLogo *CResultLogo::m_apLogo[LOGOTYPE_MAX] = {};

//=============================================================================
// CResultLogo�̃R���X�g���N�^
//=============================================================================
CResultLogo::CResultLogo(int nPriority) : CScene(nPriority)
{
	// �ϐ��̏�����
	for (int nCntResultLogo = 0; nCntResultLogo < LOGOTYPE_MAX; nCntResultLogo++)
	{
		m_apLogo[nCntResultLogo] = NULL;
	}
}

//=============================================================================
// CResultLogo�̃f�X�g���N�^
//=============================================================================
CResultLogo::~CResultLogo()
{

}

//=============================================================================
// ��������
//=============================================================================
CResultLogo *CResultLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CResultLogo *pResultLogo;
	pResultLogo = new CResultLogo(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pResultLogo != NULL)
	{
		// ����������
		pResultLogo->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pResultLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pResultLogo;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResultLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ���S�̏���������
	m_apLogo[LOGOTYPE_RESULT] = CLogo::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), 
		D3DXVECTOR2(220.0f, 60.0f), 
		CLogo::TEXTURETYPE_RESULTLOGO);

	m_apLogo[LOGOTYPE_ARCADEMODE] = CLogo::Create(D3DXVECTOR3(pos.x, pos.y + 60.0f, 0.0f),
		D3DXVECTOR2(300.0f, 40.0f),
		CLogo::TEXTURETYPE_ARCADEMODE);

	m_apLogo[LOGOTYPE_YOURSCORE] = CLogo::Create(D3DXVECTOR3(400.0f, pos.y + 120.0f, 0.0f),
		D3DXVECTOR2(250.0f, 40.0f),
		CLogo::TEXTURETYPE_YOURSCORE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResultLogo::Uninit(void)
{
	// ���S�̏I������
	for (int nCntResultLogo = 0; nCntResultLogo < LOGOTYPE_MAX; nCntResultLogo++)
	{
		if (m_apLogo[nCntResultLogo] != NULL)
		{
			m_apLogo[nCntResultLogo]->Uninit();
		}
	}

	// �j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResultLogo::Update(void)
{
	// ���S�̍X�V����
	for (int nCnttitleLogo = 0; nCnttitleLogo < LOGOTYPE_MAX; nCnttitleLogo++)
	{
		if (m_apLogo[nCnttitleLogo] != NULL)
		{
			m_apLogo[nCnttitleLogo]->Update();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResultLogo::Draw(void)
{
	// ���S�̕`�揈��
	for (int nCntResultLogo = 0; nCntResultLogo < LOGOTYPE_MAX; nCntResultLogo++)
	{
		if (m_apLogo[nCntResultLogo] != NULL)
		{
			m_apLogo[nCntResultLogo]->Draw();
		}
	}
}