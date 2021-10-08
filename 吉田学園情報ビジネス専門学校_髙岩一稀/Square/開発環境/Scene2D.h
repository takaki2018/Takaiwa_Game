//=============================================================================
//
// �V�[��2D���� [scene2D.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1) // ���_�t�H�[�}�b�g
#define MAX_VERTEX (4)												// ���_�̐�

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef  struct
{
	D3DXVECTOR3 pos; // ���_���W
	float rhw;	     // 1.0�ŌŒ�
	D3DCOLOR col;    // ���_�J���[
	D3DXVECTOR2 tex;
} VERTEX_2D;

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority);
	~CScene2D();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_fWidth,m_fHeight); }
	D3DXVECTOR3 GetRotate(void) { return m_rot; }
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR2 size);
	void SetRotate(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetRotateVertex(float fAngle);
	void SetVertex(void);
	void SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets);
	void SetCol(D3DXCOLOR col);
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// �e�N�X�`�����̃|�C���^
	D3DXVECTOR3	m_pos;							// ���W
	D3DXCOLOR m_col;							// ���_�J���[

	D3DXVECTOR3	m_rot;							// �|���S���̉�]�p
	float m_fAngle;								// �|���S���̑Ίp���̊p�x
	float m_fLength;							// �|���S���̑Ίp���̒���
	float m_fWidth;								// �|���S���̕�
	float m_fHeight;							// �|���S���̍���

	bool m_bDeath;								// ���S�t���O
};
#endif