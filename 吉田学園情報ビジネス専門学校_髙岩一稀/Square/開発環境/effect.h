//=============================================================================
//
// エフェクトヘッダー (effect.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "scene2D.h"

//*****************************************************************************
// エフェクトクラス（派生クラス）
//*****************************************************************************
class CEffect : public CScene2D
{
public:
	CEffect(int nPriority);
	~CEffect();

	// テクスチャの種類
	typedef enum
	{
		EFFECT_TYPE_NORMAL = 0,
		EFFECT_TYPE_EEXPLOSION ,
		EFFECT_TYPE_TILE,
		EFFECT_TYPE_MAX,
	}EFFECT_TYPE;

	static HRESULT Load(void);
	static void Unload(void);
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
		float fRemove, D3DXVECTOR2 size, float fCntAlpha, int nLife, EFFECT_TYPE type,int nInterval);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
		float fRemove, D3DXVECTOR2 size, float fCntAlpha, int nLife, int nInterval);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// エフェクトの要素
	typedef struct
	{
		D3DXVECTOR3 move;		// 移動量
		D3DXCOLOR col;			// 色
		float fRemove;			// 移動量の減少量
		float nCntAlpha;		// α値の移動
		int nLife;				// 寿命
		int nInterval;			// カウント制御
	} Effect;

	static LPDIRECT3DTEXTURE9 m_pTexture[EFFECT_TYPE_MAX];	// テクスチャ情報のポインタ
	Effect m_effect;										// エフェクト情報
};

#endif
