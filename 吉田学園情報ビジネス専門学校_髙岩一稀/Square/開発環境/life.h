//=============================================================================
//
// ライフ処理 [life.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

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
class CLife : public CScene
{
public:
	CLife(int nPriority);
	~CLife();

	static HRESULT Load(void);
	static void Unload(void);
	static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
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