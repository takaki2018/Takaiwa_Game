//=============================================================================
//
// UI背景処理 [UI_bg.h]
// Author : 
//
//=============================================================================
#ifndef _UIBG_H_
#define _UIBG_H_

#include "scene2D.h"

//-----------------------------------------------------------------------------
// ハイスコアクラス(派生クラス)
//-----------------------------------------------------------------------------
class CUIBg : public CScene2D
{
public:
	// テクスチャの種類
	typedef enum
	{
		TEXTURETYPE_UI,			// UI背景
		TEXTURETYPE_LOGO,		// ロゴ背景
		TEXTURETYPE_MAX,
	}TEXTURETYPE_UIBG;

	CUIBg(int nPriority);
	~CUIBg();
	static HRESULT Load(void);
	static void Unload(void);
	static CUIBg *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURETYPE_MAX];	// テクスチャ情報のポインタ
};
#endif