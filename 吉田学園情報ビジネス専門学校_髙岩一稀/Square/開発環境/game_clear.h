//=============================================================================
//
// �Q�[���N���A���� [game_clear.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _GAMECLEAR_H_
#define _GAMECLEAR_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DIGIT	(8)		// ����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLogo;		// ���S�N���X
class CUIBg;		// UI�w�i�N���X

//*****************************************************************************
// �Q�[���N���A�N���X(�h���N���X)
//*****************************************************************************
class CGameClear : public CScene
{
public:
	// �Q�[���J�ڂ̏���
	typedef enum
	{
		GAMETRANSITION_CLEAR = 0,
		GAMETRANSITION_OVER,
		GAMETRANSITION_MAX,
	}GameTransition;

	CGameClear(int nPriority);
	~CGameClear();

	static CGameClear *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, GameTransition gameTransition);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static bool GetTransition(void) { return m_bTransition; }

private:
	static bool m_bTransition;					// �J�ڂ��邩�ǂ���
	static int m_nCntInterval;					// �J�ڂ܂ł̎���
	static GameTransition m_gameTransition;		// �J�ڏ���
	CLogo *m_pLogo;								// ���S���̃|�C���^
	CUIBg *m_pUIBg;								// UI�w�i���̃|�C���^
};
#endif