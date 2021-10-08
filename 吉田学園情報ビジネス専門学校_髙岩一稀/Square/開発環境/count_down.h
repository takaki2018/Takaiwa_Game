//=============================================================================
//
// �J�E���g�_�E������ [count_down.h]
// Author : 
//
//=============================================================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_NUMBER		(4)			// �J�E���g�_�E���̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;		// �����N���X
class CLogo;		// ���S�N���X
class CUIBg;		// �w�i�N���X

//-----------------------------------------------------------------------------
// ���C�t�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CCountDown : public CScene
{
public:
	CCountDown(int nPriority);
	~CCountDown();
	static HRESULT Load(void);
	static void Unload(void);
	static CCountDown *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static bool GetFinishCountDown(void) { return m_bFinishCountDown; }
	int GetCountDown(void) { return m_nNumCountDown; }

private:
	CNumber *m_apNumber[MAX_NUMBER];		// �������
	CLogo *m_pLogo;							// ���S���
	static bool m_bFinishCountDown;			// �J�E���g�_�E�����I��������ǂ���
	int m_nCntCountDown;					// �J�E���g�_�E���̃J�E���g
	int m_nNumCountDown;					// ���Ԗڂ̃J�E���g�_�E����
};
#endif