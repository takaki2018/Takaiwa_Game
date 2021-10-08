//=============================================================================
//
// 敵発生処理 [spawn_enemy.cpp]
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
// 静的メンバ変数宣言
//*****************************************************************************
int CSpawnEnemy::m_nCntRemainingEnemy = 0;
char CSpawnEnemy::SpawnEnemyText[MAX_FHASE][128] = 
{"data/TEXTFILE/SpawnEnemy_Info/Fhase_1.txt",
"data/TEXTFILE/SpawnEnemy_Info/Fhase_2.txt" ,
"data/TEXTFILE/SpawnEnemy_Info/Fhase_3.txt" ,
"data/TEXTFILE/SpawnEnemy_Info/Fhase_4.txt" };

//=============================================================================
// CSpawnEnemyのコンストラクタ
//=============================================================================
CSpawnEnemy::CSpawnEnemy(int nPriority)
{
	// メンバ変数の初期化
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
// CSpawnEnemyのデストラクタ
//=============================================================================
CSpawnEnemy::~CSpawnEnemy()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpawnEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 使用状態をfalseにする
	for (int nCntFhase = 0; nCntFhase < MAX_FHASE; nCntFhase++)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMYSPAWN; nCntEnemy++)
		{
			m_aSpawnEnemy_info[nCntFhase][nCntEnemy].bUse = false;
		}
	}
	// メンバ変数の初期化
	m_nCntRemainingEnemy = 0;
	m_nCntEnemy = 0;
	m_nCounter = 0;
	m_nNumFhase = 0;
	for (int nCnt = 0; nCnt < MAX_FHASE; nCnt++)
	{
		m_nNumEnemy[nCnt] = 0;
	}
	// 敵発生情報のロード
	if (!LoadSpawnEnemyData())
	{
		return false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSpawnEnemy::Uninit(void)
{
	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSpawnEnemy::Update(void)
{
	// カウンターの更新
	m_nCounter++;

	// インターバルごとに敵を生成
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
					// タイル情報を取得
					CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nNumTile);

					// NULLチェック
					if (pScene != NULL)
					{
						// 位置情報取得
						CTile *pTile = (CTile*)pScene;
						D3DXVECTOR3 pos = pTile->GetPosition();

						// 敵発生UI配置
						CSpawnEnemyUI::Create(pos,
							m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].size,
							m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].fMoveAI,
							m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nCntSpawnInterval,
							m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nEnemyType);

						// 敵番号の更新
						m_nCntEnemy++;

						// カウンターの初期化
						m_nCounter = 0;
					}
				}
			}
			else
			{
				// タイル情報を取得
				CScene *pScene = CScene::GetScene(CScene::PRIORITY_TILE, m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nNumTile);

				// NULLチェック
				if (pScene != NULL)
				{
					// 位置情報取得
					CTile *pTile = (CTile*)pScene;
					D3DXVECTOR3 pos = pTile->GetPosition();

					// 敵発生UI配置
					CSpawnEnemyUI::Create(pos,
						m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].size,
						m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].fMoveAI,
						m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nCntSpawnInterval,
						m_aSpawnEnemy_info[m_nNumFhase][m_nCntEnemy].nEnemyType);

					// 敵番号の更新
					m_nCntEnemy++;

					// カウンターの初期化
					m_nCounter = 0;
				}
			}
		}
	}
	// 生成した敵の数がフェーズの総数と一緒になったらフェーズの更新
	if (m_nCntEnemy == m_nNumEnemy[m_nNumFhase])
	{
		m_nNumFhase++;

		// 敵の総数も初期化
		m_nCntEnemy = 0;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CSpawnEnemy::Draw(void)
{

}

//=============================================================================
// 敵発生情報の読み込み
//=============================================================================
bool CSpawnEnemy::LoadSpawnEnemyData(void)
{
	// 変数宣言
	char aLine[256] = {};				// 行
	char aText[128];					// 一行の最大文字数
	bool bSetFhase = false;				// フェーズを作るかどうか
	bool bSetSpawnEnemy_Info = false;	// 敵発生の条件があるかどうか
	int nCntFhase = 0;					// フェーズカウント
	int nCntEnemy = 0;					// 敵カウント
	bool bSetGroup = false;

	while (SpawnEnemyText[nCntFhase] != NULL)
	{
		// FILE型のポインタ関数
		FILE *pFile;

		// ファイルを開く
		pFile = fopen(&SpawnEnemyText[nCntFhase][0], "r");

		if (pFile != NULL)
		{
			// 一文ずつ調べる
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// 一文ずつ詠み込む
				fscanf(pFile, "%s", &aText[0]);

				//SETFHASEを見つけたらフェーズ作成することを伝える
				if (strcmp(&aText[0], "SETFHASE") == 0)
				{
					bSetFhase = true;
				}
				if (bSetFhase == true)
				{
					//SETGROUPを見つけたら情報取得することを伝える
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
					//SETENEMYを見つけたら情報取得することを伝える
					if (strcmp(&aText[0], "SETENEMY") == 0)
					{
						bSetSpawnEnemy_Info = true;
					}
					if (bSetSpawnEnemy_Info == true)
					{
						// TILENUMを見つけたら何番目のタイル情報を抜き取るか保存する
						if (strcmp(&aText[0], "TILENUM") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aSpawnEnemy_info[nCntFhase][nCntEnemy].nNumTile);
						}
						// SIZEを見つけたらサイズ情報を保存する
						if (strcmp(&aText[0], "SIZE") == 0)
						{
							fscanf(pFile, "%s %f %f", &aText[0],
								&m_aSpawnEnemy_info[nCntFhase][nCntEnemy].size.x,
								&m_aSpawnEnemy_info[nCntFhase][nCntEnemy].size.y);
						}
						// MOVEを見つけたら移動量を保存する
						if (strcmp(&aText[0], "MOVE") == 0)
						{
							fscanf(pFile, "%s %f", &aText[0],&m_aSpawnEnemy_info[nCntFhase][nCntEnemy].fMoveAI);
						}
						// ENEMYTYPEを見つけたら敵の種類を保存する
						if (strcmp(&aText[0], "ENEMYTYPE") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aSpawnEnemy_info[nCntFhase][nCntEnemy].nEnemyType);
						}
						// SPAWNINTERVALを見つけたら敵を発生させるフレームをを保存する
						if (strcmp(&aText[0], "SPAWNINTERVAL") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aSpawnEnemy_info[nCntFhase][nCntEnemy].nCntSpawnInterval);
						}
						// INTERVALを見つけたら前の敵発生UIから何フレーム後に敵発生UIを出現させるかを保存する
						if (strcmp(&aText[0], "INTERVAL") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aSpawnEnemy_info[nCntFhase][nCntEnemy].nCntInterval);
						}
						// END_SETENEMYを見つけたら敵の情報取得を終えることを伝える  
						if (strcmp(&aText[0], "END_SETENEMY") == 0)
						{
							// 使用状態をtrueにする
							m_aSpawnEnemy_info[nCntFhase][nCntEnemy].bUse = true;

							// 敵番号の更新
							nCntEnemy++;

							// 情報取得を終えたことを伝える
							bSetSpawnEnemy_Info = false;
						}
					}
					//END_SETENEMYを見つけたらフェーズ作成を終える
					if (strcmp(&aText[0], "END_SETFHASE") == 0)
					{
						// 敵の数を保存
						m_nNumEnemy[nCntFhase] = nCntEnemy;

						// 生成した敵の合計値を計算
						m_nCntRemainingEnemy += nCntEnemy;

						// フェーズを更新
						nCntFhase++;

						// 敵番号の初期化
						nCntEnemy = 0;

						bSetFhase = false;
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
		else
		{
			return false;
		}
	}
	return true;
}