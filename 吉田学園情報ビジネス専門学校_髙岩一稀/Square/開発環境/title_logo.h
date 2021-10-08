//=============================================================================
//
// �^�C�g�����S���� [title_logo.h]
// Author : 
//
//=============================================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLogo;			// ���S�N���X
class CUIBg;			// UI�w�i�N���X

//-----------------------------------------------------------------------------
// �^�C�g�����S�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CTitleLogo : public CScene
{
public:
	// ���S�̎�ނ̎��
	typedef enum
	{
		LOGOTYPE_TITLE = 0,		// �^�C�g�����S
		LOGOTYPE_PLAY,			// �v���C���S
		LOGOTYPE_TUTORIAL,		// �`���[�g���A�����S
		LOGOTYPE_RANKING,		// �����L���O���S
		LOGOTYPE_OPTION,		// �I�v�V�������S
		LOGOTYPE_MAX,
	}LOGOTYPE;

	CTitleLogo(int nPriority);
	~CTitleLogo();
	static CTitleLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static bool GetTransition(void) { return bTransition; }

private:
	CLogo *m_apLogo[LOGOTYPE_MAX];		// ���S�N���X�̃|�C���^
	int m_nSelectLogo;					// ���Ԗڂ̃��S���I�΂�Ă��邩
	static bool bTransition;			// �J�ڂ��邩�ǂ���
};
#endif