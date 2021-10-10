//=============================================================================
//
// �^�C������ [tile.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "tile.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "AI.h"
#include "bullet.h"
#include "enemy.h"
#include "setparticle.h"
#include "tile_revival.h"
#include "stage.h"
#include "count_down.h"
#include "game.h"
#include "UI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAAX_CORNER		(4)				// �p�̐�

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTile::m_pTexture = NULL;		// �e�N�X�`�����̃|�C���^

//=============================================================================
// CTile�̃R���X�g���N�^
//=============================================================================
CTile::CTile(int nPriority) : CScene2D(nPriority)
{
	// �����o�ϐ��̏�����
	m_tileType = TILETYPE_NONE;
	m_pTileRevival = NULL;
	m_nNum = -1;
	m_nCntRevival = 0;
}

//=============================================================================
// CTile�̃f�X�g���N�^
//=============================================================================
CTile::~CTile()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CTile::Load(void)
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
void CTile::Unload(void)
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
CTile *CTile::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,TILETYPE type, int nNum)
{
	// �|�C���^�ϐ��̐���
	CTile *pTile;
	pTile = new CTile(CScene::PRIORITY_TILE);

	// NULL�`�F�b�N
	if (pTile != NULL)
	{
		// ����������
		pTile->Init(pos, size,type,nNum);

		// �e�N�X�`���ݒ�
		pTile->BindTexture(m_pTexture);
	}

	return pTile;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTile::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size,TILETYPE type, int nNum)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_TILE);

	// ��ނɂ���ĐF��ς���
	FillTile(type);

	// ���Ԗڂ̃^�C�����ۑ�
	m_nNum = nNum;

	// �C���^�[�o���̏�����
	m_nCntRevival = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTile::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTile::Update(void)
{
	// �J�E���g�_�E�����̎擾
	CCountDown *pCountDown = CGame::GetUI()->GetCountDown();

	// �J�E���g�_�E����0�Ȃ�^�C���𕜊�������
	if (pCountDown->GetCountDown() == 1 && m_tileType == TILETYPE_CHANGE_BLACK)
	{
		if (CStage::GetRevival(m_nNum) != true)
		{
			// �ʒu�̎擾
			D3DXVECTOR3 pos = GetPosition();

			// �^�C�������G�t�F�N�g�̐���
			CTileRevival::Create(pos, D3DXVECTOR2(1.0f, 1.0f), CTileRevival::TILETYPE_BLACK, m_nNum);

			// �������Ă����Ԃɂ���
			CStage::SetRevival(true, m_nNum);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTile::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// ���������̂������^�C���̏ꍇ
//=============================================================================
void CTile::FillTileTypeBlack(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �ʒu�̎擾
	D3DXVECTOR3 posTile = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// ���^�C���Ƃ̓����蔻��
	if (pos.x + size.x >= posTile.x - sizeTile.x && pos.x - size.x <= posTile.x + sizeTile.x &&
		pos.y - size.y <= posTile.y + sizeTile.y && pos.y + size.y >= posTile.y - sizeTile.y)
	{
		// �^�C���𔒐F�ɂ���
		FillTile(TILETYPE_WHITE);
	}
}

//=============================================================================
// ���������̂������^�C���̏ꍇ
//=============================================================================
void CTile::FillTileTypeWhite(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �ʒu�̎擾
	D3DXVECTOR3 posTile = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// ���^�C���Ƃ̓����蔻��
	if (pos.x + size.x >= posTile.x - sizeTile.x && pos.x - size.x <= posTile.x + sizeTile.x &&
		pos.y - size.y <= posTile.y + sizeTile.y && pos.y + size.y >= posTile.y - sizeTile.y)
	{
		if (m_nCntRevival < 0)
		{
			// �^�C�������ɕς��Ƃ�
			FillTile(TILETYPE_CHANGE_BLACK);

			// �^�C�������G�t�F�N�g�̐���
			CTileRevival::Create(posTile, D3DXVECTOR2(1.0f, 1.0f), CTileRevival::TILETYPE_BLACK, m_nNum);

			// �������Ă����Ԃɂ���
			CStage::SetRevival(true, m_nNum);

			// �v���C���[�����邩�ǂ���
			CollisionPlayer();
		}
		else
		{
			m_nCntRevival--;
		}
	}
}

//=============================================================================
// ���������̂����ɕς��^�C���̏ꍇ
//=============================================================================
void CTile::FillTileTypeChangeBlack(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �ʒu�̎擾
	D3DXVECTOR3 posTile = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// ���^�C���Ƃ̓����蔻��
	if (pos.x + size.x >= posTile.x - sizeTile.x && pos.x - size.x <= posTile.x + sizeTile.x &&
		pos.y - size.y <= posTile.y + sizeTile.y && pos.y + size.y >= posTile.y - sizeTile.y)
	{
		// �^�C���𔒐F�ɂ���
		FillTile(TILETYPE_WHITE);

		// �������ĂȂ���Ԃɂ���
		CStage::SetRevival(false, m_nNum);

		m_nCntRevival = 15;
	}
}

//=============================================================================
// ���������̂������̃^�C���̏ꍇ
//=============================================================================
void CTile::EffectReflection(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR2 size)
{
	// �ʒu�̎擾
	D3DXVECTOR3 posTile = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �^�C���̓����蔻��
	if (pPos->x > posTile.x - sizeTile.x && pPos->x < posTile.x + sizeTile.x)
	{
		if (pPos->y > posTile.y - sizeTile.y && pPos->y < posTile.y + sizeTile.y + size.y)
		{
			pMove->y *= -1.0f;
			pPos->y = posTile.y + sizeTile.y + size.y;
		}
		else if (pPos->y > posTile.y - sizeTile.y - size.y && pPos->y < posTile.y + sizeTile.y)
		{
			pMove->y *= -1.0f;
			pPos->y = posTile.y - sizeTile.y - size.y;
		}
	}
	if (pPos->y > posTile.y - sizeTile.y && pPos->y < posTile.y + sizeTile.y)
	{
		if (pPos->x > posTile.x - sizeTile.x && pPos->x < posTile.x + sizeTile.x + size.x)
		{
			pMove->x *= -1.0f;
			pPos->x = posTile.x + sizeTile.x + size.x;
		}
		else if (pPos->x > posTile.x - sizeTile.x - size.x && pPos->x < posTile.x + sizeTile.x)
		{
			pMove->x *= -1.0f;
			pPos->x = posTile.x - sizeTile.x - size.x;
		}
	}
}

//=============================================================================
// ���������̂������̃^�C���̏ꍇ
//=============================================================================
bool CTile::CollisionObject(D3DXVECTOR3 *pos, D3DXVECTOR3 posOld,D3DXVECTOR3 *move ,D3DXVECTOR2 size)
{
	// �ϐ��錾
	bool bHitTile = false;

	// �ʒu�̎擾
	D3DXVECTOR3 posTile = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �^�C���̓����蔻��
	if (pos->x > posTile.x - sizeTile.x && pos->x < posTile.x + sizeTile.x)
	{
		if (pos->y > posTile.y - sizeTile.y && pos->y < posTile.y + sizeTile.y + size.y)
		{
			move->y = 0.0f;
			pos->y = posTile.y + sizeTile.y + size.y;
			bHitTile = true;
		}
		else if (pos->y > posTile.y - sizeTile.y - size.y && pos->y < posTile.y + sizeTile.y)
		{
			move->y = 0.0f;
			pos->y = posTile.y - sizeTile.y - size.y;
			bHitTile = true;
		}
	}
	if (pos->y > posTile.y - sizeTile.y && pos->y < posTile.y + sizeTile.y)
	{
		if (pos->x > posTile.x - sizeTile.x && pos->x < posTile.x + sizeTile.x + size.x)
		{
			move->x = 0.0f;
			pos->x = posTile.x + sizeTile.x + size.x;
			bHitTile = true;
		}
		else if(pos->x > posTile.x - sizeTile.x - size.x && pos->x < posTile.x + sizeTile.x)
		{
			move->x = 0.0f;
			pos->x = posTile.x - sizeTile.x - size.x;
			bHitTile = true;
		}
	}

	// �^�C���̎l�p�̈ʒu
	D3DXVECTOR3 aPos[MAAX_CORNER] = { D3DXVECTOR3(posTile.x + sizeTile.x,posTile.y + sizeTile.y,0.0f) ,
		D3DXVECTOR3(posTile.x + sizeTile.x,posTile.y - sizeTile.y,0.0f) ,
		D3DXVECTOR3(posTile.x - sizeTile.x,posTile.y + sizeTile.y,0.0f), 
		D3DXVECTOR3(posTile.x - sizeTile.x,posTile.y - sizeTile.y,0.0f) };

	for (int nCnt = 0; nCnt < MAAX_CORNER; nCnt++)
	{
		// ���S�ƃ^�C���̊p�Ƃ̋����𑪂�
		D3DXVECTOR3 vec = *pos - aPos[nCnt];
		float fLength = sqrtf(vec.x * vec.x + vec.y * vec.y);

		if (fLength < size.x)
		{
			// �~�̔��a����r�I�u�W�F�N�g�̃T�C�Y��菬�����ꍇ
			float fRate = 1.0f - (fLength / size.x);

			float x = vec.x * fRate;
			float y = vec.y * fRate;

			pos->x += x;
			pos->y += y;
		}
	}

	return bHitTile;
}

//=============================================================================
// �e���^�C���ɓ��������Ƃ�
//=============================================================================
void CTile::FillTileforBullet(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR3 posOld, D3DXCOLOR colEffect, TILETYPE tileType, CBullet *pBullet)
{
	// �ʒu�̎擾
	D3DXVECTOR3 posTile = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �����������ǂ���
	bool bHitTile = false;

	// �^�C���̓����蔻��
	if ((pos.x - size.x) < (posTile.x + sizeTile.x) && (pos.x + size.x) > posTile.x - sizeTile.x)
	{// �I�u�W�F�N�g���^�C���̒��ɂ���Ƃ�
		if (posOld.y + size.y <= posTile.y - sizeTile.y)
		{// �I�u�W�F�N�g���^�C���̏�ɂ���Ƃ�
			if (pos.y + size.y > (posTile.y - sizeTile.y))
			{
				CParticle::SetFillTile(posTile,
					D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					sizeTile,
					colEffect,
					CParticle::BULLETMOVE_UP,
					D3DXVECTOR2(3, 3));

				bHitTile = true;
			}
		}
		else if ((posOld.y - size.y) >= (posTile.y + sizeTile.y))
		{// �v���C���[���u���b�N��posTile
			if ((pos.y - size.y) < (posTile.y + sizeTile.y))
			{
				CParticle::SetFillTile(posTile,
					D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					sizeTile,
					colEffect,
					CParticle::BULLETMOVE_DOWN,
					D3DXVECTOR2(3, 3));

				bHitTile = true;
			}
		}
	}
	if ((pos.y + size.y) > posTile.y - sizeTile.y && (pos.y - size.y) < (posTile.y + sizeTile.y))
	{// �v���C���[���u���b�N��Y���ɂ���Ƃ�
		if ((posOld.x + size.x) <= posTile.x - sizeTile.x)
		{// �v���C���[���u���b�N�̍����ɂ���Ƃ�
			if ((pos.x + size.x) > posTile.x - sizeTile.x)
			{
				CParticle::SetFillTile(posTile,
					D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					sizeTile,
					colEffect,
					CParticle::BULLETMOVE_LEFT,
					D3DXVECTOR2(3, 3));

				bHitTile = true;
			}
		}
		else if ((posOld.x - size.x) >= (posTile.x + sizeTile.x))
		{// �v���C���[���u���b�N�̉E���ɂ���Ƃ�
			if ((pos.x - size.x) < (posTile.x + sizeTile.x))
			{
				CParticle::SetFillTile(posTile,
					D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					sizeTile,
					colEffect,
					CParticle::BULLETMOVE_RIGHT,
					D3DXVECTOR2(3, 3));

				bHitTile = true;
			}
		}
	}
	if (bHitTile == true)
	{
		// �^�C����h��Ԃ�
		FillTile(tileType);
	}
}

//=============================================================================
// �������^�C���ɓ��������Ƃ�
//=============================================================================
void CTile::CircleCollision(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE tileType)
{

}

//=============================================================================
// �v���C���[�Ƃ̂����蔻��
//=============================================================================
void CTile::CollisionPlayer(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 posTile = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �I�u�W�F�N�g���̎擾
	CScene *pScene = CScene::GetScene(PRIORITY_PLAYER);

	// �G�Ƃ̓����蔻��
	while (pScene)
	{
		// ���̏����擾
		CScene *pSceneNext = pScene->GetNext();

		// �I�u�W�F�N�g�̎�ނ̎擾
		CScene::OBJTYPE objType = pScene->GetObjType();

		// �I�u�W�F�N�g�̎�ނ�Enemy�������珈��
		if (objType == CScene::OBJTYPE_PLAYER)
		{
			// �v���C���[�C���X�^���X���̎擾
			CPlayer *pPlayer = (CPlayer*)pScene;

			// �ʒu�ƃT�C�Y���擾
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXVECTOR2 sizePlayer = D3DXVECTOR2(pPlayer->GetSize().x / 2.0f,pPlayer->GetSize().y / 2.0f);

			// �v���C���[�Ƃ̓����蔻��
			if (posPlayer.x + sizePlayer.x >= posTile.x - sizeTile.x && posPlayer.x - sizePlayer.x <= posTile.x + sizeTile.x &&
				posPlayer.y - sizePlayer.y <= posTile.y + sizeTile.y && posPlayer.y + sizePlayer.y >= posTile.y - sizeTile.y)
			{
				if (pPlayer->GetPlayerState() == CPlayer::PLAYERSTATE_NORMAL)
				{
					// �����������Ƃ�`����
					pPlayer->HitPlayer();
				}
				else if (pPlayer->GetPlayerState() == CPlayer::PLAYERSTATE_INVINCIBLE)
				{
					// �^�C���𔒂ɂ���
					FillTile(TILETYPE_WHITE);
				}

				break;
			}
		}
		pScene = pSceneNext;
	}
}

//=============================================================================
// �G�Ƃ̂����蔻��
//=============================================================================
void CTile::CollisionEnemy(CBullet *pBullet)
{
	// �ʒu�̎擾
	D3DXVECTOR3 posTile = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 sizeTile = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �I�u�W�F�N�g���̎擾
	CScene *pScene = CScene::GetScene(PRIORITY_ENEMY);

	// �G�Ƃ̓����蔻��
	while (pScene)
	{
		// ���̏����擾
		CScene *pSceneNext = pScene->GetNext();

		// �I�u�W�F�N�g�̎�ނ̎擾
		CScene::OBJTYPE objType = pScene->GetObjType();

		// �I�u�W�F�N�g�̎�ނ�Enemy�������珈��
		if (objType == CScene::OBJTYPE_ENEMY)
		{
			// �v���C���[�C���X�^���X���̎擾
			CEnemy *pEnemy = (CEnemy*)pScene;

			// �ʒu�ƃT�C�Y���擾
			D3DXVECTOR3 posEnemy = pEnemy->GetPosition();
			D3DXVECTOR2 sizeEnemy = pEnemy->GetSize();

			// �v���C���[�Ƃ̓����蔻��
			if (posEnemy.x + sizeEnemy.x >= posTile.x - sizeTile.x && posEnemy.x - sizeEnemy.x <= posTile.x + sizeTile.x &&
				posEnemy.y - sizeEnemy.y <= posTile.y + sizeTile.y && posEnemy.y + sizeEnemy.y >= posTile.y - sizeTile.y)
			{
				// �����������Ƃ�`����
				pEnemy->HitEnemy();

				// �e������
				pBullet->Uninit();

				break;
			}
		}
		pScene = pSceneNext;
	}
}

//=============================================================================
// �^�C���̓h��Ԃ�����
//=============================================================================
void CTile::FillTile(TILETYPE tileType)
{
	// �^�C���̎�ނɂ���ď��������
	switch (tileType)
	{
		// ��
	case TILETYPE_BLACK:
		// �e�N�X�`�����W�̕ύX
		SetTex(D3DXVECTOR2(0.5f, 1.0f), D3DXVECTOR2(2, 1));
		break;

		// ��
	case TILETYPE_WHITE:
		// �e�N�X�`�����W�̕ύX
		SetTex(D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(2, 1));
		break;

		// ����
	case TILETYPE_INVISIBLE:
		// �F�̕ύX
		SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		break;

		// ���ɕς��Ƃ�
	case TILETYPE_CHANGE_BLACK:
		// �e�N�X�`�����W�̕ύX
		SetTex(D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(2, 1));
		break;

		// ���ɕς��Ƃ�
	case TILETYPE_CHANGE_WHITE:
		// �e�N�X�`�����W�̕ύX
		SetTex(D3DXVECTOR2(0.5f, 1.0f), D3DXVECTOR2(2, 1));
		break;

	}
	// �^�C�v�X�V
	m_tileType = tileType;
}