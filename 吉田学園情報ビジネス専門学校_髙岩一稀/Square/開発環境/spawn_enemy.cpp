//=============================================================================
//
// �G�������� [spawn_enemy.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "spawn_enemy.h"
#include "spawn_enemyUI.h"
#include "enemy.h"
#include "scene.h"
#include "tile.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CSpawnEnemy::m_nCntRemainingEnemy = 0;
char CSpawnEnemy::SpawnEnemyText[MAX_FHASE][128] = 
{"data/TEXTFILE/SpawnEnemy_Info/Fhase_1.txt",
"data/TEXTFILE/SpawnEnemy_Info/Fhase_2.txt" ,
"data/TEXTFILE/SpawnEnemy_Info/Fhase_3.txt" ,
"data/TEXTFILE/SpawnEnemy_Info/Fhase_4.txt" };

//=============================================================================
// CSpawnEnemy�̃R���X�g���N�^
//=============================================================================
CSpawnEnemy::CSpawnEnemy(int nPriority)
{
	// �����o�ϐ��̏�����
	m_nCntRemainingEnemy = 0;
	m_nCntEnemy = 0;
	m_nCounter = 0;
	m_nNumFhase = 0;
	for (int nCnt = 0; nCnt < MAX_FHASE; nCnt++)
	{
		m_nNumEnemy[nCnt] = 0;
	}
}

//=============================================================================
// CSpawnEnemy�̃f�X�g���N�^
//=============================================================================
CSpawnEnemy::~CSpawnEnemy()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSpawnEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �g�p��Ԃ�false�ɂ���
	for (int nCntFhase = 0; nCntFhase < MAX_FHASE; nCntFhase++)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMYSPAWN; nCntEnemy++)
		{
			m_aSpawnEnemy_info[nCntFhase][nCntEnemy].bUse = false;
		}
	}
	// �����o�ϐ��̏�����
	m_nCntRemainingEnemy = 0;
	m_nCntEnemy = 0;
	m_nCounter = 0;
	m_nNumFhase = 0;
	for (int nCnt = 0; nCnt < MAX_FHASE; nCnt++)
	{
		m_nNumEnemy[nCnt] = 0;
	}
	// �G�������̃��[�h
	if (!LoadSpawnEnemyData())
	{
		return false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSpawnEnemy::Uninit(void)
{
	// �폜
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSpawnEnemy::Update(void)
{
	// �J�E���^�[�̍X�V
	m_nCounter++;

	// �C���^�[�o�����ƂɓG�𐶐�
	if (m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].bUse != false)
	{
		if (m_nCounter % m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nCntInterval == 0)
		{

			if (m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].bGroup == true)
			{

				int nNumEnemy = m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nCntGroupEnemy;
				int nCnt = m_nCntEnemy;

				for (int nCntEnemy = m_nCntEnemy; nCntEnemy < nCnt + nNumEnemy; nCntEnemy++)
				{
					// �^�C�������擾
					CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nNumTile);

					// NULL�`�F�b�N
					if (pScene != NULL)
					{
						// �ʒu���擾
						CTile *pTile = (CTile*)pScene;
						D3DXVECTOR3 pos = pTile->GetPosition();

						// �G����UI�z�u
						CSpawnEnemyUI::Create(pos,
							m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].size,
							m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].fMoveAI,
							m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nCntSpawnInterval,
							m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nEnemyType);

						// �G�ԍ��̍X�V
						m_nCntEnemy++;

						// �J�E���^�[�̏�����
						m_nCounter = 0;
					}
				}
			}
			else
			{
				// �^�C�������擾
				CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nNumTile);

				// NULL�`�F�b�N
				if (pScene != NULL)
				{
					// �ʒu���擾
					CTile *pTile = (CTile*)pScene;
					D3DXVECTOR3 pos = pTile->GetPosition();

					// �G����UI�z�u
					CSpawnEnemyUI::Create(pos,
						m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].size,
						m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].fMoveAI,
						m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nCntSpawnInterval,
						m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nEnemyType);

					// �G�ԍ��̍X�V
					m_nCntEnemy++;

					// �J�E���^�[�̏�����
					m_nCounter = 0;
				}
			}
		}
	}
	// ���������G�̐����t�F�[�Y�̑����ƈꏏ�ɂȂ�����t�F�[�Y�̍X�V
	if (m_nCntEnemy == m_nNumEnemy[m_nNumFhase])
	{
		m_nNumFhase++;

		// �G�̑�����������
		m_nCntEnemy = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSpawnEnemy::Draw(void)
{

}

//=============================================================================
// �G�������̓ǂݍ���
//=============================================================================
bool CSpawnEnemy::LoadSpawnEnemyData(void)
{
	// �ϐ��錾
	char aLine[256] = {};				// �s
	char aText[128];					// ��s�̍ő啶����
	bool bSetFhase = false;				// �t�F�[�Y����邩�ǂ���
	bool bSetSpawnEnemy_Info = false;	// �G�����̏��������邩�ǂ���
	int nCntFhase = 0;					// �t�F�[�Y�J�E���g
	int nCntEnemy = 0;					// �G�J�E���g
	bool bSetGroup = false;

	while (SpawnEnemyText[nCntFhase] != NULL)
	{
		// FILE�^�̃|�C���^�֐�
		FILE *pFile;

		// �t�@�C�����J��
		pFile = fopen(&SpawnEnemyText[nCntFhase][0], "r");

		if (pFile != NULL)
		{
			// �ꕶ�����ׂ�
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// �ꕶ���r�ݍ���
				fscanf(pFile, "%s", &aText[0]);

				//SETFHASE����������t�F�[�Y�쐬���邱�Ƃ�`����
				if (strcmp(&aText[0], "SETFHASE") == 0)
				{
					bSetFhase = true;
				}
				if (bSetFhase == true)
				{
					//SETGROUP������������擾���邱�Ƃ�`����
					if (strcmp(&aText[0], "SETGROUP") == 0)
					{
						m_aSpawnEnemy_info[nCntFhase][nCntEnemy].bGroup = true;
						bSetGroup = true;
					}
					if (bSetGroup == true &&
						strcmp(&aText[0], "SETGROUPENEMYNUM") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &m_aSpawnEnemy_info[nCntFhase][nCntEnemy].nCntGroupEnemy);
						bSetGroup = false;
					}
					//SETENEMY������������擾���邱�Ƃ�`����
					if (strcmp(&aText[0], "SETENEMY") == 0)
					{
						bSetSpawnEnemy_Info = true;
					}
					if (bSetSpawnEnemy_Info == true)
					{
						// TILENUM���������牽�Ԗڂ̃^�C�����𔲂���邩�ۑ�����
						if (strcmp(&aText[0], "TILENUM") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aSpawnEnemy_info[nCntFhase][nCntEnemy].nNumTile);
						}
						// SIZE����������T�C�Y����ۑ�����
						if (strcmp(&aText[0], "SIZE") == 0)
						{
							fscanf(pFile, "%s %f %f", &aText[0],
								&m_aSpawnEnemy_info[nCntFhase][nCntEnemy].size.x,
								&m_aSpawnEnemy_info[nCntFhase][nCntEnemy].size.y);
						}
						// MOVE����������ړ��ʂ�ۑ�����
						if (strcmp(&aText[0], "MOVE") == 0)
						{
							fscanf(pFile, "%s %f", &aText[0],&m_aSpawnEnemy_info[nCntFhase][nCntEnemy].fMoveAI);
						}
						// ENEMYTYPE����������G�̎�ނ�ۑ�����
						if (strcmp(&aText[0], "ENEMYTYPE") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aSpawnEnemy_info[nCntFhase][nCntEnemy].nEnemyType);
						}
						// SPAWNINTERVAL����������G�𔭐�������t���[������ۑ�����
						if (strcmp(&aText[0], "SPAWNINTERVAL") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aSpawnEnemy_info[nCntFhase][nCntEnemy].nCntSpawnInterval);
						}
						// INTERVAL����������O�̓G����UI���牽�t���[����ɓG����UI���o�������邩��ۑ�����
						if (strcmp(&aText[0], "INTERVAL") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aSpawnEnemy_info[nCntFhase][nCntEnemy].nCntInterval);
						}
						// END_SETENEMY����������G�̏��擾���I���邱�Ƃ�`����  
						if (strcmp(&aText[0], "END_SETENEMY") == 0)
						{
							// �g�p��Ԃ�true�ɂ���
							m_aSpawnEnemy_info[nCntFhase][nCntEnemy].bUse = true;

							// �G�ԍ��̍X�V
							nCntEnemy++;

							// ���擾���I�������Ƃ�`����
							bSetSpawnEnemy_Info = false;
						}
					}
					//END_SETENEMY����������t�F�[�Y�쐬���I����
					if (strcmp(&aText[0], "END_SETFHASE") == 0)
					{
						// �G�̐���ۑ�
						m_nNumEnemy[nCntFhase] = nCntEnemy;

						// ���������G�̍��v�l���v�Z
						m_nCntRemainingEnemy += nCntEnemy;

						// �t�F�[�Y���X�V
						nCntFhase++;

						// �G�ԍ��̏�����
						nCntEnemy = 0;

						bSetFhase = false;
					}
				}
			}
			// �t�@�C�������
			fclose(pFile);
		}
		else
		{
			return false;
		}
	}
	return true;
}