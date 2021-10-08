//=============================================================================
//
// ハイスコア処理 [hiscore.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "hiscore.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// CHiScoreのコンストラクタ
//=============================================================================
CHiScore::CHiScore(int nPriority) : CScore(nPriority)
{
	// 変数の初期化
	m_nHiScore = 0;
}

//=============================================================================
// CHiScoreのデストラクタ
//=============================================================================
CHiScore::~CHiScore()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CHiScore *CHiScore::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fSpace, SCOREPLACEMENT scorePlacement)
{
	// ポインタ変数の生成
	CHiScore *pHiScore;
	pHiScore = new CHiScore(CScene::PRIORITY_UI);

	// NULLチェック
	if (pHiScore != NULL)
	{
		// 初期化処理
		pHiScore->Init(pos, size,fSpace,scorePlacement);

		// オブジェクトタイプの設定
		pHiScore->SetObjType(OBJTYPE_SCORE);
	}

	return pHiScore;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CHiScore::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fSpace, SCOREPLACEMENT scorePlacement)
{
	// スコア配置の種類を保存
	CScore::SetScorePlacement(scorePlacement);

	// スペースの保存
	CScore::SetSpace(fSpace);

	// CScoreの初期化処理
	CScore::Init(pos, size);

	// ハイスコアの読み込み
	LoadHiScore();

	// スコアの設定
	CScore::SetScore(m_nHiScore);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CHiScore::Uninit(void)
{
	// CScoreの終了処理
	CScore::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CHiScore::Update(void)
{
	// CScoreの更新処理
	CScore::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CHiScore::Draw(void)
{  
	// CScoreの描画処理
	CScore::Draw();
}

//=============================================================================
// ハイスコアのロード処理
//=============================================================================
void CHiScore::LoadHiScore(void)
{
	// ローカル変数宣言
	FILE *pFile;

	// データの読み込み
	pFile = fopen("data/TEXTFILE/HiScore.txt", "r");

	// NULLチェック
	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &m_nHiScore);

		fclose(pFile);
	}
	else
	{

	}
}