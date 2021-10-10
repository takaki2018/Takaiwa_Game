//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "bg.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBg::m_pTexture[TEXTURETYPE_MAX] = {}; // �e�N�X�`�����̃|�C���^

//=============================================================================
// CBg�̃R���X�g���N�^
//=============================================================================
CBg::CBg(int nPriority) : CScene2D(nPriority)
{
	// �ϐ��̏�����
	m_nTextureNumber = 0;
}

//=============================================================================
// CBg�̃f�X�g���N�^
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CBg::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tile.jpg", &m_pTexture[TEXTURETYPE_TITLE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stage_bg.jpg", &m_pTexture[TEXTURETYPE_GAME]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CBg::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntBg = 0; nCntBg < TEXTURETYPE_MAX; nCntBg++)
	{
		if (m_pTexture[nCntBg] != NULL)
		{
			m_pTexture[nCntBg]->Release();
			m_pTexture[nCntBg] = NULL;
		}			  
	}
}

//=============================================================================
// ��������
//=============================================================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber)
{
	// �|�C���^�ϐ��̐���
	CBg *pBg;
	pBg = new CBg(CScene::PRIORITY_BG);

	// NULL�`�F�b�N
	if (pBg != NULL)
	{
		// ����������
		pBg->Init(pos, size,col,nTextureNumber);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pBg->SetObjType(OBJTYPE_BG);
	}

	return pBg;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �e�N�X�`���ԍ��̕ۑ�
	m_nTextureNumber = nTextureNumber;

	// �F�̎w��
	SetCol(col);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_pTexture[m_nTextureNumber]);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBg::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBg::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBg::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}