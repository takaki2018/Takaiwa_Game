//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "game.h"

#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "tile.h"
#include "effect.h"
#include "stage.h"
#include "input_keyboard.h"
#include "manager.h"
#include "fade.h"
#include "UI.h"
#include "pause.h"
#include "spawn_enemyUI.h"
#include "spawn_enemy.h"
#include "game_clear.h"
#include "UI_bg.h"
#include "life_ui.h"
#include "sound.h"
#include "model.h"
#include "tile_revival.h"
#include "tile_frame_effect.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer *CGame::m_pPlayer = NULL;				// �v���C���[���̃|�C���^
CUI *CGame::m_pUI = NULL;						// UI���̃|�C���^
CSpawnEnemy *CGame::m_pSpawnEnemy = NULL;		// �G�������̃|�C���^
CGameClear *CGame::m_pGameClear = NULL;			// �Q�[���N���A���̃|�C���^
CModel *CGame::m_pModel = NULL;					// ���f�����̃|�C���^
int CGame::m_nCntTileFrameEffect = 0;			// �^�C���t���[���G�t�F�N�g���o���J�E���^�[

//=============================================================================
// CGame�̃R���X�g���N�^
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// CManager�̃f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �v���C���[�̃e�N�X�`�����[�h
	CPlayer::Load("data/TEXTURE/player.png");

	// �e�̃e�N�X�`�����[�h
	CBullet::Load();

	// �����̃e�N�X�`�����[�h
	CExplosion::Load();

	// �G�̃e�N�X�`�����[�h
	CEnemy::Load();

	// �G�t�F�N�g�̃e�N�X�`�����[�h
	CEffect::Load();

	// �|�[�Y�̃e�N�X�`�����[�h
	CPause::Load();

	// �G����UI�̃e�N�X�`�����[�h
	CSpawnEnemyUI::Load();

	// �^�C�������G�t�F�N�g�̃e�N�X�`�����[�h
	CTileRevival::Load();

	// �^�C���t���[���G�t�F�N�g�̃e�N�X�`�����[�h
	CTileFrameEffect::Load();

	// �w�i�̐���
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
		CBg::TEXTURETYPE_GAME);

	// �^�C���̐���
	CStage::LoadStageData(CStage::STAGETYPE_BOX);

	// UI�̏���������
	m_pUI = new CUI;

	if (m_pUI != NULL)
	{
		m_pUI->Init(CManager::MODE_GAME);
	}

	// �G�����̏���������
	m_pSpawnEnemy = new CSpawnEnemy(CScene::OBJTYPE_ENEMY);

	if (m_pSpawnEnemy != NULL)
	{
		m_pSpawnEnemy->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	// �^�C�������擾
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, CStage::GetNumPlayerSpawn());

	// �ʒu���擾
	CTile *pTile = (CTile*)pScene;

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create(pTile->GetPosition(), D3DXVECTOR2(35.0f, 35.0f));
	
	// �|�[�Y�̐���
	CPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// ���f���̐���
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f, -2.0f, 5.0f), D3DXVECTOR2(0.0f, 0.0f), "data/MODEL/block_black.x");

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	// �T�E���h��~
	CManager::GetSound()->Stop();

	// �v���C���[�̃e�N�X�`���A�����[�h
	CPlayer::Unload();

	// �e�̃e�N�X�`���A�����[�h
	CBullet::Unload();

	// �����̃e�N�X�`���A�����h
	CExplosion::Unload();

	// �G�̃e�N�X�`���A�����[�h
	CEnemy::Unload();

	// �G�t�F�N�g�̃e�N�X�`���A�����[�h
	CEffect::Unload();

	// �|�[�Y�̃e�N�X�`���A�����[�h
	CPause::Unload();

	// �G����UI�̃e�N�X�`���A�����[�h
	CSpawnEnemyUI::Unload();

	// �^�C�������G�t�F�N�g�̃e�N�X�`���A�����[�h
	CTileRevival::Unload();

	// �^�C���t���[���G�t�F�N�g�̃e�N�X�`���A�����[�h
	CTileFrameEffect::Unload();

	// UI�̏I������
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		m_pUI = NULL;
	}

	// m_pGameClear�̏I������
	if (m_pGameClear != NULL)
	{
		m_pGameClear->Uninit();
		m_pGameClear = NULL;
	}

	// �폜
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{
	// �G��|���������擾
	int nCntRemainingEnemy = CSpawnEnemy::GetRemainingEnemy();

	// ���C�t�̎擾
	int nLife = CUI::GetLife()->GetLife();

	if (nCntRemainingEnemy == 0 || nLife == 0)
	{
		if (m_pGameClear != NULL)
		{
			// �J�ڂ��Ă悢��
			bool bTransition = CGameClear::GetTransition();

			// ��ʑJ��(Enter�L�[���������G��S�ł�������)
			if (bTransition == true)
			{
				CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
			}
		}
		else if (m_pGameClear == NULL)
		{
			// UI�w�i���̏�����
			CUIBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 
				D3DXVECTOR2(SCREEN_WIDTH, 80.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 
				CUIBg::TEXTURETYPE_LOGO);

			if (nCntRemainingEnemy == 0)
			{
				// �Q�[���N���A�C���X�^���X�̐���
				m_pGameClear = CGameClear::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, 80.0f),CGameClear::GAMETRANSITION_CLEAR);
			}
			else if (nLife == 0)
			{
				// �Q�[���N���A�C���X�^���X�̐���
				m_pGameClear = CGameClear::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, 80.0f), CGameClear::GAMETRANSITION_OVER);
			}
		}
	}

	// �^�C���t���[���G�t�F�N�g�̐���
	TileFrameEffectCreate();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// �^�C���t���[���G�t�F�N�g�̐�������
//=============================================================================
void CGame::TileFrameEffectCreate(void)
{
	// �J�E���^�[�X�V
	m_nCntTileFrameEffect++;

	// �^�C���t���[���G�t�F�N�g�̐���
	if (m_nCntTileFrameEffect % 30 == 0)
	{
		// �[�̔ԍ��̎擾
		CStage::TileEdge tileEdge = CStage::GetTileEdge();
		CStage::NumTileEdge nNumTileEdge = CStage::GetNumTileEdge();
		int nNumTile;

		// �^�C�����̎擾
		CTile *pTile;

		D3DXVECTOR3 posTile;
		D3DXVECTOR2 sizeTile;

		int nCnt = rand() % 4;

		// 2�����ŕ�����
		switch (nCnt)
		{
		case 0:
			nNumTile = rand() % nNumTileEdge.nNumTileEdgeLeft;
			pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, tileEdge.nTileEdgeLeft[nNumTile]);
			// NULL�`�F�b�N
			if (pTile != NULL)
			{
				// �^�C���������ȊO�̂Ƃ�����
				if (pTile->GetTileType() != CTile::TILETYPE_INVISIBLE)
				{
					// �ʒu�ƃT�C�Y�̎擾
					posTile = pTile->GetPosition();
					sizeTile = pTile->GetSize();

					// �^�C���t���[���G�t�F�N�g�̐���
					CTileFrameEffect::Create(D3DXVECTOR3(posTile.x + sizeTile.x / 2.0f, posTile.y - sizeTile.y / 2.0f, posTile.z),
						D3DXVECTOR2(20.0f, 5.0f),
						D3DXVECTOR3(25.0f, 0.0f, 0.0f));
				}
			}
			break;

		case 1:
			nNumTile = rand() % nNumTileEdge.nNumTileEdgeRight;
			pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, tileEdge.nTileEdgeRight[nNumTile]);
			// NULL�`�F�b�N
			if (pTile != NULL)
			{
				// �^�C���������ȊO�̂Ƃ�����
				if (pTile->GetTileType() != CTile::TILETYPE_INVISIBLE)
				{
					// �ʒu�ƃT�C�Y�̎擾
					posTile = pTile->GetPosition();
					sizeTile = pTile->GetSize();

					// �^�C���t���[���G�t�F�N�g�̐���
					CTileFrameEffect::Create(D3DXVECTOR3(posTile.x - sizeTile.x / 2.0f, posTile.y - sizeTile.y / 2.0f, posTile.z),
						D3DXVECTOR2(20.0f, 5.0f),
						D3DXVECTOR3(-25.0f, 0.0f, 0.0f));
				}
			}
			break;

		case 2:
			nNumTile = rand() % nNumTileEdge.nNumTileEdgeTop;
			pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, tileEdge.nTileEdgeTop[nNumTile]);
			// NULL�`�F�b�N
			if (pTile != NULL)
			{
				// �^�C���������ȊO�̂Ƃ�����
				if (pTile->GetTileType() != CTile::TILETYPE_INVISIBLE)
				{
					// �ʒu�ƃT�C�Y�̎擾
					posTile = pTile->GetPosition();
					sizeTile = pTile->GetSize();

					// �^�C���t���[���G�t�F�N�g�̐���
					CTileFrameEffect::Create(D3DXVECTOR3(posTile.x - sizeTile.x / 2.0f, posTile.y + sizeTile.y / 2.0f, posTile.z),
						D3DXVECTOR2(5.0f, 20.0f),
						D3DXVECTOR3(0.0f, 25.0f, 0.0f));
				}
			}
			break;
		case 3:
			nNumTile = rand() % nNumTileEdge.nNumTileEdgeBottom;
			pTile = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, tileEdge.nTileEdgeBottom[nNumTile]);
			// NULL�`�F�b�N
			if (pTile != NULL)
			{
				// �^�C���������ȊO�̂Ƃ�����
				if (pTile->GetTileType() != CTile::TILETYPE_INVISIBLE)
				{
					// �ʒu�ƃT�C�Y�̎擾
					posTile = pTile->GetPosition();
					sizeTile = pTile->GetSize();

					// �^�C���t���[���G�t�F�N�g�̐���
					CTileFrameEffect::Create(D3DXVECTOR3(posTile.x + sizeTile.x / 2.0f, posTile.y - sizeTile.y / 2.0f, posTile.z),
						D3DXVECTOR2(5.0f, 20.0f),
						D3DXVECTOR3(0.0f, -25.0f, 0.0f));
				}
			}
			break;
		}
	}
}

//=============================================================================
// �v���C���[���̎擾
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// �X�R�A���̎擾
//=============================================================================
CUI *CGame::GetUI(void)
{
	return m_pUI;
}