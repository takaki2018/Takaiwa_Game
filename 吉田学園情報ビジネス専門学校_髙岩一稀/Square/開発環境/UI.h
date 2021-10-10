//=============================================================================
//
// UI���� [UI.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "manager.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScore;				// �X�R�A�N���X
class CHiScore;				// �n�C�X�R�A�N���X
class CLife;				// ���C�t�N���X
class CUIBg;				// UI�w�i�N���X
class CCountDown;			// �J�E���g�_�E���N���X

//*****************************************************************************
// UI�N���X(��{�N���X)
//*****************************************************************************
class CUI
{
public:

	CUI();
	~CUI();

	HRESULT Init(CManager::MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *GetScore(void) { return m_pScore; }
	static CHiScore *GetHiScore(void) { return m_pHiScore; }
	static CLife *GetLife(void) { return m_pLife; }
	static CUIBg *GetLogoBg(void) { return m_pLogoBg; }
	static CCountDown *GetCountDown(void) { return m_pCountDown; }

private:
	static CScore *m_pScore;					// �X�R�A���̃|�C���^
	static CHiScore *m_pHiScore;				// �n�C�X�R�A���̃|�C���^
	static CLife *m_pLife;						// ���C�t���̃|�C���^
	static CUIBg *m_pLogoBg;					// ���S�w�i���
	static CCountDown *m_pCountDown;			// �J�E���g�_�E�����̃|�C���^
};
#endif