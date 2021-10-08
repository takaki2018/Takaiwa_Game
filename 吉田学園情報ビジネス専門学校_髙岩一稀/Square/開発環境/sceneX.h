//-------------------------------------------------------------------
//
// ���f���w�b�_�[ (model.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "scene.h"

//-------------------------------------------------------------------
// �V�[��X�N���X(�h���N���X)
//-------------------------------------------------------------------
class CSceneX : public CScene
{
public:
	CSceneX(int nPriority);
	~CSceneX();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);			
	void Update(void);			
	void Draw(void);			
	void BindXFileName(char *pModelFileName) { m_pModelFileName = pModelFileName; }
	D3DXVECTOR3 GetPosision(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRotate(void) { return m_rot; }
	void SetRotate(D3DXVECTOR3 rot);
	LPD3DXBUFFER GetMat(void) { return m_pBuffMat; }
	int GetNumMat(void) { return m_nNumMat; }
	void SetMat(D3DXMATERIAL *pMat, int nCntMat);

private:
	char *m_pModelFileName;				// ���f���̃t�@�C����
	LPDIRECT3DTEXTURE9 m_pTexture[8];	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH m_pMesh;					// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;			// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD m_nNumMat;					// �}�e���A���̐�
	D3DXVECTOR3 m_pos;					// ���S���W
	D3DXVECTOR3 m_rot;					// ��]�p
	D3DXMATRIX m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_vtxMinModel;			// �e���_���W�̍ŏ��l
	D3DXVECTOR3 m_vtxMaxModel;			// �e���_���W�̍ő�l
};

#endif