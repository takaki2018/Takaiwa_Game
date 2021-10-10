//=============================================================================
//
// リザルトロゴ処理 [result_logo.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULTLOGO_H_
#define _RESULTLOGO_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLogo;			// ロゴクラス
class CUIBg;			// UI背景クラス

//*****************************************************************************
// リザルトロゴクラス(派生クラス)
//*****************************************************************************
class CResultLogo : public CScene
{
public:
	// ロゴの種類の種類
	typedef enum
	{
		LOGOTYPE_RESULT = 0,		// リザルトロゴ
		LOGOTYPE_RANK,				// ランクロゴ
		LOGOTYPE_YOURSCORE,			// YourScoreロゴ
		LOGOTYPE_NEWRECORD,			// NewRecordロゴ
		LOGOTYPE_MAXCOMBO,			// 最大コンボ数ロゴ
		LOGOTYPE_KILLENEMY,			// 敵撃破数ロゴ
		LOGOTYPE_MAX,
	}LOGOTYPE;

	CResultLogo(int nPriority);
	~CResultLogo();

	static CResultLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLogo **GetResultLogo(void) { return &m_apLogo[0]; }

private:
	static CLogo *m_apLogo[LOGOTYPE_MAX];		// ロゴクラスのポインタ
};
#endif