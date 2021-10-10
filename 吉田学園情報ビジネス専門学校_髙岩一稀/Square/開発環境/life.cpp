//=============================================================================
//
// ���C�t���� [life.cpp]
// Author : istuki takaiwa
//
//=============================================================================
#include "life.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLife::m_pTexture = NULL;		// �e�N�X�`�����̃|�C���^

//=============================================================================
// CLife�̃R���X�g���N�^
//=============================================================================
CLife::CLife(int nPriority) : CScene(nPriority)
{
	// �ϐ��̏�����
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		m_apScene2D[nCntLife] = NULL;
	}
	m_nLife = MAX_LIFE;
}

//=============================================================================
// CBg�̃f�X�g���N�^
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CLife::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CLife::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ��������
//=============================================================================
CLife *CLife::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CLife *pLife;
	pLife = new CLife(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pLife != NULL)
	{
		// ����������
		pLife->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pLife->SetObjType(OBJTYPE_LIFE);
	}

	return pLife;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLife::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	for (int nCntLife = 0; nCntLife < m_nLife; nCntLife++)
	{
		// �������̊m��
		m_apScene2D[nCntLife] = new CScene2D(CScene::PRIORITY_UI);

		// CScene2D�̏���������
		m_apScene2D[nCntLife]->Init(D3DXVECTOR3(pos.x + 40.0f * nCntLife,pos.y,0.0f), size);

		// �e�N�X�`���̊��蓖��
		m_apScene2D[nCntLife]->BindTexture(m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLife::Uninit(void)
{
	for (int nCntLife = 0; nCntLife < m_nLife; nCntLife++)
	{
		// CScene2D�̏I������
		m_apScene2D[nCntLife]->Uninit();
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLife::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CLife::Draw(void)
{
	for (int nCntLife = 0; nCntLife < m_nLife; nCntLife++)
	{
		// CScene2D�̕`�揈��
		m_apScene2D[nCntLife]->Draw();
	}
}

//=============================================================================
// ���C�t�̐ݒ菈��
//=============================================================================
void CLife::SetLife(int nlife)
{
	// ���C�t�̃Z�b�g
	m_nLife = nlife;

	// CScene2D�̏I������
	m_apScene2D[m_nLife]->Uninit();
}