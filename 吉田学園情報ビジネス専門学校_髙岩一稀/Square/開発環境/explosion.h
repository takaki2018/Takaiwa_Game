//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "scene2D.h"

//*****************************************************************************
// 爆発クラス(派生クラス)
//*****************************************************************************
class CExplosion : public CScene2D
{
public:
	// 爆発の種類
	typedef enum
	{
		EXPLOSIONTYPE_NONE = 0,	// なし
		EXPLOSIONTYPE_PLAYER,	// プレイヤー
		EXPLOSIONTYPE_ENEMY,	// 敵
		EXPLOSIONTYPE_WAVE,	// 衝撃波
		EXPLOSIONTYPE_MAX,
	}ExplosionType;

	// テクスチャの種類
	typedef enum
	{
		EXPLOSION_TEXTURETYPE_NORMAL = 0,
		EXPLOSION_TEXTURETYPE_WAVE,
		EXPLOSION_TEXTURETYPE_MAX,
	}Explosion_TextureType;

	CExplosion(int nPriority);
	~CExplosion();

	static HRESULT Load(void);
	static void Unload(void);
	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXCOLOR col,float sizeMove, float fInertia,int nCntSwitch,
		float fMoveAlpha, ExplosionType explosionType,Explosion_TextureType textureType);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, float sizeMove, float fInertia, int nCntSwitch, 
		float fMoveAlpha,ExplosionType explosionType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// 爆発エフェクトの要素
	typedef struct
	{
		float sizeMove;					// サイズ移動量
		float fInertia;					// 慣性
		int nCnt;						// カウント
		int nCntSwitch;					// 切り替えカウント
		ExplosionType explosionType;	// 爆発の種類
		bool bSwitch;					// 切り替えたかどうか
		float fMoveAlpha;				// 透明度移動量
	}Explosion;

	void NormalExplosion(void);											// 通常爆発の時
	void WaveExplosion(void);											// 衝撃波の時
	bool HitEnemy(void);												// 敵との当たり判定
	void CollosionTile(void);											// タイルとのあたり判定

	static LPDIRECT3DTEXTURE9 m_pTexture[EXPLOSION_TEXTURETYPE_MAX];	// テクスチャ情報のポインタ
	Explosion m_explosion;												// 爆発情報
};
#endif