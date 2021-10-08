//=============================================================================
//
// �V�[��3D���� [scene2D.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "scene.h"

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�N���X(��{�N���X)
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
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// �e�N�X�`�����̃|�C���^
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;							// ���W
	D3DXCOLOR m_col;							// ���_�J���[

	D3DXVECTOR3	m_rot;							// �|���S���̉�]�p
	float m_fAngle;								// �|���S���̑Ίp���̊p�x
	float m_fLength;							// �|���S���̑Ίp���̒���
	float m_fWidth;								// �|���S���̕�
	float m_fDepth;							// �|���S���̍���

	bool m_bDeath;								// ���S�t���O
};
#endif