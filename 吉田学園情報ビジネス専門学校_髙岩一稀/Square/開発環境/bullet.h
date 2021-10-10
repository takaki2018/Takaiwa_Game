//=============================================================================
//
// 弾処理 [bullet.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _BUllET_H_
#define _BUllET_H_

#include "scene2D.h"

//*****************************************************************************
// 弾クラス(派生クラス)
//*****************************************************************************
class CBullet : public CScene2D
{
public:
	CBullet(int nPriority);
	~CBullet();

	// 弾の種類
	typedef enum
	{
		BULETTTYPE_NONE = 0,
		BULLETTYPE_PLEYER,
		BULLETTYPE_ENEMY,
		BULLETTYPE_MAX,
	}BULLETTYPE;

	// 弾が発射する向き
	typedef enum
	{
		BULLETDIRECTION_UP = 0,
		BULLETDIRECTION_DOWN,
		BULLETDIRECTION_RIGHT,
		BULLETDIRECTION_LEFT,
		BULLETDIRECTION_MAX,
	}BULLETDIRECTION;

	static HRESULT Load(void);
	static void Unload(void);
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, BULLETTYPE type,BULLETDIRECTION direction);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, BULLETTYPE type, BULLETDIRECTION direction);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetOldPosition(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	BULLETTYPE GetBulletType(void) { return m_bulletType; }

private:
	void Move(void);						// 移動処理
	void CollisionTile(void);				// タイルとのあたり判定

	D3DXVECTOR3 m_move;						// 移動量
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ情報のポインタ
	int m_nLife;							// 弾の寿命
	BULLETTYPE m_bulletType;				// 弾の種類
	D3DXVECTOR3 m_posOld;					// 前回の位置
};
#endif