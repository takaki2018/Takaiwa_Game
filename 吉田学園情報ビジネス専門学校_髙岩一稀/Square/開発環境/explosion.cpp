//=============================================================================
//
// �������� [explosion.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "score.h"
#include "game.h"
#include "UI.h"
#include "tile.h"
#include "spawn_enemy.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture[EXPLOSION_TEXTURETYPE_MAX] = {};	// �e�N�X�`�����̃|�C���^

//=============================================================================
// CExplosion�̃R���X�g���N�^
//=============================================================================
CExplosion::CExplosion(int nPriority) : CScene2D(nPriority)
{
	// �����o�ϐ��̏�����
	m_explosion.sizeMove = 0.0f;
	m_explosion.nCnt = 0;
	m_explosion.nCntSwitch = 0;
	m_explosion.explosionType = EXPLOSIONTYPE_NONE;
	m_explosion.bSwitch = false;
}

//=============================================================================
// CPlayer�̃f�X�g���N�^
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CExplosion::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_explosion.png", &m_pTexture[EXPLOSION_TEXTURETYPE_NORMAL]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect_explosion_wave.png", &m_pTexture[EXPLOSION_TEXTURETYPE_WAVE]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CExplosion::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < EXPLOSION_TEXTURETYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, float sizeMove, float fInertia,int nCntSwitch, float fMoveAlpha, ExplosionType explosionType, Explosion_TextureType textureType)
{
	// �|�C���^�ϐ��̐���
	CExplosion *pExplosion;
	pExplosion = new CExplosion(CScene::PRIORITY_EFFECT);

	// NULL�`�F�b�N
	if (pExplosion != NULL)
	{
		// ����������
		pExplosion->Init(pos, size,col,sizeMove, fInertia,nCntSwitch, fMoveAlpha,explosionType);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pExplosion->SetObjType(OBJTYPE_EXPLOSION);

		// �e�N�X�`���ݒ�
		pExplosion->BindTexture(m_pTexture[textureType]);
	}

	return pExplosion;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, float sizeMove, float fInertia,int nCntSwitch, float fMoveAlpha, ExplosionType explosionType)
{
	// �����o�ϐ��̏�����
	m_explosion.sizeMove = sizeMove;
	m_explosion.fInertia = fInertia;
	m_explosion.nCnt = 0;
	m_explosion.nCntSwitch = nCntSwitch;
	m_explosion.explosionType = explosionType;
	m_explosion.bSwitch = false;
	m_explosion.fMoveAlpha = fMoveAlpha;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �F�̐ݒ�
	CScene2D::SetCol(col);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CExplosion::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CExplosion::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();

	// �^�C�v�ɂ���ď����𕪂���
	if (m_explosion.explosionType == EXPLOSIONTYPE_WAVE)
	{
		// �Ռ��g����
		WaveExplosion();
	}
	else
	{
		// �ʏ폈��
		NormalExplosion();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CExplosion::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �ʏ픚������
//=============================================================================
void CExplosion::NormalExplosion(void)
{
	// �T�C�Y�̎擾
	D3DXVECTOR2 size = GetSize();

	// �J�E���g�X�V
	m_explosion.nCnt++;

	// �T�C�Y���������Ă���Ƃ�
	if (m_explosion.bSwitch == false)
	{
		// �؂�Ԃ��J�E���g�Ɠ����ɂȂ�����T�C�Y�����ʂ�-1.0f����Z����
		if (m_explosion.nCnt == m_explosion.nCntSwitch)
		{
			m_explosion.sizeMove *= -1.0f;

			// �؂�ւ������Ƃ�`����
			m_explosion.bSwitch = true;
		}
	}

	// �������v���C���[�ȊO�̂������Ƃ�
	if (m_explosion.explosionType != EXPLOSIONTYPE_PLAYER)
	{
		if (HitEnemy())
		{
			return;
		}
	}
	// �^�C���Ƃ̂����蔻��
	CollosionTile();

	// �T�C�Y��0�����ɂȂ�����폜
	if (size.x < 0.0f)
	{
		Uninit();

		return;
	}
	// �F���̎擾
	D3DXCOLOR col = GetCol();

	// ���l�̈ړ�
	col.a -= m_explosion.fMoveAlpha;;

	// �F�̐ݒ�
	SetCol(col);

	// ���l��0.0f�ȉ��̎��폜
	if (col.a <= 0.0f)
	{
		Uninit();

		return;
	}

	// ����
	m_explosion.sizeMove += (0.0f - m_explosion.sizeMove) * m_explosion.fInertia;

	// �T�C�Y�̍X�V
	size.x += m_explosion.sizeMove;
	size.y += m_explosion.sizeMove;

	// �T�C�Y�̐ݒ�
	SetSize(size);
}

//=============================================================================
// �Ռ��g����
//=============================================================================
void CExplosion::WaveExplosion(void)
{
	// �T�C�Y�̎擾
	D3DXVECTOR2 size = GetSize();

	// �F���̎擾
	D3DXCOLOR col = GetCol();

	// �T�C�Y�̍X�V
	size.x += m_explosion.sizeMove;
	size.y += m_explosion.sizeMove;

	// ���l�̈ړ�
	col.a -= m_explosion.fMoveAlpha;;

	// �T�C�Y�̐ݒ�
	SetSize(size);

	// �F�̐ݒ�
	SetCol(col);

	// ���l��0.0f�ȉ��̎��폜
	if (col.a <= 0.0f)
	{
		Uninit();
	}
}

//=============================================================================
// �G�Ƃ̓����蔻�菈��
//=============================================================================
bool CExplosion::HitEnemy(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �I�u�W�F�N�g���̎擾
	CScene *pScene = GetScene(CScene::PRIORITY_ENEMY);

		// NULL�`�F�b�N
	while (pScene)
	{
		// ���̏����擾
		CScene *pSceneNext = pScene->GetNext();

		if (!pScene->GetDeath())
		{
			// �I�u�W�F�N�g�̎�ނ̎擾
			CScene::OBJTYPE objType = pScene->GetObjType();

			// �I�u�W�F�N�g�̎�ނ�Enemy�������珈��
			if (objType == CScene::OBJTYPE_ENEMY)
			{
				// �G�C���X�^���X���̎擾
				CEnemy *pEnemy = (CEnemy*)pScene;

				// �ʒu�̎擾
				D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

				// �T�C�Y�̎擾
				D3DXVECTOR2 sizeEnemy = D3DXVECTOR2(pEnemy->GetSize().x / 2.0f, pEnemy->GetSize().y / 2.0f);

				// ��`�̓����蔻��
				if (pos.x + size.x >= posEnemy.x - sizeEnemy.x && pos.x - size.x <= posEnemy.x + sizeEnemy.x &&
					pos.y - size.y <= posEnemy.y + sizeEnemy.y && pos.y + size.y >= posEnemy.y - sizeEnemy.y)
				{
					// �G�̔j��
					pEnemy->HitEnemy();

					return true;
				}
			}
		}
		// ���̏���
		pScene = pSceneNext;
	}
	return false;
}

//=============================================================================
// �^�C���Ƃ̓����蔻�菈��
//=============================================================================
void CExplosion::CollosionTile(void)
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

			switch (pTile->GetTileType())
			{
			case CTile::TILETYPE_BLACK:
				if (m_explosion.explosionType != EXPLOSIONTYPE_PLAYER)
				{
					// �^�C���Ƃ̓����蔻��
					pTile->FillTileTypeBlack(pos, size);
				}
				break;

			case CTile::TILETYPE_WHITE:
				if (m_explosion.explosionType == EXPLOSIONTYPE_PLAYER)
				{
					pTile->FillTileTypeWhite(pos, size);
				}
				break;

			case CTile::TILETYPE_CHANGE_BLACK:
				if (m_explosion.explosionType != EXPLOSIONTYPE_PLAYER)
				{
					// �^�C���Ƃ̓����蔻��
					pTile->FillTileTypeChangeBlack(pos, size);
				}
				break;
			}
		}
		// ���̏���
		pScene = pSceneNext;
	}
}