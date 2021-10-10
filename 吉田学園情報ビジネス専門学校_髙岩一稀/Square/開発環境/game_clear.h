//=============================================================================
//
// ゲームクリア処理 [game_clear.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _GAMECLEAR_H_
#define _GAMECLEAR_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DIGIT	(8)		// 桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLogo;		// ロゴクラス
class CUIBg;		// UI背景クラス

//*****************************************************************************
// ゲームクリアクラス(派生クラス)
//*****************************************************************************
class CGameClear : public CScene
{
public:
	// ゲーム遷移の条件
	typedef enum
	{
		GAMETRANSITION_CLEAR = 0,
		GAMETRANSITION_OVER,
		GAMETRANSITION_MAX,
	}GameTransition;

	CGameClear(int nPriority);
	~CGameClear();

	static CGameClear *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, GameTransition gameTransition);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static bool GetTransition(void) { return m_bTransition; }

private:
	static bool m_bTransition;					// 遷移するかどうか
	static int m_nCntInterval;					// 遷移までの時間
	static GameTransition m_gameTransition;		// 遷移条件
	CLogo *m_pLogo;								// ロゴ情報のポインタ
	CUIBg *m_pUIBg;								// UI背景情報のポインタ
};
#endif