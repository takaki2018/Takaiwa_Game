//=============================================================================
//
// スコア処理 [score.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DIGIT	(8)		// 桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;			// ナンバークラス

//*****************************************************************************
// オブジェクトクラス(基本クラス)
//*****************************************************************************
class CScore : public CScene
{
public:
	// スコア配置の種類
	typedef enum
	{
		SCOREPLACEMENT_NORMAL = 0,	// 通常配置
		SCOREPLACEMENT_RIGHT,		// 右寄せ
		SCOREPLACEMENT_LEFT,		// 左寄せ
		SCOREPLACEMENT_MAX,
	}SCOREPLACEMENT;

	CScore(int nPriority);
	~CScore();

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,float fSpace,SCOREPLACEMENT scorePlacement);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetScore(int nScore);
	void AddScore(int nValue);
	int GetScore(void) { return m_nScore; }	
	void SetScorePlacement(SCOREPLACEMENT scorePlacement) { m_scorePlacement = scorePlacement; } 
	void SetSpace(float fSpace) { m_fSpace = fSpace; }

private:
	void RightJustified(void);			// 右寄せ配置
	void LeftJustified(void);			// 左寄せ配置
	void Placementtype(void);			// スコアの描画更新
	CNumber *m_apNumber[MAX_DIGIT];		// ナンバークラスのポインタ
	float m_fSpace;						// 配置の間隔
	int m_nScore;						// スコア
	int m_nDigit;						// 桁数
	SCOREPLACEMENT m_scorePlacement;	// スコア配置の種類
};
#endif