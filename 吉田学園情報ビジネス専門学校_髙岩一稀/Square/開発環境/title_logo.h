//=============================================================================
//
// タイトルロゴ処理 [title_logo.h]
// Author : 
//
//=============================================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLogo;			// ロゴクラス
class CUIBg;			// UI背景クラス

//-----------------------------------------------------------------------------
// タイトルロゴクラス(派生クラス)
//-----------------------------------------------------------------------------
class CTitleLogo : public CScene
{
public:
	// ロゴの種類の種類
	typedef enum
	{
		LOGOTYPE_TITLE = 0,		// タイトルロゴ
		LOGOTYPE_PLAY,			// プレイロゴ
		LOGOTYPE_TUTORIAL,		// チュートリアルロゴ
		LOGOTYPE_RANKING,		// ランキングロゴ
		LOGOTYPE_OPTION,		// オプションロゴ
		LOGOTYPE_MAX,
	}LOGOTYPE;

	CTitleLogo(int nPriority);
	~CTitleLogo();
	static CTitleLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static bool GetTransition(void) { return bTransition; }

private:
	CLogo *m_apLogo[LOGOTYPE_MAX];		// ロゴクラスのポインタ
	int m_nSelectLogo;					// 何番目のロゴが選ばれているか
	static bool bTransition;			// 遷移するかどうか
};
#endif