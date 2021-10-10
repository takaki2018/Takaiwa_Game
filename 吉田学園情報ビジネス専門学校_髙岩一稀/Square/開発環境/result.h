//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScore;			// スコアクラス

//*****************************************************************************
// リザルト画面クラス(派生クラス)
//*****************************************************************************
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *GetScore(void) { return m_pScore; }
	static CScore *GetMaxCombo(void) { return m_pMaxCombo; }
	static CScore *GetKillEnemy(void) { return m_pKillEnemy; }

private:
	static CScore *m_pScore;		// スコア情報のポインタ
	static CScore *m_pMaxCombo;		// 最大コンボ数情報のポインタ
	static CScore *m_pKillEnemy;	// 敵撃破数情報のポインタ
};
#endif