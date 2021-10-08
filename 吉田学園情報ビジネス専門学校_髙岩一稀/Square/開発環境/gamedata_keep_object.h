//=============================================================================
//
// ゲームデータ保存オブジェクト処理 [gamedata_keep_object.h]
// Author : 
//
//=============================================================================
#ifndef _GAMEDATAKEEPOBJECT_H_
#define _GAMEDATAKEEPOBJECT_H_

#include "main.h"

//-----------------------------------------------------------------------------
// ゲームデータ保存オブジェクトクラス(基本クラス)
//-----------------------------------------------------------------------------
class CGameDataKeepObject
{
public:
	CGameDataKeepObject();
	~CGameDataKeepObject();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetScore(void) { return m_nKeepScore; }
	static int GetMaxCombo(void) { return m_nKeepMaxCombo; }
	static int GetKillEnemy(void) { return m_nKeepKillEnemy; }
	static int GetTimer(void) { return m_nTimer; }
	static void SetScore(int nScore) { m_nKeepScore = nScore; }
	static void SetMaxCombo(int nMaxCombo) { m_nKeepMaxCombo = nMaxCombo; }
	static void SetKillEnemy(int nKillEnemy) { m_nKeepKillEnemy = nKillEnemy; }
	static void SetTimer(int nTimer) { m_nTimer = nTimer; }

private:
	static int m_nKeepScore;		// スコア保存用
	static int m_nKeepMaxCombo;		// 最大コンボ数保存用
	static int m_nKeepKillEnemy;	// 敵を倒した数保存用
	static int m_nTimer;			// クリアまでにかかった時間保存用
};
#endif