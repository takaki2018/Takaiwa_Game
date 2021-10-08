//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : 
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
#include "life.h"
#include "sound.h"
#include "model.h"
#include "tile_revival.h"
#include "tile_frame_effect.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer *CGame::m_pPlayer = NULL;
CUI *CGame::m_pUI = NULL;
CSpawnEnemy *CGame::m_pSpawnEnemy = NULL;
CGameClear *CGame::m_pGameClear = NULL;
CModel *CGame::m_pModel = NULL;
int CGame::m_nCntTileFrameEffect = 0;

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
		// ��
		int *pNumTileEdgeSide = CStage::GetTileEdgeSide();
		int nNumTileSide = CStage::GetCntTileEdgeSide();
		nNumTileSide = rand() % nNumTileSide;

		// �c
		int *pNumTileEdgeVertical = CStage::GetTileEdgeVertical();
		int nNumTileVertical = CStage::GetCntTileEdgeVertical();
		nNumTileVertical = rand() % nNumTileVertical;

		// �^�C�����̎擾
		CTile *pTileSide = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, pNumTileEdgeSide[nNumTileSide]);
		CTile *pTileVertical = (CTile*)CScene::GetScene(CScene::PRIORITY_TILE, pNumTileEdgeSide[nNumTileVertical]);

		// NULL�`�F�b�N
		if (pTileSide != NULL)
		{
			// �^�C���������ȊO�̂Ƃ�����
			if (pTileSide->GetTileType() != CTile::TILETYPE_INVISIBLE)
			{
				D3DXVECTOR3 posTileSide = pTileSide->GetPosition();
				D3DXVECTOR3 sizeTileSide = pTileSide->GetSize();

				int nCnt = rand() % 2;

				// 2�����ŕ�����
				switch (nCnt)
				{
				case 0:
					CTileFrameEffect::Create(D3DXVECTOR3(posTileSide.x + sizeTileSide.x / 2.0f, posTileSide.y - sizeTileSide.y / 2.0f, posTileSide.z),
						D3DXVECTOR2(20.0f, 5.0f),
						D3DXVECTOR3(25.0f, 0.0f, 0.0f));
					break;

				case 1:
					CTileFrameEffect::Create(D3DXVECTOR3(posTileSide.x - sizeTileSide.x / 2.0f, posTileSide.y - sizeTileSide.y / 2.0f, posTileSide.z),
						D3DXVECTOR2(20.0f, 5.0f),
						D3DXVECTOR3(-25.0f, 0.0f, 0.0f));
					break;
				}

			}
		}
		// NULL�`�F�b�N
		if (pTileVertical != NULL)
		{
			// �^�C���������ȊO�̂Ƃ�����
			if (pTileVertical->GetTileType() != CTile::TILETYPE_INVISIBLE)
			{
				D3DXVECTOR3 posTileVertical = pTileVertical->GetPosition();
				D3DXVECTOR3 sizeTileVertical = pTileVertical->GetSize();

				int nCnt = rand() % 2;

				// �l�����ŕ�����
				switch (nCnt)
				{
				case 0:
					CTileFrameEffect::Create(D3DXVECTOR3(posTileVertical.x - sizeTileVertical.x / 2.0f, posTileVertical.y + sizeTileVertical.y / 2.0f, posTileVertical.z),
						D3DXVECTOR2(5.0f, 20.0f),
						D3DXVECTOR3(0.0f, 25.0f, 0.0f));
					break;
				case 1:
					CTileFrameEffect::Create(D3DXVECTOR3(posTileVertical.x + sizeTileVertical.x / 2.0f, posTileVertical.y - sizeTileVertical.y / 2.0f, posTileVertical.z),
						D3DXVECTOR2(5.0f, 20.0f),
						D3DXVECTOR3(0.0f, -25.0f, 0.0f));
					break;
				}
			}
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