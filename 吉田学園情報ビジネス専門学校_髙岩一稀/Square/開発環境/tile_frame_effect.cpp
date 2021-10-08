//=============================================================================
//
// �^�C���t���[���G�t�F�N�g���� [tile_frame_effect.cpp]
// Author : 
//
//=============================================================================
#include "tile_frame_effect.h"
#include "manager.h"
#include "renderer.h"
#include "tile.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTileFrameEffect::m_pTexture = NULL;

//=============================================================================
// CTileFrameEffect�̃R���X�g���N�^
//=============================================================================
CTileFrameEffect::CTileFrameEffect(int nPriority) : CScene2D(nPriority)
{
	// �����o�ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// CTileFrameEffect�̃f�X�g���N�^
//=============================================================================
CTileFrameEffect::~CTileFrameEffect()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CTileFrameEffect::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_tile_frame.jpg", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CTileFrameEffect::Unload(void)
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
CTileFrameEffect *CTileFrameEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR3 move)
{
	// �|�C���^�ϐ��̐���
	CTileFrameEffect *pTileFrameEffect;
	pTileFrameEffect = new CTileFrameEffect(CScene::PRIORITY_EFFECT);

	// NULL�`�F�b�N
	if (pTileFrameEffect != NULL)
	{
		// ����������
		pTileFrameEffect->Init(pos, size, move);

		// �e�N�X�`���ݒ�
		pTileFrameEffect->BindTexture(m_pTexture);
	}

	return pTileFrameEffect;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTileFrameEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR3 move)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_EFFECT);

	// �����o�ϐ��̏�����
	m_move = move;

	// �F�̎擾
	D3DXCOLOR col = GetCol();

	// ���l�̐ݒ�
	col.a = 0.0f;

	// �F�̐ݒ�
	SetCol(col);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTileFrameEffect::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTileFrameEffect::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();

	// �ʒu���擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = GetSize();

	// �F�̎擾
	D3DXCOLOR col = GetCol();

	// ���l�̑���
	col.a += 0.1f;

	if (col.a >= 0.6f)
	{
		col.a = 0.6f;
	}
	// �ʒu�̕ۑ�
	m_posOld = pos;

	// �ړ�������
	pos += m_move;

	// �^�C���Ƃ̂����蔻��
	if (CollitionTile(pos, size))
	{
		return;
	}
	// �ʒu�̐ݒ�
	SetPosition(pos);

	// �F�̐ݒ�
	SetCol(col);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTileFrameEffect::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �^�C���Ƃ̓����蔻��
//=============================================================================
bool CTileFrameEffect::CollitionTile(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �I�u�W�F�N�g���̎擾
	CScene *pScene = GetScene(CScene::PRIORITY_TILE);

	// �G�Ƃ̓����蔻��
	while (pScene)
	{
		// ���̏����擾
		CScene *pSceneNext = pScene->GetNext();

		// �I�u�W�F�N�g�̎�ނ̎擾
		CScene::OBJTYPE objType = pScene->GetObjType();

		// �I�u�W�F�N�g�̎�ނ�Enemy�������珈��
		if (objType == CScene::OBJTYPE_TILE)
		{
			// �G�C���X�^���X���̎擾
			CTile *pTile = (CTile*)pScene;

			switch (pTile->GetTileType())
			{
			case CTile::TILETYPE_INVISIBLE:
				// �^�C���Ƃ̓����蔻��
				if (pTile->CollisionObject(&pos, m_posOld, &m_move, size))
				{
					// ����
					Uninit();

					return true;
				}
				break;

			default:
				break;
			}
		}
		// ���̏���
		pScene = pSceneNext;
	}
	return false;
}