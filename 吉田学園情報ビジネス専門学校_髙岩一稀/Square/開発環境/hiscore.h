//=============================================================================
//
// ハイスコア処理 [hiscore.h]
// Author : istuki takaiwa
//
//=============================================================================
#ifndef _HISCORE_H_
#define _HISCORE_H_

#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DIGIT	(8)		// 桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;			// ナンバークラス

//-----------------------------------------------------------------------------
// ハイスコアクラス(派生クラス)
//-----------------------------------------------------------------------------
class CHiScore : public CScore
{
public:
	CHiScore(int nPriority);
	~CHiScore();

	static CHiScore *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,float fSpace, SCOREPLACEMENT scorePlacement);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fSpace, SCOREPLACEMENT scorePlacement);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetScore(void) { return m_nHiScore; }

private:
	void LoadHiScore(void);				// ハイスコアのロード

	int m_nHiScore;						// スコア
};
#endif