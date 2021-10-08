//==============================================================================================================
//
// ステージ (stage.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "stage.h"
#include <stdio.h>
#include <string.h>

#include "tile.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_WDITH		(40)
#define MAX_HEIGHT		(30)

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
char CStage::m_aStageData[STAGETYPE_MAX][128] = 
{
	"data/STAGEDATA/stage_box.txt",
	"data/STAGEDATA/stage2.txt"
};
char CStage::m_aStageInfo[128] = {};
int CStage::m_nCntX = 0;
int CStage::m_nCntY = 0;
D3DXVECTOR3 CStage::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CStage::m_nNumPlayerSpawn = 0;
bool CStage::m_bRevival[512] = {};
int CStage::m_nNumTile = 0;
int CStage::m_nNumTileEdgeSide[64] = {};
int CStage::m_nCnttileEdgeSide = 0;
int CStage::m_nNumTileEdgeVertical[64] = {};
int CStage::m_nCnttileEdgeVertical = 0;

//=============================================================================
// CStageのコンストラクタ
//=============================================================================
CStage::CStage()
{

}

//=============================================================================
// CStageのデストラクタ
//=============================================================================
CStage::~CStage()
{

}

//--------------------------------------------------------------------------------------------------------------
// ステージ情報の読み込み
// 引数		：
// 返り値	：
//--------------------------------------------------------------------------------------------------------------
bool CStage::LoadStageData(STAGETYPE stageType)
{
	// メンバ変数の初期化
	for (int nCnt = 0; nCnt < 512; nCnt++)
	{
		m_bRevival[nCnt] = false;
	}
	m_nNumTile = 0;
	for (int nCnt = 0; nCnt < 64; nCnt++)
	{
		m_nNumTileEdgeSide[0] = 0;
		m_nNumTileEdgeVertical[0] = 0;
	}
	m_nCnttileEdgeSide = 0;
	m_nCnttileEdgeVertical = 0;

	// テキスト情報を取得
	char *StageInfo = &m_aStageData[stageType][0];

	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(&m_aStageData[stageType][0], "r");

	// 変数宣言
	char aLine[64] = {};
	char aText[128];
	bool bSetDetail = false;

	if (pFile != NULL)
	{
		// 一文ずつ調べる
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

		// ステージの詳細を設定する
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

//--------------------------------------------------------------------------------------------------------------
// ステージ情報の読み込み
// 引数		：
// 返り値	：
//--------------------------------------------------------------------------------------------------------------
bool CStage::SetStageDetale(void)
{
	int nCntX = 0, nCntY = 0;
	int aMapData[MAX_WDITH][MAX_HEIGHT] = {};
	char aLine[128] = {};
	float fSize = TILE_SIZE;

	// FILE型のポインタ関数
	FILE *pFileStage;

	// ファイルを開く
	pFileStage = fopen(&m_aStageInfo[0], "r");

	if (pFileStage != NULL)
	{
		// 一文ずつ調べる
		while (fgets(aLine, 128, pFileStage) != NULL)
		{
			nCntX = 0;
			char *pTokun = strtok(aLine, ",");

			while (pTokun != NULL)
			{
				int nNum = atoi(pTokun);
				aMapData[nCntY][nCntX] = nNum;
				pTokun = strtok(NULL, ",");
				nCntX++;
			}
			nCntY++;
		}
		fclose(pFileStage);

		int nCnt = 0;

		// ステージの生成
		for (int nCntY = 0; nCntY < m_nCntY; nCntY++)
		{
			for (int nCntX = 0; nCntX < m_nCntX; nCntX++)
			{
				if (aMapData[nCntY][nCntX] != 0)
				{
					CTile::Create(D3DXVECTOR3(m_pos.x + fSize * nCntX, m_pos.y + fSize * nCntY, 0.0f),
						D3DXVECTOR2(fSize, fSize),
						(CTile::TILETYPE)aMapData[nCntY][nCntX],
						nCnt);

					if (nCntX - 1 >= 0 && nCntX < m_nCntX)
					{
						if ((aMapData[nCntY][nCntX] != aMapData[nCntY][nCntX - 1])  && (aMapData[nCntY][nCntX - 1] == 3)|| 
							(aMapData[nCntY][nCntX] != aMapData[nCntY][nCntX + 1]) && (aMapData[nCntY][nCntX + 1] == 3))
						{
							m_nNumTileEdgeSide[m_nCnttileEdgeSide] = nCnt;
							m_nCnttileEdgeSide++;
						}
					}
					if (nCntY - 1 >= 0 && nCntY < m_nCntY)
					{
						if ((aMapData[nCntY][nCntX] != aMapData[nCntY - 1][nCntX]) && (aMapData[nCntY - 1][nCntX] == 3) ||
							(aMapData[nCntY][nCntX] != aMapData[nCntY + 1][nCntX]) && (aMapData[nCntY + 1][nCntX] == 3))
						{
							m_nNumTileEdgeVertical[m_nCnttileEdgeVertical] = nCnt;
							m_nCnttileEdgeVertical++;
						}
					}
					
					nCnt++;
				}
			}
		}
		m_nNumTile = nCnt;
	}
	else
	{
		return false;
	}

	return true;
}
 