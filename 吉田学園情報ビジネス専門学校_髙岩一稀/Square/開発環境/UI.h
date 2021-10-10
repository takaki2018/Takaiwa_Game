//=============================================================================
//
// UI処理 [UI.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "manager.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScore;				// スコアクラス
class CHiScore;				// ハイスコアクラス
class CLife;				// ライフクラス
class CUIBg;				// UI背景クラス
class CCountDown;			// カウントダウンクラス

//*****************************************************************************
// UIクラス(基本クラス)
//*****************************************************************************
class CUI
{
public:

	CUI();
	~CUI();

	HRESULT Init(CManager::MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *GetScore(void) { return m_pScore; }
	static CHiScore *GetHiScore(void) { return m_pHiScore; }
	static CLife *GetLife(void) { return m_pLife; }
	static CUIBg *GetLogoBg(void) { return m_pLogoBg; }
	static CCountDown *GetCountDown(void) { return m_pCountDown; }

private:
	static CScore *m_pScore;					// スコア情報のポインタ
	static CHiScore *m_pHiScore;				// ハイスコア情報のポインタ
	static CLife *m_pLife;						// ライフ情報のポインタ
	static CUIBg *m_pLogoBg;					// ロゴ背景情報
	static CCountDown *m_pCountDown;			// カウントダウン情報のポインタ
};
#endif