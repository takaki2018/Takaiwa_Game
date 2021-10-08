//=============================================================================
//
// タイル処理 [tile.h]
// Author : 
//
//=============================================================================
#ifndef _TILE_H_
#define _TILE_H_

#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TILE_SIZE		(45.0f)		// タイルの大きさ

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CBullet;		// 弾クラス
class CTileRevival;	// タイル復活クラス

//-----------------------------------------------------------------------------
// タイルクラス(派生クラス)
//-----------------------------------------------------------------------------
class CTile : public CScene2D
{
public:
	CTile(int nPriority);
	~CTile();

	// タイルの種類
	typedef enum
	{
		TILETYPE_NONE = 0,
		TILETYPE_BLACK,			// 黒
		TILETYPE_WHITE,			// 白
		TILETYPE_INVISIBLE,		// 透明(ステージ外部に出ないようにするため使用)
		TILETYPE_CHANGE_BLACK,	// 黒に変わるとき
		TILETYPE_CHANGE_WHITE,	// 白に変わるとき
		TILETYPE_MAX,
	}TILETYPE;

	static HRESULT Load(void);
	static void Unload(void);
	static CTile *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type,int nNum);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type, int nNum);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void FillTileTypeBlack(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void FillTileTypeWhite(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void FillTileTypeChangeBlack(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void CircleCollision(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE tileType);
	void EffectReflection(D3DXVECTOR3 *pPos,D3DXVECTOR3 *pMove, D3DXVECTOR2 size);
	bool CollisionObject(D3DXVECTOR3 *pos, D3DXVECTOR3 posOld, D3DXVECTOR3 *move, D3DXVECTOR2 size);
	void FillTileforBullet(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR3 posOld,D3DXCOLOR colEffect,TILETYPE tileType, CBullet *pBullet);
	void FillTileforEnemy(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 posOld, D3DXCOLOR colEffect, TILETYPE tileType);
	void CollisionPlayer(void);
	void CollisionEnemy(CBullet *pBullet);
	void FillTile(TILETYPE tileType);
	TILETYPE GetTileType(void) { return m_tileType; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;								// テクスチャ情報のポインタ
	TILETYPE m_tileType;												// タイルの種類
	CTileRevival *m_pTileRevival;										// タイル復活情報のポインタ
	int m_nNum;															// 何番目のタイルか保存用
	int m_nCntRevival;													// 復活までのインターバル用カウント
};
#endif