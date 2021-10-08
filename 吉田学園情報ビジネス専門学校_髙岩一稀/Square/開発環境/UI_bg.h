//=============================================================================
//
// UI�w�i���� [UI_bg.h]
// Author : 
//
//=============================================================================
#ifndef _UIBG_H_
#define _UIBG_H_

#include "scene2D.h"

//-----------------------------------------------------------------------------
// �n�C�X�R�A�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CUIBg : public CScene2D
{
public:
	// �e�N�X�`���̎��
	typedef enum
	{
		TEXTURETYPE_UI,			// UI�w�i
		TEXTURETYPE_LOGO,		// ���S�w�i
		TEXTURETYPE_MAX,
	}TEXTURETYPE_UIBG;

	CUIBg(int nPriority);
	~CUIBg();
	static HRESULT Load(void);
	static void Unload(void);
	static CUIBg *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURETYPE_MAX];	// �e�N�X�`�����̃|�C���^
};
#endif