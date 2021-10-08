//=============================================================================
//
// �Q�[�����S���� [game_logo.h]
// Author : 
//
//=============================================================================
#ifndef _GAMELOGO_H_
#define _GAMELOGO_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLogo;			// ���S�N���X

//-----------------------------------------------------------------------------
// �Q�[�����S�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CGameLogo : public CScene
{
public:
	// ���S�̎�ނ̎��
	typedef enum
	{
		LOGOTYPE_SCORE = 0,		// �X�R�A���S
		LOGOTYPE_HIGHSCORE,		// �n�C�X�R�A���S
		LOGOTYPE_PRESSENTER,	// PressEnter
		LOGOTYPE_MAX,
	}LOGOTYPE;

	CGameLogo(int nPriority);
	~CGameLogo();
	static CGameLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static bool GetTransition(void) { return bTransition; }

private:
	CLogo *m_apLogo[LOGOTYPE_MAX];		// ���S�N���X�̃|�C���^
	static bool bTransition;			// �J�ڂ��邩�ǂ���
};
#endif