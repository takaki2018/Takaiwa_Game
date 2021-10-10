//=============================================================================
//
// UI�w�i���� [UI_bg.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "UI_bg.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CUIBg::m_pTexture[TEXTURETYPE_MAX] = {};		// �e�N�X�`�����̃|�C���^

//=============================================================================
// CUIBg�̃R���X�g���N�^
//=============================================================================
CUIBg::CUIBg(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CUIBg�̃f�X�g���N�^
//=============================================================================
CUIBg::~CUIBg()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CUIBg::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui_bg2.jpg", &m_pTexture[TEXTURETYPE_UI]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_bg.jpg", &m_pTexture[TEXTURETYPE_LOGO]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CUIBg::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < TEXTURETYPE_MAX; nCntTexture++)
	{
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CUIBg *CUIBg::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber)
{
	// �|�C���^�ϐ��̐���
	CUIBg *pUIBg;
	pUIBg = new CUIBg(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pUIBg != NULL)
	{
		// ����������
		pUIBg->Init(pos, size,col);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pUIBg->SetObjType(OBJTYPE_BG);

		// �e�N�X�`���ݒ�
		pUIBg->BindTexture(m_pTexture[nTextureNumber]);
	}

	return pUIBg;
}


//=============================================================================
// ����������
//=============================================================================
HRESULT CUIBg::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col)
{
	// CScore�̏���������
	CScene2D::Init(pos, size);

	// �F�̐ݒ�
	SetCol(col);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUIBg::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUIBg::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUIBg::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}