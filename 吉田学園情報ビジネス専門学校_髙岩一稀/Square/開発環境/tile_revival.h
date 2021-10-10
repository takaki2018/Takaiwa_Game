//=============================================================================
//
// タイル復活処理 [tile_revival.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TILEREVIVAL_H_
#define _TILEREVIVAL_H_

#include "scene2D.h"

//*****************************************************************************
// タイル復活クラス(派生クラス)
//*****************************************************************************
class CTileRevival : public CScene2D
{
public:
	CTileRevival(int nPriority);
	~CTileRevival();

	// タイルの種類
	typedef enum
	{
		TILETYPE_NONE = 0,
		TILETYPE_BLACK,			// 黒
		TILETYPE_WHITE,			// 白
		TILETYPE_MAX,
	}TILETYPE;

	static HRESULT Load(void);
	static void Unload(void);
	static CTileRevival *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type,int nNum);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type, int nNum);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;								// テクスチャ情報のポインタ
	TILETYPE m_tileType;												// タイルの種類
	int m_nNum;															// 何番目のタイルか
};
#endif