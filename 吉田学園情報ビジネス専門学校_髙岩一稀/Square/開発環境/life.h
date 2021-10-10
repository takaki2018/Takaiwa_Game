//=============================================================================
//
// ���C�t���� [life.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIFE		(3)			// �ő僉�C�t��
#define LIFE_SIZE		(30.0f)		// ���C�t�̃T�C�Y

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// ���C�t�N���X(�h���N���X)
//*****************************************************************************
class CLife : public CScene
{
public:
	CLife(int nPriority);
	~CLife();

	static HRESULT Load(void);
	static void Unload(void);
	static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetLife(void) { return m_nLife; }
	void SetLife(int nLife);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`�����̃|�C���^
	CScene2D *m_apScene2D[MAX_LIFE];		// 2D�|���S�����
	int m_nLife;							// ���C�t��
};
#endif