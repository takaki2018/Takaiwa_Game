//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "player.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "sound.h"
#include "collision.h"
#include "tile.h"
#include "enemy.h"
#include "explosion.h"
#include "life.h"
#include "UI.h"
#include "stage.h"
#include "setparticle.h"
#include "model.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MOVE		(1.6f)		// �ړ���
#define INERTIA			(0.2f)		// ����

#define EXPANTION_COUNT	(50)		// �g��J�E���g
#define SHRINK_COUNT	(100)		// �k���J�E���g
#define MOVE_LENGTH		(0.5f)		// �|���S���T�C�Y�̈ړ���

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;	// �e�N�X�`�����̃|�C���^

//=============================================================================
// CPlayer�̃R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene2D(nPriority)
{
	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntDisp = 0;
	m_nCntState = 0;
	m_playerState = PLAYERSTATE_NORMAL;
}

//=============================================================================
// CPlayer�̃f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CPlayer::Load(const char *TextureName)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, TextureName, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CPlayer::Unload(void)
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
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);

	// NULL�`�F�b�N
	if (pPlayer != NULL)
	{
		// ����������
		pPlayer->Init(pos, size);

		// �I�u�W�F�N�g�̎�ސݒ�
		pPlayer->SetObjType(OBJTYPE_PLAYER);

		// �e�N�X�`���ݒ�
		pPlayer->BindTexture(m_pTexture);
	}

	return pPlayer;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CScene2D�̏���������
	CScene2D::Init(pos,size);

	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntDisp = 0;
	m_nCntState = 0;
	m_playerState = PLAYERSTATE_NORMAL;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// ���C�t�̎擾
	int nLife = CUI::GetLife()->GetLife();

	if (nLife > 0)
	{
		// ��Ԃɂ���ď����𕪂���
		switch (m_playerState)
		{
			// �ʏ�
		case PLAYERSTATE_NORMAL:
			StateNormal();
			break;

			// �q�b�g
		case PLAYERSTATE_HIT:
			StateHit();
			break;

			// ���G
		case PLAYERSTATE_INVINCIBLE:
			StateInvincible();
			break;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer::Move(void)
{
	// CInputKeyboard�C���X�^���X�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// CInputKeyboard�C���X�^���X�̎擾
	CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

	// �Q�[���p�b�h���̎擾
	DIJOYSTATE2 Controller = pInputJoypad->GetControllerPless(PLAYER_1);

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = GetSize();

	// �ړ�����
	if (pInputKeyboard->GetPress(pInputKeyboard->KEYINFO_UP))
	{
		// ��
		m_move.y -= PLAYER_MOVE;
	}
	if (pInputKeyboard->GetPress(pInputKeyboard->KEYINFO_DOWN))
	{
		// ��
		m_move.y += PLAYER_MOVE;
	}
	if (pInputKeyboard->GetPress(pInputKeyboard->KEYINFO_LEFT))
	{
		// ��
		m_move.x -= PLAYER_MOVE;
	}
	if (pInputKeyboard->GetPress(pInputKeyboard->KEYINFO_RIGHT))
	{
		// �E
		m_move.x += PLAYER_MOVE;
	}

	if (Controller.lX != 0 || Controller.lY != 0)
	{// �X�e�B�b�N�̓��͊p�x
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY);

		m_move.x += PLAYER_MOVE * sinf(fAngle);			// X���ړ��ʂ̌���
		m_move.y += PLAYER_MOVE * cosf(fAngle);			// Z���ړ��ʂ̌���
	}

	// ��������
	m_move.x += (0.0f - m_move.x) * INERTIA;
	m_move.y += (0.0f - m_move.y) * INERTIA;
}

//=============================================================================
// �e���ˏ���
//=============================================================================
void CPlayer::Shot(void)
{
	// CInputKeyboard�C���X�^���X�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// CInputKeyboard�C���X�^���X�̎擾
	CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �e�̃T�C�Y
	D3DXVECTOR2 size = D3DXVECTOR2(BULLET_SIZE / 2.0f, BULLET_SIZE);

	// �ϐ��錾	
	bool bShopt = false;														// �e�𔭎˂������ǂ���
	D3DXVECTOR3 moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �e�̈ړ���
	CBullet::BULLETDIRECTION bulletdirection = CBullet::BULLETDIRECTION_UP;		// �e��������

	if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::JPINFO_SHOT_UP) || pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_SHOT_UP))
	{// �����
		moveBullet = D3DXVECTOR3(0.0f, -BULLET_MOVE, 0.0f);
		bulletdirection = CBullet::BULLETDIRECTION_UP;
		bShopt = true;
	}
	else if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::JPINFO_SHOT_DOWN) || pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_SHOT_DOWN))
	{// ������
		moveBullet = D3DXVECTOR3(0.0f, BULLET_MOVE, 0.0f);
		bulletdirection = CBullet::BULLETDIRECTION_DOWN;
		bShopt = true;
	}
	else if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::JPINFO_SHOT_RIGHT) || pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_SHOT_RIGHT))
	{// �E����
		moveBullet = D3DXVECTOR3(BULLET_MOVE, 0.0f, 0.0f);
		bulletdirection = CBullet::BULLETDIRECTION_RIGHT;
		bShopt = true;
	}
	else if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::JPINFO_SHOT_LEFT) || pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_SHOT_LEFT))
	{// ������
		moveBullet = D3DXVECTOR3(-BULLET_MOVE, 0.0f, 0.0f);
		bulletdirection = CBullet::BULLETDIRECTION_LEFT;
		bShopt = true;
	}

	// �e��������Ă�����
	if (bShopt == true)
	{
		// ���f���̊p�x�ړ��ʂ����Z
		//CGame::GetModel()->AddMoveRotate(D3DXVECTOR3(0.0f, 0.05f, 0.0f));

		// �e������
		CBullet::Create(pos,moveBullet, size, CBullet::BULLETTYPE_PLEYER, bulletdirection);
		
		// ���ʉ�
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SHOOT);
	}
}

//=============================================================================
// �^�C���Ƃ̓����蔻��
//=============================================================================
D3DXVECTOR3 CPlayer::CollitionTile(D3DXVECTOR3 pos,D3DXVECTOR2 size)
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
				pTile->CollisionObject(&pos, m_posOld, &m_move, size);
				break;

			case CTile::TILETYPE_BLACK:
				pTile->CollisionObject(&pos, m_posOld, &m_move, size);
				break;

			default:
				break;
			}
		}
		// ���̏���
		pScene = pSceneNext;
	}

	return pos;
}

//=============================================================================
// �G�Ƃ̓����蔻�菈��
//=============================================================================
bool CPlayer::CollitionEnemy(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �I�u�W�F�N�g���̎擾
	CScene *pScene = GetScene(CScene::PRIORITY_ENEMY);

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
			// �G�C���X�^���X���̎擾
			CEnemy *pEnemy = (CEnemy*)pScene;

			// �ʒu�̎擾
			D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

			// �T�C�Y�̎擾
			D3DXVECTOR2 sizeEnemy = pEnemy->GetSize();

			// ��`�̓����蔻��
			if (pos.x + size.x >= posEnemy.x - size.x && pos.x - size.x <= posEnemy.x + size.x &&
				pos.y - size.y <= posEnemy.y + size.y && pos.y + size.y >= posEnemy.y - size.y)
			{
				// ���������Ƃ�����
				if (m_playerState == PLAYERSTATE_NORMAL)
				{
					HitPlayer();
				}

				return true;
			}
		}
		// ���̏���
		pScene = pSceneNext;
	}
	return false;
}

//=============================================================================
// �e�Ƃ̓����蔻�菈��
//=============================================================================
bool CPlayer::CollitionBullet(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);;

	// �I�u�W�F�N�g���̎擾
	CScene *pScene = GetScene(PRIORITY_BULLET);

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

			if (pBullet->GetBulletType() == CBullet::BULLETTYPE_ENEMY)
			{
				// �ʒu�̎擾
				D3DXVECTOR3 posPlayer = pBullet->GetPosition();

				// �T�C�Y�̎擾
				D3DXVECTOR2 sizePlayer = pBullet->GetSize();

				// ��`�̓����蔻��
				if (pos.x + size.x >= posPlayer.x - size.x && pos.x - size.x <= posPlayer.x + size.x &&
					pos.y - size.y <= posPlayer.y + size.y && pos.y + size.y >= posPlayer.y - size.y)
				{
					// �e�̔j��
					pBullet->Uninit();

					if (m_playerState == PLAYERSTATE_NORMAL)
					{
						// ���������Ƃ�����
						HitPlayer();
					}

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
// �v���C���[���I�u�W�F�N�g�ɓ��������Ƃ�
//=============================================================================
void CPlayer::HitPlayer(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// ��Ԃ�ς���
	m_playerState = PLAYERSTATE_HIT;

	// ��ԃJ�E���^�ƕ\���J�E���^�̏�����
	m_nCntState = 0;
	m_nCntDisp = 60;

	// ���ʉ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT_PLAYER);

	// �����̐���
	CExplosion::Create(pos,
		D3DXVECTOR2(30.0f,30.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		35.0f,
		0.13f,
		5,
		0.0f,
		CExplosion::EXPLOSIONTYPE_PLAYER,
		CExplosion::EXPLOSION_TEXTURETYPE_NORMAL);

	// �����p�[�e�B�N��
	CParticle::SetExplosion(pos,
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		size,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.01f,
		D3DXVECTOR2(5, 5));

	// �F�̕ύX
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// ���C�t�̌���
	int nLife = CUI::GetLife()->GetLife();
	CUI::GetLife()->SetLife(nLife - 1);

	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// ���ʏ���
//=============================================================================
void CPlayer::CommonProcess(void)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �T�C�Y�̎擾
	D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

	// �ʒu�̕ۑ�
	SetPosOld(pos);

	// CScene2D�̍X�V����
	CScene2D::Update();

	// �e�̔��ˏ���
	Shot();

	// �ړ�����
	Move();

	// ���W�̐ݒ�
	pos += m_move;

	// �^�C���Ƃ̓����蔻�菈��
	pos = CollitionTile(pos, size);

	// �G�Ƃ̓����蔻��
	CollitionEnemy();

	// �e�Ƃ̓����蔻��
	CollitionBullet();

	// ���W�̐ݒ�
	CScene2D::SetPosition(pos);
}

//=============================================================================
// �ʏ��Ԃ̂Ƃ�
//=============================================================================
void CPlayer::StateNormal(void)
{
	// �ǂ̏�Ԃł����ʂȏ���
	CommonProcess();
}

//=============================================================================
// �I�u�W�F�N�g�ɓ���������Ԃ̂Ƃ�
//=============================================================================
void CPlayer::StateHit(void)
{
	// ��ԃJ�E���^�̍X�V
	m_nCntState++;

	// ��ԃJ�E���^���\���J�E���^�Ɠ����l�ɂȂ������ԍX�V
	if (m_nCntState % m_nCntDisp == 0)
	{
		// �T�C�Y�̎擾
		D3DXVECTOR2 size = D3DXVECTOR2(GetSize().x / 2.0f, GetSize().y / 2.0f);

		// ��Ԃ𖳓G�ɂ���
		m_playerState = PLAYERSTATE_INVINCIBLE;

		// ��ԃJ�E���^�ƕ\���J�E���^��������
		m_nCntState = 0;
		m_nCntDisp = 120;
	}
	else if (m_nCntState % (m_nCntDisp - 1) == 0)
	{
		// �ʒu���X�e�[�W�̒����ɂ���
		// �^�C�������擾
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, CStage::GetNumPlayerSpawn());

		// �ʒu���擾
		CTile *pTile = (CTile*)pScene;

		// �ʒu�̕ύX
		SetPosition(pTile->GetPosition());

		// �����̐���
		CExplosion::Create(pTile->GetPosition(),
			D3DXVECTOR2(100.0f, 100.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.0f),
			40.0f,
			0.1f,
			5,
			0.0f,
			CExplosion::EXPLOSIONTYPE_NONE,
			CExplosion::EXPLOSION_TEXTURETYPE_NORMAL);
	}
}

//=============================================================================
// ���G��Ԃ̂Ƃ�
//=============================================================================
void CPlayer::StateInvincible(void)
{
	// ��ԃJ�E���^�̍X�V
	m_nCntState++;

	// �킩��₷���_�ł�����
	if (m_nCntState % 20 == 10)
	{
		// �������ɂ���
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
	else if (m_nCntState % 20 == 0)
	{
		// �s�����ɂ���
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �ʒu���X�e�[�W�̒����ɂ���
		// �^�C�������擾
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, CStage::GetNumPlayerSpawn());

		// �ʒu���擾
		CTile *pTile = (CTile*)pScene;

		// �Ռ��g�̐���
		CExplosion::Create(pTile->GetPosition() ,
			D3DXVECTOR2(30.0f, 30.0f),
			D3DXCOLOR(0.8f, 0.5f, 0.2f, 0.8f),
			5.0f,
			0.0f,
			0,
			0.01f,
			CExplosion::EXPLOSIONTYPE_WAVE,
			CExplosion::EXPLOSION_TEXTURETYPE_NORMAL);
	}

	// ��ԃJ�E���g�ƕ\���J�E���g�������ɂȂ����Ƃ���Ԃ̕ύX
	if (m_nCntState % m_nCntDisp == 0)
	{
		// ��Ԃ�ʏ�ɂ���
		m_playerState = PLAYERSTATE_NORMAL;

		// �s�����ɂ���
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// �ǂ̏�Ԃł����ʂȏ���
	CommonProcess();
}