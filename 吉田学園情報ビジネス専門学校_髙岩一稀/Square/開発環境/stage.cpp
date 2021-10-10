//=============================================================================
//
// ステージ (stage.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "stage.h"
#include <stdio.h>
#include <string.h>

#include "tile.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
char CStage::m_aStageData[STAGETYPE_MAX][128] =				// ステージデータが入ったテキスト名格納用
{															
	"data/STAGEDATA/stage_box.txt",							
	"data/STAGEDATA/stage2.txt"								
};															
char CStage::m_aStageInfo[128] = {};						// ステージ配置情報のパス保存用
int CStage::m_aMapData[MAX_WDITH][MAX_HEIGHT] = {};			// マップデータ保存用
int CStage::m_nCntX = 0;									// 横幅のブロック数
int CStage::m_nCntY = 0;									// 縦幅のブロック数
D3DXVECTOR3 CStage::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// タイルの初期位置
int CStage::m_nNumPlayerSpawn = 0;							// プレイヤーのスポーン位置
bool CStage::m_bRevival[MAX_TILE] = {};						// 復活中かどうか
int CStage::m_nNumTile = 0;									// タイルの総数
CStage::TileEdge CStage::m_tileEdge = {};					// タイルの端番号の保存
CStage::NumTileEdge CStage::m_numTileEdge = {};				// タイルの端総数の保存用

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

//=============================================================================
// ステージ情報の読み込み
//=============================================================================
bool CStage::LoadStageData(STAGETYPE stageType)
{
	// メンバ変数の初期化
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

//=============================================================================
// ステージ情報の読み込み
//=============================================================================
bool CStage::SetStageDetale(void)
{
	// 変数宣言
	int nCntX = 0, nCntY = 0;
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
				m_aMapData[nCntY][nCntX] = nNum;
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
				if (m_aMapData[nCntY][nCntX] != 0)
				{
					// タイルの生成
					CTile::Create(D3DXVECTOR3(m_pos.x + fSize * nCntX, m_pos.y + fSize * nCntY, 0.0f),
						D3DXVECTOR2(fSize, fSize),
						(CTile::TILETYPE)m_aMapData[nCntY][nCntX],
						nCnt);

					// 端情報の保存
					SetEdgeInfo(nCntX, nCntY,nCnt);

					nCnt++;
				}
			}
		}

		// タイルの総数を保存
		m_nNumTile = nCnt;
	}
	else
	{
		return false;
	}

	return true;
}

//=============================================================================
// ステージ端情報の保存
//=============================================================================
void CStage::SetEdgeInfo(int nCntX, int nCntY, int nCnttile)
{
	// 左右の端
	if (nCntX - 1 >= 0 && nCntX < m_nCntX)
	{
		if (m_aMapData[nCntY][nCntX] != m_aMapData[nCntY][nCntX - 1] && (m_aMapData[nCntY][nCntX - 1] == 3))
		{
			// 左端
			m_tileEdge.nTileEdgeLeft[m_numTileEdge.nNumTileEdgeLeft] = nCnttile;
			m_numTileEdge.nNumTileEdgeLeft++;
		}
		else if ((m_aMapData[nCntY][nCntX] != m_aMapData[nCntY][nCntX + 1]) && (m_aMapData[nCntY][nCntX + 1] == 3))
		{
			// 右端
			m_tileEdge.nTileEdgeRight[m_numTileEdge.nNumTileEdgeRight] = nCnttile;
			m_numTileEdge.nNumTileEdgeRight++;
		}
	}

	// 上下の端
	if (nCntY - 1 >= 0 && nCntY < m_nCntY)
	{
		if ((m_aMapData[nCntY][nCntX] != m_aMapData[nCntY - 1][nCntX]) && (m_aMapData[nCntY - 1][nCntX] == 3))
		{
			// 上端
			m_tileEdge.nTileEdgeTop[m_numTileEdge.nNumTileEdgeTop] = nCnttile;
			m_numTileEdge.nNumTileEdgeTop++;
		}
		else if ((m_aMapData[nCntY][nCntX] != m_aMapData[nCntY + 1][nCntX]) && (m_aMapData[nCntY + 1][nCntX] == 3))
		{
			// 下端
			m_tileEdge.nTileEdgeBottom[m_numTileEdge.nNumTileEdgeBottom] = nCnttile;
			m_numTileEdge.nNumTileEdgeBottom++;
		}
	}
}
 