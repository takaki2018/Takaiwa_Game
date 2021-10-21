//=============================================================================
//
// �����_���[���� [renderer.h]
// Author : 
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// �����_���[�N���X(��{�N���X)
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

	void SetFadeBackEffect(void);				// �t�F�[�h�o�b�N�G�t�F�N�g

private:
	HRESULT InitRenderTex(void);				// �e�N�X�`�������_�����O�p�e�N�X�`���̏�����
	void DrawFPS(void);

	LPDIRECT3D9	m_pD3D = NULL;					// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;		// Device�I�u�W�F�N�g(�`��ɕK�v)
	LPD3DXFONT m_pFont = NULL;					// �t�H���g�ւ̃|�C���^

	LPDIRECT3DTEXTURE9 m_apTextureMT[2];		// �����_�����O�^�[�Q�b�g�p�̃e�N�X�`��
	LPDIRECT3DSURFACE9 m_apRenderMT[2];			// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X
	LPDIRECT3DSURFACE9 m_apBuffMT[2];			// �e�N�X�`�������_�����O�pZ�o�b�t�@
	D3DVIEWPORT9 m_viewportMT;					// �e�N�X�`�������_�����O�p�r���[�|�[�g
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;		// �t�B�[�h�o�b�N�p�|���S���̒��_�o�b�t�@

	float m_fAlpha;								// ���l
	float m_fAlphaDebug;
	D3DXVECTOR2 m_size;							// �T�C�Y
	bool m_bFadeBackEffect;
	int m_nCnt;
};
#endif