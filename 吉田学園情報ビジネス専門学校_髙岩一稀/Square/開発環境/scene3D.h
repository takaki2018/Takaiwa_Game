//=============================================================================
//
// シーン3D処理 [scene2D.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "scene.h"

//-----------------------------------------------------------------------------
// オブジェクトクラス(基本クラス)
//-----------------------------------------------------------------------------
class CScene3D : public CScene
{
public:
	CScene3D(int nPriority);
	~CScene3D();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_fWidth, m_fDepth); }
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR2 size);
	void SetVertex(void);
	void SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets);
	void SetCol(D3DXCOLOR col);
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// テクスチャ情報のポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3	m_pos;							// 座標
	D3DXCOLOR m_col;							// 頂点カラー

	D3DXVECTOR3	m_rot;							// ポリゴンの回転角
	float m_fAngle;								// ポリゴンの対角線の角度
	float m_fLength;							// ポリゴンの対角線の長さ
	float m_fWidth;								// ポリゴンの幅
	float m_fDepth;							// ポリゴンの高さ

	bool m_bDeath;								// 死亡フラグ
};
#endif