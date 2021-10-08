//=============================================================================
//
// �i���o�[���� [number.h]
// Author : 
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "Scene2D.h"

//-----------------------------------------------------------------------------
// �i���o�[�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CNumber
{
public:
	CNumber();
	~CNumber();
	static HRESULT Load(void);
	static void Unload(void);
	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos,D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR2 GetSize(void) { return m_size; }
	void SetPosition(D3DXVECTOR3 pos);
	void SetCol(D3DXCOLOR col);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`�����̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR2 m_size;						// �傫��		
};
#endif