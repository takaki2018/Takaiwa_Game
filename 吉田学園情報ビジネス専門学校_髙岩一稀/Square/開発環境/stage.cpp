//=============================================================================
//
// �X�e�[�W (stage.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "stage.h"
#include <stdio.h>
#include <string.h>

#include "tile.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
char CStage::m_aStageData[STAGETYPE_MAX][128] =				// �X�e�[�W�f�[�^���������e�L�X�g���i�[�p
{															
	"data/STAGEDATA/stage_box.txt",							
	"data/STAGEDATA/stage2.txt"								
};															
char CStage::m_aStageInfo[128] = {};						// �X�e�[�W�z�u���̃p�X�ۑ��p
int CStage::m_aMapData[MAX_WDITH][MAX_HEIGHT] = {};			// �}�b�v�f�[�^�ۑ��p
int CStage::m_nCntX = 0;									// �����̃u���b�N��
int CStage::m_nCntY = 0;									// �c���̃u���b�N��
D3DXVECTOR3 CStage::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �^�C���̏����ʒu
int CStage::m_nNumPlayerSpawn = 0;							// �v���C���[�̃X�|�[���ʒu
bool CStage::m_bRevival[MAX_TILE] = {};						// ���������ǂ���
int CStage::m_nNumTile = 0;									// �^�C���̑���
CStage::TileEdge CStage::m_tileEdge = {};					// �^�C���̒[�ԍ��̕ۑ�
CStage::NumTileEdge CStage::m_numTileEdge = {};				// �^�C���̒[�����̕ۑ��p

//=============================================================================
// CStage�̃R���X�g���N�^
//=============================================================================
CStage::CStage()
{

}

//=============================================================================
// CStage�̃f�X�g���N�^
//=============================================================================
CStage::~CStage()
{

}

//=============================================================================
// �X�e�[�W���̓ǂݍ���
//=============================================================================
bool CStage::LoadStageData(STAGETYPE stageType)
{
	// �����o�ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_TILE; nCnt++)
	{
		m_bRevival[nCnt] = false;
	}
	m_nNumTile = 0;
	for (int nCnt = 0; nCnt < MAX_EDGE; nCnt++)
	{
		m_tileEdge.nTileEdgeTop[nCnt] = 0;
		m_tileEdge.nTileEdgeBottom[nCnt] = 0;
		m_tileEdge.nTileEdgeRight[nCnt] = 0;
		m_tileEdge.nTileEdgeLeft[nCnt] = 0;
	}
	m_numTileEdge.nNumTileEdgeTop = 0;
	m_numTileEdge.nNumTileEdgeBottom = 0;
	m_numTileEdge.nNumTileEdgeRight = 0;
	m_numTileEdge.nNumTileEdgeLeft = 0;
	for (int nCntY = 0; nCntY < MAX_HEIGHT; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_WDITH; nCntX++)
		{
			m_aMapData[nCntX][nCntY] = 0;
		}
	}

	// �e�L�X�g�����擾
	char *StageInfo = &m_aStageData[stageType][0];

	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(&m_aStageData[stageType][0], "r");

	// �ϐ��錾
	char aLine[64] = {};
	char aText[128];
	bool bSetDetail = false;

	if (pFile != NULL)
	{
		// �ꕶ�����ׂ�
		while (fgets(aLine, 64, pFile) != NULL)
		{
			fscanf(pFile, "%s", &aText[0]);

			if (strcmp(&aText[0], "STAGEDATA") == 0)
			{
				fscanf(pFile, "%s %s", &aText[0], &m_aStageInfo[0]);
			}

			if (strcmp(&aText[0], "SETDETAIL") == 0)
			{
				bSetDetail = true;
			}
			if (bSetDetail == true)
			{
				if (strcmp(&aText[0], "TILENUM") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nNumPlayerSpawn);
				}
				if (strcmp(&aText[0], "WIDTH") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nCntX);
				}
				if (strcmp(&aText[0], "HEIGHT") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nCntY);
				}
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0], &m_pos.x,&m_pos.y,&m_pos.z);
				}
				if (strcmp(&aText[0], "END_SETDETAIL") == 0)
				{
					break;
				}
			}
		}
		fclose(pFile);

		// �X�e�[�W�̏ڍׂ�ݒ肷��
		if (!SetStageDetale())
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

//=============================================================================
// �X�e�[�W���̓ǂݍ���
//=============================================================================
bool CStage::SetStageDetale(void)
{
	// �ϐ��錾
	int nCntX = 0, nCntY = 0;
	char aLine[128] = {};
	float fSize = TILE_SIZE;

	// FILE�^�̃|�C���^�֐�
	FILE *pFileStage;

	// �t�@�C�����J��
	pFileStage = fopen(&m_aStageInfo[0], "r");

	if (pFileStage != NULL)
	{
		// �ꕶ�����ׂ�
		while (fgets(aLine, 128, pFileStage) != NULL)
		{
			nCntX = 0;
			char *pTokun = strtok(aLine, ",");

			while (pTokun != NULL)
			{
				int nNum = atoi(pTokun);
				m_aMapData[nCntY][nCntX] = nNum;
				pTokun = strtok(NULL, ",");
				nCntX++;
			}
			nCntY++;
		}
		fclose(pFileStage);

		int nCnt = 0;

		// �X�e�[�W�̐���
		for (int nCntY = 0; nCntY < m_nCntY; nCntY++)
		{
			for (int nCntX = 0; nCntX < m_nCntX; nCntX++)
			{
				if (m_aMapData[nCntY][nCntX] != 0)
				{
					// �^�C���̐���
					CTile::Create(D3DXVECTOR3(m_pos.x + fSize * nCntX, m_pos.y + fSize * nCntY, 0.0f),
						D3DXVECTOR2(fSize, fSize),
						(CTile::TILETYPE)m_aMapData[nCntY][nCntX],
						nCnt);

					// �[���̕ۑ�
					SetEdgeInfo(nCntX, nCntY,nCnt);

					nCnt++;
				}
			}
		}

		// �^�C���̑�����ۑ�
		m_nNumTile = nCnt;
	}
	else
	{
		return false;
	}

	return true;
}

//=============================================================================
// �X�e�[�W�[���̕ۑ�
//=============================================================================
void CStage::SetEdgeInfo(int nCntX, int nCntY, int nCnttile)
{
	// ���E�̒[
	if (nCntX - 1 >= 0 && nCntX < m_nCntX)
	{
		if (m_aMapData[nCntY][nCntX] != m_aMapData[nCntY][nCntX - 1] && (m_aMapData[nCntY][nCntX - 1] == 3))
		{
			// ���[
			m_tileEdge.nTileEdgeLeft[m_numTileEdge.nNumTileEdgeLeft] = nCnttile;
			m_numTileEdge.nNumTileEdgeLeft++;
		}
		else if ((m_aMapData[nCntY][nCntX] != m_aMapData[nCntY][nCntX + 1]) && (m_aMapData[nCntY][nCntX + 1] == 3))
		{
			// �E�[
			m_tileEdge.nTileEdgeRight[m_numTileEdge.nNumTileEdgeRight] = nCnttile;
			m_numTileEdge.nNumTileEdgeRight++;
		}
	}

	// �㉺�̒[
	if (nCntY - 1 >= 0 && nCntY < m_nCntY)
	{
		if ((m_aMapData[nCntY][nCntX] != m_aMapData[nCntY - 1][nCntX]) && (m_aMapData[nCntY - 1][nCntX] == 3))
		{
			// ��[
			m_tileEdge.nTileEdgeTop[m_numTileEdge.nNumTileEdgeTop] = nCnttile;
			m_numTileEdge.nNumTileEdgeTop++;
		}
		else if ((m_aMapData[nCntY][nCntX] != m_aMapData[nCntY + 1][nCntX]) && (m_aMapData[nCntY + 1][nCntX] == 3))
		{
			// ���[
			m_tileEdge.nTileEdgeBottom[m_numTileEdge.nNumTileEdgeBottom] = nCnttile;
			m_numTileEdge.nNumTileEdgeBottom++;
		}
	}
}
 