//=============================================================================
//
// �n�C�X�R�A���� [hiscore.h]
// Author : istuki takaiwa
//
//=============================================================================
#ifndef _HISCORE_H_
#define _HISCORE_H_

#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DIGIT	(8)		// ����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;			// �i���o�[�N���X

//-----------------------------------------------------------------------------
// �n�C�X�R�A�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CHiScore : public CScore
{
public:
	CHiScore(int nPriority);
	~CHiScore();

	static CHiScore *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,float fSpace, SCOREPLACEMENT scorePlacement);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fSpace, SCOREPLACEMENT scorePlacement);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetScore(void) { return m_nHiScore; }

private:
	void LoadHiScore(void);				// �n�C�X�R�A�̃��[�h

	int m_nHiScore;						// �X�R�A
};
#endif