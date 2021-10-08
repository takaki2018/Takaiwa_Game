//=============================================================================
//
// タイルフレームエフェクト処理 [tile_frame_effect.h]
// Author : 
//
//=============================================================================
#ifndef _TILEFRAMEEFFECT_H_
#define _TILEFRAMEEFFECT_H_

#include "scene2D.h"

//-----------------------------------------------------------------------------
// タイルフレームエフェクトクラス(派生クラス)
//-----------------------------------------------------------------------------
class CTileFrameEffect : public CScene2D
{
public:
	CTileFrameEffect(int nPriority);
	~CTileFrameEffect();

	static HRESULT Load(void);
	static void Unload(void);
	static CTileFrameEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR3 move);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	bool CollitionTile(D3DXVECTOR3 pos, D3DXVECTOR2 size);	// タイルとのあたり判定
	static LPDIRECT3DTEXTURE9 m_pTexture;					// テクスチャ情報のポインタ
	D3DXVECTOR3 m_posOld;									// 前回の位置
	D3DXVECTOR3 m_move;										// 移動量
};
#endif