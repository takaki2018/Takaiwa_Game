//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LOGOBG		(2)		// ロゴ背景の種類

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUIBg;		// UI背景クラス
class CModel;		// モデルクラス

//*****************************************************************************
// タイトル画面クラス(派生クラス)
//*****************************************************************************
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static bool GetPressEnter(void) { return m_bPressEnter; }
	static CUIBg **GetLogoBg(void) { return &m_apLogoBg[0]; }

private:
	static bool m_bPressEnter;				// エンターを押したかどうか
	static CUIBg *m_apLogoBg[MAX_LOGOBG];	// ロゴ背景情報
};
#endif