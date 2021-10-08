//=============================================================================
//
// ゲームロゴ処理 [game_logo.h]
// Author : 
//
//=============================================================================
#ifndef _GAMELOGO_H_
#define _GAMELOGO_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLogo;			// ロゴクラス

//-----------------------------------------------------------------------------
// ゲームロゴクラス(派生クラス)
//-----------------------------------------------------------------------------
class CGameLogo : public CScene
{
public:
	// ロゴの種類の種類
	typedef enum
	{
		LOGOTYPE_SCORE = 0,		// スコアロゴ
		LOGOTYPE_HIGHSCORE,		// ハイスコアロゴ
		LOGOTYPE_PRESSENTER,	// PressEnter
		LOGOTYPE_MAX,
	}LOGOTYPE;

	CGameLogo(int nPriority);
	~CGameLogo();
	static CGameLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static bool GetTransition(void) { return bTransition; }

private:
	CLogo *m_apLogo[LOGOTYPE_MAX];		// ロゴクラスのポインタ
	static bool bTransition;			// 遷移するかどうか
};
#endif