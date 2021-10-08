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

private:
	void DrawFPS(void);

	LPDIRECT3D9	m_pD3D = NULL;					// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;		// Deviceオブジェクト(描画に必要)
	LPD3DXFONT m_pFont = NULL;					// フォントへのポインタ
};
#endif