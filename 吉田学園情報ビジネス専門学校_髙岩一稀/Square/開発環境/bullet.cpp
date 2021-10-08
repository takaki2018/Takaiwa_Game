//=============================================================================
//
// �e���� [bullet.cpp]
// Author : 
//
//=============================================================================
#include "bullet.h"
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "collision.h"
#include "life.h"
#include "setparticle.h"
#include "game.h"
#include "tile.h"
#include "UI.h"
#include "setparticle.h"
#include "spawn_enemy.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
// CBullet�̃R���X�g���N�^
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{
	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bulletType = BULETTTYPE_NONE;
}

//=============================================================================
// CPlayer�̃f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CBullet::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CBullet::Unload(void)
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
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, BULLETTYPE type, BULLETDIRECTION direction)
{
	// �|�C���^�ϐ��̐���
	CBullet *pBullet;
	pBullet = new CBullet(CScene::PRIORITY_BULLET);

	// NULL�`�F�b�N
	if (pBullet != NULL)
	{
		// ����������
		pBullet->Init(pos, move, size, type,direction);

		// �e�N�X�`���ݒ�
		pBullet->BindTexture(m_pTexture);

		// �I�u�W�F�N�g�̎�ސݒ�
		pBullet->SetObjType(CScene::OBJTYPE_BULLET);
	}

	return pBullet;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, BULLETTYPE type, BULLETDIRECTION direction)
{
	// �����o�ϐ��̏�����
	m_move = move;
	m_bulletType = type;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �e�N�X�`�����W�̐ݒ�
	switch (m_bulletType)
	{
	case BULLETTYPE_PLEYER:
		SetTex(D3DXVECTOR2(0.5f,1.0f), D3DXVECTOR2(2.0f, 1.0f));
		break;

	case BULLETTYPE_ENEMY:
		SetTex(D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(2.0f, 1.0f));
		break;
	}

	// �����̐ݒ�
	switch (direction)
	{
	case BULLETDIRECTION_DOWN:
		SetRotateVertex(D3DX_PI);
		break;

	case BULLETDIRECTION_RIGHT:
		SetRotateVertex((D3DX_PI / 2.0f) * 3.0f);
		break;

	case BULLETDIRECTION_LEFT:
		SetRotateVertex(D3DX_PI / 2.0f);
		break;
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = GetSize();

	// �ʒu�̕ۑ�
	SetOldPosition(pos);

	// �e�̈ړ�����
	Move();

	// �^�C���Ƃ̓����蔻��
	CollisionTile();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �e�̈ړ�����
//=============================================================================
void CBullet::Move(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// ���W�̐ݒ�
	pos += m_move;

	// ���W�̐ݒ�
	CScene2D::SetPosition(pos);
}

//=============================================================================
// �^�C���Ƃ̓����蔻��
//=============================================================================
void CBullet::CollisionTile(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

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

			// �^�C���̎�ނ��擾
			CTile::TILETYPE tileType = pTile->GetTileType();

			switch (tileType)
			{// ���^�C���̏ꍇ
			case CTile::TILETYPE_BLACK:
				if (m_bulletType == BULLETTYPE_PLEYER)
				{
					pTile->FillTileforBullet(pos, size, m_posOld, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), CTile::TILETYPE_WHITE,this);
				}
				break;
				// ���^�C���̏ꍇ
			case CTile::TILETYPE_WHITE:
				if (m_bulletType == BULLETTYPE_ENEMY)
				{
					pTile->FillTileTypeWhite(pos, size);
				}
				break;
				// �����^�C���̏ꍇ
			case CTile::TILETYPE_INVISIBLE:
				if (pTile->CollisionObject(&pos, m_posOld, &m_move, size))
				{
					// ����������폜
					Uninit();
					return;
				}
				break;
				// ���ɕς��^�C���������ꍇ
			case CTile::TILETYPE_CHANGE_BLACK:
				if (m_bulletType == BULLETTYPE_PLEYER)
				{
					pTile->FillTileTypeChangeBlack(pos, size);
					pTile->FillTileforBullet(pos, size, m_posOld, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), CTile::TILETYPE_WHITE, this);
				}
				break;

			default:
				break;
			}
		}
		// ���̏���
		pScene = pSceneNext;;
	}
}