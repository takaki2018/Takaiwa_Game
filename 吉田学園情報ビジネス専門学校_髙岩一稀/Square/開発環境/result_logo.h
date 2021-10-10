//=============================================================================
//
// ���U���g���S���� [result_logo.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULTLOGO_H_
#define _RESULTLOGO_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLogo;			// ���S�N���X
class CUIBg;			// UI�w�i�N���X

//*****************************************************************************
// ���U���g���S�N���X(�h���N���X)
//*****************************************************************************
class CResultLogo : public CScene
{
public:
	// ���S�̎�ނ̎��
	typedef enum
	{
		LOGOTYPE_RESULT = 0,		// ���U���g���S
		LOGOTYPE_RANK,				// �����N���S
		LOGOTYPE_YOURSCORE,			// YourScore���S
		LOGOTYPE_NEWRECORD,			// NewRecord���S
		LOGOTYPE_MAXCOMBO,			// �ő�R���{�����S
		LOGOTYPE_KILLENEMY,			// �G���j�����S
		LOGOTYPE_MAX,
	}LOGOTYPE;

	CResultLogo(int nPriority);
	~CResultLogo();

	static CResultLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLogo **GetResultLogo(void) { return &m_apLogo[0]; }

private:
	static CLogo *m_apLogo[LOGOTYPE_MAX];		// ���S�N���X�̃|�C���^
};
#endif