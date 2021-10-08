//=============================================================================
//
// �G����UI���� [spawn_enemyUI.cpp]
// Author : 
//
//=============================================================================
#include "spawn_enemyUI.h"
#include "enemy.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
#define INTERVAL_BASE		(40)		// �C���^�[�o���̊�l
#define MOVEALPHA_BASE		(0.05f)		// ���l�ړ��ʂ̊�l

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSpawnEnemyUI::m_pTexture = NULL;

//=============================================================================
// CSpawnEnemyUI�̃R���X�g���N�^
//=============================================================================
CSpawnEnemyUI::CSpawnEnemyUI(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CSpawnEnemyUI�̃f�X�g���N�^
//=============================================================================
CSpawnEnemyUI::~CSpawnEnemyUI()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CSpawnEnemyUI::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spawn_enemy.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CSpawnEnemyUI::Unload(void)
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
CSpawnEnemyUI *CSpawnEnemyUI::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, int nCntSpawnInterval, int nEnemyType)
{
	// �|�C���^�ϐ��̐���
	CSpawnEnemyUI *pSpawnEnemyUI;
	pSpawnEnemyUI = new CSpawnEnemyUI(CScene::PRIORITY_ENEMY);

	// NULL�`�F�b�N
	if (pSpawnEnemyUI != NULL)
	{
		// ����������
		pSpawnEnemyUI->Init(pos, size,fMoveAI,nCntSpawnInterval,nEnemyType);

		// �e�N�X�`���ݒ�
		pSpawnEnemyUI->BindTexture(m_pTexture);

		// �I�u�W�F�N�g�̎�ސݒ�
		pSpawnEnemyUI->SetObjType(CScene::OBJTYPE_SPAWNENEMYUI);
	}

	return pSpawnEnemyUI;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSpawnEnemyUI::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, int nCntSpawnInterval, int nEnemyType)
{
	// �ϐ��̏�����(��l)
	m_nCntSpawnInterval = nCntSpawnInterval;
	m_nCounter = 0;			
	m_nCntFlash = 0;
	m_nEnemyType = nEnemyType;

	// ��l�ƈ������r���A���l�̈ړ��ʂ����߂�
	float fInterbalBase = (float)INTERVAL_BASE / (float)m_nCntSpawnInterval;
	m_fMoveAlpha = fInterbalBase * MOVEALPHA_BASE;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// ���_�J���[���̎擾
	D3DXCOLOR col = GetCol();

	// ���l��ݒ�
	col.a = m_fMoveAlpha;

	// ���_�J���[�̐ݒ�
	SetCol(col);

	// ��{�ړ��ʂ̐ݒ�
	m_fMoveAI = fMoveAI;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSpawnEnemyUI::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSpawnEnemyUI::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();

	// �J�E���^�[�̍X�V
	m_nCounter++;

	// �_�ŏ���
	Blinking();

	// �_�ŉ񐔂�3��ɂȂ�����G���o��������
	if (m_nCntFlash == 3)
	{
		CreateEnemy();

		return;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSpawnEnemyUI::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �_�ŏ���
//=============================================================================
void CSpawnEnemyUI::Blinking(void)
{
	// ���_�J���[���̎擾
	D3DXCOLOR col = GetCol();

	// �����̐��l���v�Z
	float fCommon = (float)(m_nCounter % m_nCntSpawnInterval);
	float fHalf = (float)m_nCntSpawnInterval / 2.0f;

	// �_�ŏ���
	if (fCommon < fHalf)
	{
		// ���l�̑���
		col.a += m_fMoveAlpha;

		if (col.a >= 1.0f)
		{
			col.a = 1.0f;
		}
	}
	else if (fCommon >= fHalf && fCommon < m_nCntSpawnInterval)
	{
		// ���l�̌��Z
		col.a -= m_fMoveAlpha;

		if (col.a <= 0.0f)
		{
			col.a = 0.0f;
		}
	}
	if (fCommon == m_nCntSpawnInterval - 1)
	{
		// �_�ŉ񐔂��J�E���g
		m_nCntFlash++;
	}

	// ���_�J���[�̐ݒ�
	SetCol(col);
}

//=============================================================================
// �G�̐�������
//=============================================================================
void CSpawnEnemyUI::CreateEnemy(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = GetSize();

	switch (m_nEnemyType)
	{
	case CEnemy::ENEMYTYPE_NORMAL:
		// �G�̐���
		CEnemy::Create(pos, size,m_fMoveAI, CEnemy::ENEMYTYPE_NORMAL);
		break;
	}

	// �폜
	Uninit();
}