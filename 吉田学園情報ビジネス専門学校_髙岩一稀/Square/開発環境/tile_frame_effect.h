//=============================================================================
//
// �^�C���t���[���G�t�F�N�g���� [tile_frame_effect.h]
// Author : 
//
//=============================================================================
#ifndef _TILEFRAMEEFFECT_H_
#define _TILEFRAMEEFFECT_H_

#include "scene2D.h"

//-----------------------------------------------------------------------------
// �^�C���t���[���G�t�F�N�g�N���X(�h���N���X)
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
	bool CollitionTile(D3DXVECTOR3 pos, D3DXVECTOR2 size);	// �^�C���Ƃ̂����蔻��
	static LPDIRECT3DTEXTURE9 m_pTexture;					// �e�N�X�`�����̃|�C���^
	D3DXVECTOR3 m_posOld;									// �O��̈ʒu
	D3DXVECTOR3 m_move;										// �ړ���
};
#endif