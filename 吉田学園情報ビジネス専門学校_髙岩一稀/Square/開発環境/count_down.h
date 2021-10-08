//=============================================================================
//
// カウントダウン処理 [count_down.h]
// Author : 
//
//=============================================================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_NUMBER		(4)			// カウントダウンの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;		// 数字クラス
class CLogo;		// ロゴクラス
class CUIBg;		// 背景クラス

//-----------------------------------------------------------------------------
// ライフクラス(派生クラス)
//-----------------------------------------------------------------------------
class CCountDown : public CScene
{
public:
	CCountDown(int nPriority);
	~CCountDown();
	static HRESULT Load(void);
	static void Unload(void);
	static CCountDown *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static bool GetFinishCountDown(void) { return m_bFinishCountDown; }
	int GetCountDown(void) { return m_nNumCountDown; }

private:
	CNumber *m_apNumber[MAX_NUMBER];		// 数字情報
	CLogo *m_pLogo;							// ロゴ情報
	static bool m_bFinishCountDown;			// カウントダウンが終わったかどうか
	int m_nCntCountDown;					// カウントダウンのカウント
	int m_nNumCountDown;					// 何番目のカウントダウンか
};
#endif