//=============================================================================
//
// レンダラー処理 [renderer.h]
// Author : 
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// レンダラークラス(基本クラス)
//-----------------------------------------------------------------------------
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	void SetFadeBackEffect(void);				// フェードバックエフェクト

private:
	HRESULT InitRenderTex(void);				// テクスチャレンダリング用テクスチャの初期化
	void DrawFPS(void);

	LPDIRECT3D9	m_pD3D = NULL;					// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;		// Deviceオブジェクト(描画に必要)
	LPD3DXFONT m_pFont = NULL;					// フォントへのポインタ

	LPDIRECT3DTEXTURE9 m_apTextureMT[2];		// レンダリングターゲット用のテクスチャ
	LPDIRECT3DSURFACE9 m_apRenderMT[2];			// テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9 m_apBuffMT[2];			// テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9 m_viewportMT;					// テクスチャレンダリング用ビューポート
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;		// フィードバック用ポリゴンの頂点バッファ

	float m_fAlpha;								// α値
	float m_fAlphaDebug;
	D3DXVECTOR2 m_size;							// サイズ
	bool m_bFadeBackEffect;
	int m_nCnt;
};
#endif