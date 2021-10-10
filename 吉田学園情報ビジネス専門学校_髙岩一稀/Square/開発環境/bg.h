//=============================================================================
//
// �w�i���� [bg.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "scene2D.h"

//*****************************************************************************
// �w�i�N���X(�h���N���X)
//*****************************************************************************
class CBg : public CScene2D
{
public:
	// �e�N�X�`���̎��
	typedef enum
	{
		TEXTURETYPE_TITLE = 0,		// �^�C�g���w�i
		TEXTURETYPE_GAME,			// �Q�[���w�i
		TEXTURETYPE_MAX,
	}TEXTURETYPE_BG;

	CBg(int nPriority);
	~CBg();

	static HRESULT Load(void);
	static void Unload(void);
	static CBg *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXCOLOR col,int nTextureNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTextureNumber);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURETYPE_MAX];	// �e�N�X�`�����̃|�C���^
	int m_nTextureNumber;									// �e�N�X�`�����ʔԍ�
};
#endif