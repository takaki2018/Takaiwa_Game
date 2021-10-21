//=============================================================================
//
// �G���� [enemy.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "AI.h"
#include "bullet.h"
#include "setparticle.h"
#include "tile.h"
#include "explosion.h"
#include "game.h"
#include "UI.h"
#include "score.h"
#include "spawn_enemy.h"
#include "sound.h"
#include "model.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMYTYPE_NAX] = {}; // �e�N�X�`�����̃|�C���^

//=============================================================================
// CEnemy�̃R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority) : CScene2D(nPriority)
{
	// �����o�ϐ��̏�����
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_enemyType = ENEMYTYPE_NORMAL;
}

//=============================================================================
// CEnemy�̃f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy.png", &m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CEnemy::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < MAX_ENEMYTYPE; nCntTexture++)
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
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,float fMoveAI, EnemyType enemyType)
{
	// �|�C���^�ϐ��̐���
	CEnemy *pEnemy;
	pEnemy = new CEnemy(CScene::PRIORITY_ENEMY);

	// NULL�`�F�b�N
	if (pEnemy != NULL)
	{
		// ����������
		pEnemy->Init(pos, size,fMoveAI,enemyType);

		// �e�N�X�`���ݒ�
		pEnemy->BindTexture(m_pTexture[0]);

		// �I�u�W�F�N�g�̎�ސݒ�
		pEnemy->SetObjType(CScene::OBJTYPE_ENEMY);
	}

	return pEnemy;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, EnemyType enemyType)
{
	// �ϐ��̏�����
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_enemyType = enemyType;
	m_fMoveAI = fMoveAI;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �ʒu�̕ۑ�
	SetPosOld(pos);

	// �I�u�W�F�N�g���̎擾
	CScene *pScene= GetScene(CScene::PRIORITY_PLAYER);

	// NULL�`�F�b�N
	while (pScene)
	{
		// ���̏����擾
		CScene *pSceneNext = pScene->GetNext();

		// �I�u�W�F�N�g�̎�ނ̎擾
		CScene::OBJTYPE objType = pScene->GetObjType();

		// �I�u�W�F�N�g�̎�ނ�Enemy�������珈��
		if (objType == CScene::OBJTYPE_PLAYER)
		{
			// �G�C���X�^���X���̎擾
			CPlayer *pPlayer = (CPlayer*)pScene;

			// �ʒu�̎擾
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

			// �ǐ�
			m_move = CAi::Seek(pos, posPlayer,m_fMoveAI);

			// �ړ�
			pos += m_move;
			
			// ���W�̐ݒ�
			SetPosition(pos);

			// �v���C���[�����G��Ԃ̂Ƃ��v���C���[�Ƃ̓����蔻���t�^
			if (pPlayer->GetPlayerState() == CPlayer::PLAYERSTATE_INVINCIBLE)
			{
				CollisionPlayer(pPlayer);
			}
		}
		// ���̏���
		pScene = pSceneNext;
	}
	// �e�Ƃ̓����蔻�菈��
	if (CollisionBullet())
	{
		return;
	}

	// �^�C���Ƃ̂����蔻��
	pos = CollitionTile(pos, size);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �G�j���̑O
//=============================================================================
void CEnemy::HitEnemy(void)
{
	// �����_���[���̎擾
	CRenderer *pRender = CManager::GetRenderer();

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �����̐���
	CExplosion::Create(pos,
		D3DXVECTOR2(30.0f, 30.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		35.0f,
		0.13f,
		5,
		0.0f,
		CExplosion::EXPLOSIONTYPE_ENEMY,
		CExplosion::EXPLOSION_TEXTURETYPE_NORMAL);

	// �Ռ��g�̐���
	CExplosion::Create(pos,
		D3DXVECTOR2(30.0f, 30.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
		10.0f,
		0.0f,
		0,
		0.02f,
		CExplosion::EXPLOSIONTYPE_WAVE,
		CExplosion::EXPLOSION_TEXTURETYPE_WAVE);

	// �����p�[�e�B�N��
	CParticle::SetExplosion(pos,
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		size,
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		0.02f,
		D3DXVECTOR2(5, 5));

	// �X�R�A���̎擾
	CScore *pScore = CGame::GetUI()->GetScore();

	// �X�R�A�̉��Z
	pScore->AddScore(100);

	// �G��|���������X�V
	int nNumRemainingEnemy = CSpawnEnemy::GetRemainingEnemy();
	CSpawnEnemy::SetRemainingEnemy(nNumRemainingEnemy - 1);

	// ���f���̊p�x�ړ��ʂ����Z
	CGame::GetModel()->AddMoveRotate(D3DXVECTOR3(0.01f, 0.0f, 0.01f));

	// ���ʉ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HITENEMY);

	// �t�F�[�h�o�b�N�G�t�F�N�g�̋���
	pRender->SetFadeBackEffect();

	// �G�̔j��
	Uninit();
}

//=============================================================================
// �^�C���Ƃ̓����蔻��
//=============================================================================
D3DXVECTOR3 CEnemy::CollitionTile(D3DXVECTOR3 pos, D3DXVECTOR2 size)
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
			case CTile::TILETYPE_WHITE:
				// �^�C���Ƃ̓����蔻��
				pTile->FillTileTypeWhite(pos, size);
				break;
			}
		}
		// ���̏���
		pScene = pSceneNext;
	}
	
	return pos;
}

//=============================================================================
// �e�Ƃ̓����蔻�菈��
//=============================================================================
bool CEnemy::CollisionBullet(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �I�u�W�F�N�g���̎擾
	CScene *pScene = GetScene(CScene::PRIORITY_BULLET);

	// �G�Ƃ̓����蔻��
	while (pScene)
	{
		// ���̏����擾
		CScene *pSceneNext = pScene->GetNext();

		// �I�u�W�F�N�g�̎�ނ̎擾
		CScene::OBJTYPE objType = pScene->GetObjType();

		// �I�u�W�F�N�g�̎�ނ�Enemy�������珈��
		if (objType == CScene::OBJTYPE_BULLET)
		{
			// �G�C���X�^���X���̎擾
			CBullet *pBullet = (CBullet*)pScene;

			if (pBullet->GetBulletType() == CBullet::BULLETTYPE_PLEYER)
			{
				// �ʒu�̎擾
				D3DXVECTOR3 posBullet = pBullet->GetPosition();

				// �T�C�Y�̎擾
				D3DXVECTOR2 sizeBullet = pBullet->GetSize();

				// ��`�̓����蔻��
				if (pos.x + size.x >= posBullet.x - sizeBullet.x / 2.0f && pos.x - size.x <= posBullet.x + sizeBullet.x / 2.0f &&
					pos.y - size.y <= posBullet.y + sizeBullet.y / 2.0f && pos.y + size.y >= posBullet.y - sizeBullet.y / 2.0f)
				{
					// �e�̔j��
					pBullet->Uninit();

					// �G�̔j��
					HitEnemy();

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
// �v���C���[�Ƃ̓����蔻�菈��
//=============================================================================
bool CEnemy::CollisionPlayer(CPlayer *pPlayer)
{
	// �����������ǂ���
	bool bCollision = false;

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �O��̈ʒu�̎擾
	D3DXVECTOR3 posOld = GetPosOld();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �v���C���[�̈ʒu�擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// �v���C���[�̑O��̈ʒu�̎擾
	D3DXVECTOR3 posOldPlayer = pPlayer->GetPosOld();

	// �v���C���[�̃T�C�Y�̎擾
	D3DXVECTOR2 sizePlayer = D3DXVECTOR2(pPlayer->GetSize().x / 2.0f, pPlayer->GetSize().y / 2.0f);

	if ((pos.x - size.x) < (posPlayer.x + sizePlayer.x) &&
		(pos.x + size.x) > posPlayer.x - sizePlayer.x)
	{// �v���C���[���u���b�N�̒��ɂ���Ƃ�
		if (posOld.y + size.y <= posPlayer.y - sizePlayer.y)
		{// �v���C���[���u���b�N�̏�ɂ���Ƃ�
			if (pos.y + size.y > (posPlayer.y - sizePlayer.y))
			{// �߂荞�񂾂珈��
				pos.y = posPlayer.y - sizePlayer.y - size.y;
				m_move.y = 0.0f;

				bCollision = true;
			}
		}
		else if ((posOld.y - size.y) >= (posPlayer.y + sizePlayer.y))
		{// �v���C���[���u���b�N�̉��ɂ���Ƃ�
			if ((pos.y - size.y) < (posPlayer.y + sizePlayer.y))
			{// �߂荞�񂾂�u���b�N�̉��̍��W�ɂ���
				pos.y = posPlayer.y + sizePlayer.y + size.y;
				m_move.y = 0.0f;

				bCollision = true;
			}
		}
	}
	if ((pos.y + size.y) > posPlayer.y - sizePlayer.y && 
		(pos.y - size.y) < (posPlayer.y + sizePlayer.y))
	{// �v���C���[���u���b�N��Y���ɂ���Ƃ�
		if ((posOld.x + size.x) <= posPlayer.x - sizePlayer.x)
		{// �v���C���[���u���b�N�̍����ɂ���Ƃ�
			if ((pos.x + size.x) > posPlayer.x - sizePlayer.x)
			{// �߂荞�񂾂�u���b�N�̍��̍��W�ɂ���
				pos.x = posPlayer.x - sizePlayer.x - size.x;
				m_move.x = 0.0f;

				bCollision = true;
			}
		}
		else if ((posOld.x - size.x) >= (posPlayer.x + sizePlayer.x))
		{// �v���C���[���u���b�N�̉E���ɂ���Ƃ�
			if ((pos.x - size.x) < (posPlayer.x + sizePlayer.x))
			{// �߂荞�񂾂�u���b�N�̉E�̍��W�ɂ���
				pos.x = posPlayer.x + sizePlayer.x + size.x;
				m_move.x = 0.0f;

				bCollision = true;
			}
		}
	}
	// �ʒu��ݒ�
	SetPosition(pos);

	return bCollision;
}