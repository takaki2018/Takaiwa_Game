//=============================================================================
//
// 背景処理 [bg.h]
// Author : 
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BG		(2)		// 背景の最大数

//-----------------------------------------------------------------------------
// 背景クラス(基本クラス)
//-----------------------------------------------------------------------------
class CBg : public CScene2D
{
public:
	// テクスチャの種類
	typedef enum
	{
		TEXTURETYPE_TITLE = 0,		// タイトル背景
		TEXTURETYPE_GAME,			// ゲーム背景
		TEXTURETYPE_MAX,
	}TEXTURETYPE_BG;

	CBg(int nPriority);
	~CBg();
	static HRESULT Load(void);
	static void Unload(void);
	static CBg *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXCOLOR col,int nTextureNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURETYPE_MAX];	// テクスチャ情報のポインタ
	int m_nTextureNumber;									// テクスチャ識別番号
};
#endif