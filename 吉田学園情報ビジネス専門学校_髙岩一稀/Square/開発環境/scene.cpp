//=============================================================================
//
// シーン処理 [scene.cpp]
// Author : 
//
//=============================================================================
#include "scene.h"
#include "manager.h"
#include "count_down.h"
#include "camera.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CScene *CScene::m_pTop[PRIORITY_MAX] = {};
CScene *CScene::m_pCur[PRIORITY_MAX] = {};

//=============================================================================
// CSceneのデフォルトコンストラクタ
//=============================================================================
CScene::CScene()
{
	// 先頭がNULLじゃないとき
	if (m_pTop[PRIORITY_NONE] == NULL)
	{
		// リストに追加する
		this->m_pNext = NULL;
		this->m_pPrev = NULL;

		// 先頭に情報を保存する
		m_pTop[PRIORITY_NONE] = this;
	}
	else
	{
		// リストに追加する
		m_pCur[PRIORITY_NONE]->m_pNext = this;
		this->m_pPrev = m_pCur[PRIORITY_NONE];
		this->m_pNext = NULL;
	}

	// 最後尾に情報を保存する
	m_pCur[PRIORITY_NONE] = this;

	// 死亡フラグの初期化
	m_bDeath = false;
}

//=============================================================================
// CSceneのコンストラクタ(オーバーロード)
//=============================================================================
CScene::CScene(int nPriority)
{
	// 先頭がNULLじゃないとき
	if (m_pTop[nPriority] == NULL)
	{
		// リストに追加する
		this->m_pNext = NULL;
		this->m_pPrev = NULL;

		// 先頭に情報を保存する
		m_pTop[nPriority] = this;
	}
	else 
	{
		// リストに追加する
		m_pCur[nPriority]->m_pNext = this;
		this->m_pPrev = m_pCur[nPriority];
		this->m_pNext = NULL;
	}
	// 最後尾に情報を保存する
	m_pCur[nPriority] = this;

	// 死亡フラグの初期化
	m_bDeath = false;
}

//=============================================================================
// CSceneのデストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// 終了処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	// 終了処理
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクト情報の取得
		CScene *pScene = m_pTop[nCntPriority];

		// NULLになるまでループ
		while (pScene)
		{
			// 次の情報を取得
			CScene *pSceneNext = pScene->m_pNext;

			// 更新処理
			pScene->Uninit();

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
	// 死亡フラグをチェックしてオブジェクトを破棄
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクト情報の取得
		CScene *pScene = m_pTop[nCntPriority];

		// NULLになるまでループ
		while (pScene)
		{
			// 次の情報を取得
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_bDeath)
			{
				// リストから削除
				if (pScene->m_pPrev != NULL)
				{
					pScene->m_pPrev->m_pNext = pScene->m_pNext;
				}
				if (pScene->m_pNext != NULL)
				{
					pScene->m_pNext->m_pPrev = pScene->m_pPrev;
				}

				if (pScene == m_pCur[nCntPriority])
				{
					m_pCur[nCntPriority] = pScene->m_pPrev;
				}
				if (pScene == m_pTop[nCntPriority])
				{
					m_pTop[nCntPriority] = pScene->m_pNext;
				}

				delete pScene;
				pScene = NULL;
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene::UpdateAll(void)
{
	// モード状態を取得
	CManager::MODE mode = CManager::GetMode();

	switch (mode)
	{
	case CManager::MODE_TITLE:
		UpdateTitle();
		break;

	case CManager::MODE_GAME:
		UpdateGame();
		break;

	case CManager::MODE_RESULT:
		UpdateResult();
		break;
	}

	// 破棄するかどうか
	ReleaseScene();
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene::DrawAll(void)
{
	// カメラの設定
	CManager::GetCamera()->SetCamera();

	// ポーズ情報を取得
	bool bPause = CManager::GetPause();

	// 描画処理
	if (bPause == true)
	{
		for (int nCntPriority = 1; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			// 先頭オブジェクト情報の取得
			CScene *pScene = m_pTop[nCntPriority];

			// NULLになるまでループ
			while (pScene)
			{
				// 次の情報を取得
				CScene *pSceneNext = pScene->m_pNext;

				// 更新処理
				pScene->Draw();

				// 次の情報にする
				pScene = pSceneNext;
			}
		}
	}
	else
	{
		for (int nCntPriority = 1; nCntPriority < PRIORITY_PAUSE; nCntPriority++)
		{
			// 先頭オブジェクト情報の取得
			CScene *pScene = m_pTop[nCntPriority];

			// NULLになるまでループ
			while (pScene)
			{
				// 次の情報を取得
				CScene *pSceneNext = pScene->m_pNext;

				// 更新処理
				pScene->Draw();

				// 次の情報にする
				pScene = pSceneNext;
			}
		}
	}
}

//=============================================================================
// 個別終了処理
//=============================================================================
void CScene::Release(void)
{
	// 死亡フラグを立てる
	this->m_bDeath = true;
}

//=============================================================================
// 何番目のオブジェクトを取得するか
//=============================================================================
CScene *CScene::GetScene(int nPriority, int nCnt)
{
	// 何番目か計算用
	int nCounter = 0;

	// 先頭オブジェクト情報の取得
	CScene *pScene = m_pTop[nPriority];

	// NULLになるまでループ
	while (pScene)
	{
		// 次の情報を取得
		CScene *pSceneNext = pScene->m_pNext;

		// 次の情報にする
		pScene = pSceneNext;

		// 何番目か
		nCounter++;

		// 指定回数まで来たら終える
		if (nCounter == nCnt)
		{
			break;
		}
	}
	return pScene;
}

//=============================================================================
// タイトルの更新処理
//=============================================================================
void CScene::UpdateTitle(void)
{
	// チュートリアル画面を開いているか
	bool bTutorial = CManager::GetTutorial();

	if (bTutorial == true)
	{
		// 先頭オブジェクト情報の取得
		CScene *pScene = m_pTop[PRIORITY_TUTORIAL];

		// NULLになるまでループ
		while (pScene)
		{
			// 次の情報を取得
			CScene *pSceneNext = pScene->m_pNext;

			// 更新処理
			pScene->Update();

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
	else
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_TUTORIAL; nCntPriority++)
		{
			// 先頭オブジェクト情報の取得
			CScene *pScene = m_pTop[nCntPriority];

			// NULLになるまでループ
			while (pScene)
			{
				// 次の情報を取得
				CScene *pSceneNext = pScene->m_pNext;

				if (pScene->m_bDeath == false)
				{
					// 更新処理
					pScene->Update();
				}

				// 次の情報にする
				pScene = pSceneNext;
			}
		}
	}
}

//=============================================================================
// ゲームの更新処理
//=============================================================================
void CScene::UpdateGame(void)
{
	// ポーズ情報の取得
	bool bPause = CManager::GetPause();

	// カウントダウン情報を取得する
	bool bFinishCountDown = CCountDown::GetFinishCountDown();

	if (bFinishCountDown == false)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_PAUSE; nCntPriority++)
		{
			if (nCntPriority == PRIORITY_TILE || nCntPriority == PRIORITY_COUNTDOWN || nCntPriority == PRIORITY_TILEREVIVAL)
			{
				// 先頭オブジェクト情報の取得
				CScene *pScene = m_pTop[nCntPriority];

				// NULLになるまでループ
				while (pScene)
				{
					// 次の情報を取得
					CScene *pSceneNext = pScene->m_pNext;

					// 更新処理
					pScene->Update();

					// 次の情報にする
					pScene = pSceneNext;
				}
			}
		}
	}
	else
	{
		if (bPause == true)
		{
			// 先頭オブジェクト情報の取得
			CScene *pScene = m_pTop[PRIORITY_PAUSE];

			// NULLになるまでループ
			while (pScene)
			{
				// 次の情報を取得
				CScene *pSceneNext = pScene->m_pNext;

				// 更新処理
				pScene->Update();

				// 次の情報にする
				pScene = pSceneNext;
			}
		}
		else
		{
			for (int nCntPriority = 0; nCntPriority < PRIORITY_PAUSE; nCntPriority++)
			{
				// 先頭オブジェクト情報の取得
				CScene *pScene = m_pTop[nCntPriority];

				// NULLになるまでループ
				while (pScene)
				{
					// 次の情報を取得
					CScene *pSceneNext = pScene->m_pNext;

					if (pScene->m_bDeath == false)
					{
						// 更新処理
						pScene->Update();
					}

					// 次の情報にする
					pScene = pSceneNext;
				}
			}
		}
	}
}

//=============================================================================
// リザルトの更新処理
//=============================================================================
void CScene::UpdateResult(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクト情報の取得
		CScene *pScene = m_pTop[nCntPriority];

		// NULLになるまでループ
		while (pScene)
		{
			// 次の情報を取得
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_bDeath == false)
			{
				// 更新処理
				pScene->Update();
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// シーンの破棄処理
//=============================================================================
void CScene::ReleaseScene(void)
{
	// 死亡フラグをチェックしてオブジェクトを破棄
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクト情報の取得
		CScene *pScene = m_pTop[nCntPriority];

		// trueのとき削除
		while (pScene)
		{
			// 次の情報を取得
			CScene *pSceneNext = pScene->m_pNext;

			// 死亡フラグが立っているとき
			if (pScene->m_bDeath)
			{
				// リストから削除
				if (pScene->m_pPrev != NULL)
				{
					pScene->m_pPrev->m_pNext = pScene->m_pNext;
				}
				if (pScene->m_pNext != NULL)
				{
					pScene->m_pNext->m_pPrev = pScene->m_pPrev;
				}

				if (pScene == m_pCur[nCntPriority])
				{
					m_pCur[nCntPriority] = pScene->m_pPrev;
				}
				if (pScene == m_pTop[nCntPriority])
				{
					m_pTop[nCntPriority] = pScene->m_pNext;
				}

				delete pScene;
				pScene = NULL;
			}
			// 次の情報にする
			pScene = pSceneNext;
		}
	}
}