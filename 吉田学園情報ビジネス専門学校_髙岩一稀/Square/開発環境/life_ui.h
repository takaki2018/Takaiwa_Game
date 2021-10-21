//=============================================================================
//
// ライフUI処理 [life_ui.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _LIFEUI_H_
#define _LIFEUI_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIFE		(3)			// 最大ライフ数
#define LIFE_SIZE		(30.0f)		// ライフのサイズ

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// ライフクラス(派生クラス)
//*****************************************************************************
class CLifeUI : public CScene
{
public:
	CLifeUI(int nPriority);
	~CLifeUI();

	static HRESULT Load(void);
	static void Unload(void);
	static CLifeUI *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetLife(void) { return m_nLife; }
	void SetLife(int nLife);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ情報のポインタ
	CScene2D *m_apScene2D[MAX_LIFE];		// 2Dポリゴン情報
	int m_nLife;							// ライフ数
};
#endif