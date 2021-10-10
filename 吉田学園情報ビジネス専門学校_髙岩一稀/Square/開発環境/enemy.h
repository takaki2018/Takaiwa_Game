//=============================================================================
//
// 敵処理 [enemy.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMYTYPE	(1)			// 敵の種類
#define BULLET_MOVE		(7.0f)		// 弾の移動量
#define BULLET_SIZE		(25.0f)		// 弾のサイズ

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;			// プレイヤークラス

//*****************************************************************************
// 敵クラス(派生クラス)
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
	// 敵の種類
	typedef enum
	{
		ENEMYTYPE_NORMAL = 0,	// 通常
		ENEMYTYPE_SHOOT,		// 弾を撃つ敵
		ENEMYTYPE_NAX,
	}EnemyType;

	CEnemy(int nPriority);
	~CEnemy();

	static HRESULT Load(void);
	static void Unload(void);
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,float fMoveAI,EnemyType enemyType);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, EnemyType enemyType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 CollitionTile(D3DXVECTOR3 pos, D3DXVECTOR2 size);	// タイルとのあたり判定
	bool CollisionBullet(void);										// 弾との当たり判定
	bool CollisionPlayer(CPlayer *pPlayer);							// プレイヤーとの当たり判定
	void HitEnemy(void);
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMYTYPE_NAX];			// テクスチャ情報のポインタ
	D3DXVECTOR3 m_posOld;											// 前回の位置
	float m_fMoveAI;												// 基本移動量
	D3DXVECTOR3 m_move;												// 移動量
	EnemyType m_enemyType;											// 敵の種類
};
#endif