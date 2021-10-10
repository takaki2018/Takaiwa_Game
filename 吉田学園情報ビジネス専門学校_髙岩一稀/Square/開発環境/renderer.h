//=============================================================================
//
// �����_���[���� [renderer.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//*****************************************************************************
// �����_���[�N���X(��{�N���X)
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// �f�o�C�X����Ԃ�

private:
	void DrawFPS(void);							// FPS�\��

	LPDIRECT3D9	m_pD3D = NULL;					// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;		// Device�I�u�W�F�N�g(�`��ɕK�v)
	LPD3DXFONT m_pFont = NULL;					// �t�H���g�ւ̃|�C���^
};
#endif