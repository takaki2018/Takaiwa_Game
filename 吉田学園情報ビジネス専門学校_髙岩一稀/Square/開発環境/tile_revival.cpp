//=============================================================================
//
// �^�C���������� [tile_revival.cpp]
// Author : 
//
//=============================================================================
#include "tile_revival.h"
#include "manager.h"
#include "renderer.h"
#include "stage.h"
#include "scene.h"
#include "tile.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAAX_CORNER		(4)				// �p�̐�

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTileRevival::m_pTexture = NULL;

//=============================================================================
// CTileRevival�̃R���X�g���N�^
//=============================================================================
CTileRevival::CTileRevival(int nPriority) : CScene2D(nPriority)
{
	// �����o�ϐ��̏�����
	m_tileType = TILETYPE_NONE;
}

//=============================================================================
// CTileRevival�̃f�X�g���N�^
//=============================================================================
CTileRevival::~CTileRevival()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CTileRevival::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tile.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CTileRevival::Unload(void)
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
CTileRevival *CTileRevival::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type, int nNum)
{
	// �|�C���^�ϐ��̐���
	CTileRevival *pTileRevival;
	pTileRevival = new CTileRevival(CScene::PRIORITY_TILEREVIVAL);

	// NULL�`�F�b�N
	if (pTileRevival != NULL)
	{
		// ����������
		pTileRevival->Init(pos, size, type,nNum);

		// �e�N�X�`���ݒ�
		pTileRevival->BindTexture(m_pTexture);
	}

	return pTileRevival;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTileRevival::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type, int nNum)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_TILE);

	// �����o�ϐ��̏�����
	m_tileType = type;

	if (m_tileType == TILETYPE_BLACK)
	{
		// �e�N�X�`�����W�̕ύX
		SetTex(D3DXVECTOR2(0.5f, 1.0f), D3DXVECTOR2(2, 1));
	}

	// ���Ԗڂ̃^�C�����ۑ�
	m_nNum = nNum;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTileRevival::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTileRevival::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = GetSize();

	// �p�x�̎擾
	D3DXVECTOR3 rot = GetRotate();

	// �T�C�Y�����Z
	size.x += 1.5f;
	size.y += 1.5f;

	// �p�x�����Z
	rot.z += 0.05f;

	// �T�C�Y��ݒ�
	SetSize(size);

	// �p�x��ݒ�
	SetRotate(rot);

	if (size.x >= TILE_SIZE || size.y >= TILE_SIZE)
	{
		// �������ĂȂ���Ԃɂ���
		CStage::SetRevival(false, m_nNum);

		// �^�C������������
		CTile *pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, m_nNum);
		pTile->FillTile(CTile::TILETYPE_BLACK);
	}

	// ������Ԃ̎擾
	if (CStage::GetRevival(m_nNum) == false)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTileRevival::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}