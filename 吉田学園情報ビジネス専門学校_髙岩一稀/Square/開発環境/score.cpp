//=============================================================================
//
// スコア処理 [score.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "score.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// CScoreのコンストラクタ
//=============================================================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
	// 変数の初期化
	for (int nCntNumber = 0; nCntNumber < MAX_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber] = NULL;
	}
	m_nScore = 0;
	m_nDigit = 1;
}

//=============================================================================
// CScoreのデストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fSpace, SCOREPLACEMENT scorePlacement)
{
	// ポインタ変数の生成
	CScore *pScore;
	pScore = new CScore(CScene::PRIORITY_UI);

	// NULLチェック
	if (pScore != NULL)
	{
		// 初期化処理
		pScore->Init(pos, size);

		// オブジェクトタイプの設定
		pScore->SetObjType(OBJTYPE_SCORE);

		// スコア配置の種類を保存
		pScore->m_scorePlacement = scorePlacement;

		// スコア配置間隔を保存
		pScore->m_fSpace = fSpace;
	}

	return pScore;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{	
	// CScene2Dの初期化処理
	m_apNumber[0] = CNumber::Create(D3DXVECTOR3(pos.x,pos.y,0.0f), size);
	m_nScore = 0;
	m_nDigit = 1;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// CScene2Dの終了処理
		m_apNumber[nCntNumber]->Uninit();
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// CScene2Dの終了処理
		m_apNumber[nCntNumber]->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// CScene2Dの描画処理
		m_apNumber[nCntNumber]->Draw();
	}
}

//=============================================================================
// スコアの加算
//=============================================================================
void CScore::AddScore(int nValue)
{
	// スコアを保存
	m_nScore += nValue;
	
	// スコアの描画
	Placementtype();
}

//=============================================================================
// スコアの設定
//=============================================================================
void CScore::SetScore(int nScore)
{
	// スコアを保存
	m_nScore = nScore;

	// スコアの描画
	Placementtype();
}

//=============================================================================
// スコア配置の種類ごとに処理
//=============================================================================
void CScore::Placementtype(void)
{
	// スコア配置の種類ごとに処理
	switch (m_scorePlacement)
	{
		// スコアの右寄せ配置
	case SCOREPLACEMENT_RIGHT:
		RightJustified();
		break;
		// スコアの左寄せ配置
	case SCOREPLACEMENT_LEFT:
		LeftJustified();
		break;
	}
}

//=============================================================================
// スコアの右寄せ配置
//=============================================================================
void CScore::RightJustified(void)
{
	// 桁数を初期化
	m_nDigit = 1;

	// 桁数を計算
	for (int nCntDigit = 1; nCntDigit < MAX_DIGIT; nCntDigit++)
	{
		int nNumber = (m_nScore / (int)powf(10.0f, (float)nCntDigit));

		if (nNumber <= 0)
		{
			// 0以下の時終わる
			break;
		}
		else
		{
			// 桁数の増加
			m_nDigit++;

			if (m_apNumber[m_nDigit - 1] == NULL)
			{
				// 情報がNULLなら生成
				D3DXVECTOR3 pos = m_apNumber[nCntDigit - 1]->GetPosition();
				D3DXVECTOR2 size = m_apNumber[nCntDigit - 1]->GetSize();
				m_apNumber[m_nDigit - 1] = CNumber::Create(D3DXVECTOR3(pos.x - m_fSpace, pos.y, pos.z), size);
			}
		}
	}

	// スコアを格納
	for (int nCntNumber = m_nDigit - 1; nCntNumber > 1; nCntNumber--)
	{
		// 桁を一つずつ格納
		int nNumber = (m_nScore / (int)powf(10.0f, (float)((nCntNumber))));

		// ナンバーセット
		m_apNumber[nCntNumber]->SetNumber(nNumber);
	}
}

//=============================================================================
// スコアの左寄せ配置
//=============================================================================
void CScore::LeftJustified(void)
{
	// 桁数を初期化
	m_nDigit = 1;

	// 桁数を計算
	for (int nCntDigit = 1; nCntDigit < MAX_DIGIT; nCntDigit++)
	{
		// 数字の保存
		int nNumber = (m_nScore / (int)powf(10.0f, (float)nCntDigit));

		if (nNumber <= 0)
		{
			// 0以下の時終わる
			break;
		}
		else
		{
			// 桁数の増加
			m_nDigit++;

			if (m_apNumber[m_nDigit - 1] == NULL)
			{
				// 情報がNULLなら生成
				D3DXVECTOR3 pos = m_apNumber[nCntDigit - 1]->GetPosition();
				D3DXVECTOR2 size = m_apNumber[nCntDigit - 1]->GetSize();
				m_apNumber[m_nDigit - 1] = CNumber::Create(D3DXVECTOR3(pos.x + m_fSpace, pos.y, 0.0f), size);
			}
		}
	}

	// スコアを格納
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// 桁を一つずつ格納
		int nNumber = (m_nScore / (int)powf(10.0f, (float)(m_nDigit - (nCntNumber + 1))));

		// ナンバーセット
		m_apNumber[nCntNumber]->SetNumber(nNumber);
	}
}