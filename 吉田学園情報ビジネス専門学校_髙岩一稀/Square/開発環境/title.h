//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LOGOBG		(2)		// ���S�w�i�̎��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUIBg;		// UI�w�i�N���X
class CModel;		// ���f���N���X

//*****************************************************************************
// �^�C�g����ʃN���X(�h���N���X)
//*****************************************************************************
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static bool GetPressEnter(void) { return m_bPressEnter; }
	static CUIBg **GetLogoBg(void) { return &m_apLogoBg[0]; }

private:
	static bool m_bPressEnter;				// �G���^�[�����������ǂ���
	static CUIBg *m_apLogoBg[MAX_LOGOBG];	// ���S�w�i���
};
#endif