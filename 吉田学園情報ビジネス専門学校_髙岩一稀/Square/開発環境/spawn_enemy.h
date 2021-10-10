//=============================================================================
//
// 敵の発生処理 [spawn_enemy.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _SPAWNENEMY_H_
#define _SPAWNENEMY_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FHASE		(10)		// 最大フェーズ数
#define MAX_ENEMYSPAWN	(50)		// 1フェーズの登場する敵の最大数

//*****************************************************************************
// 敵発生クラス(派生クラス)
//***************************************************************************** ----------------------------------------------------------
class CSpawnEnemy : public CScene
{
public:
	CSpawnEnemy(int nPriority = 0);
	~CSpawnEnemy();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static int GetRemainingEnemy(void) { return m_nCntRemainingEnemy; }
	static void SetRemainingEnemy(int nCntRemainingEnemy) { m_nCntRemainingEnemy = nCntRemainingEnemy; }

private:
	bool LoadSpawnEnemyData(void);	// 敵配置情報読み込み

	// 敵発生情報をまとめた構造体
	typedef struct
	{
		int nNumTile;			// 何番目のタイルか
		D3DXVECTOR2 size;		// サイズ情報
		float fMoveAI;			// 基本移動量
		int nEnemyType;			// 発生させる敵の種類
		int nCntSpawnInterval;	// 敵を発生させるまでの時間
		int nCntInterval;		// 次の敵を発生させるまでの時間
		bool bGroup;			// 集団(まとめて)でスポーンするかどうか
		int nCntGroupEnemy;		// 何体同時に出現させるか
		bool bUse;				// 使用しているかどうか
	}SpawnEnemy_Info;

	static char SpawnEnemyText[MAX_FHASE][128];						// 敵発生情報テキストのテキスト名格納用
	SpawnEnemy_Info m_aSpawnEnemy_info[MAX_FHASE][MAX_ENEMYSPAWN];	// 敵発生情報
	int m_nCounter;													// カウンター
	int m_nNumFhase;												// 何フェーズ目かカウント
	int m_nCntEnemy;												// 何番目の敵かカウント
	static int m_nCntRemainingEnemy;								// 残り敵数
	int m_nNumEnemy[MAX_FHASE];										// 1フェーズの敵の数
};
#endif