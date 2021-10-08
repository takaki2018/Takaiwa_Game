//=============================================================================
//
// �^�C���������� [tile_revival.h]
// Author : 
//
//=============================================================================
#ifndef _TILEREVIVAL_H_
#define _TILEREVIVAL_H_

#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TILE_SIZE		(45.0f)		// �^�C���̑傫��

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CBullet;		// �e�N���X

//-----------------------------------------------------------------------------
// �^�C�������N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CTileRevival : public CScene2D
{
public:
	CTileRevival(int nPriority);
	~CTileRevival();

	// �^�C���̎��
	typedef enum
	{
		TILETYPE_NONE = 0,
		TILETYPE_BLACK,			// ��
		TILETYPE_WHITE,			// ��
		TILETYPE_MAX,
	}TILETYPE;

	static HRESULT Load(void);
	static void Unload(void);
	static CTileRevival *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type,int nNum);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type, int nNum);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;								// �e�N�X�`�����̃|�C���^
	TILETYPE m_tileType;												// �^�C���̎��
	int m_nNum;															// ���Ԗڂ̃^�C����
};
#endif