//=============================================================================
//
// �X�R�A���� [score.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DIGIT	(8)		// ����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;			// �i���o�[�N���X

//*****************************************************************************
// �I�u�W�F�N�g�N���X(��{�N���X)
//*****************************************************************************
class CScore : public CScene
{
public:
	// �X�R�A�z�u�̎��
	typedef enum
	{
		SCOREPLACEMENT_NORMAL = 0,	// �ʏ�z�u
		SCOREPLACEMENT_RIGHT,		// �E��
		SCOREPLACEMENT_LEFT,		// ����
		SCOREPLACEMENT_MAX,
	}SCOREPLACEMENT;

	CScore(int nPriority);
	~CScore();

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,float fSpace,SCOREPLACEMENT scorePlacement);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetScore(int nScore);
	void AddScore(int nValue);
	int GetScore(void) { return m_nScore; }	
	void SetScorePlacement(SCOREPLACEMENT scorePlacement) { m_scorePlacement = scorePlacement; } 
	void SetSpace(float fSpace) { m_fSpace = fSpace; }

private:
	void RightJustified(void);			// �E�񂹔z�u
	void LeftJustified(void);			// ���񂹔z�u
	void Placementtype(void);			// �X�R�A�̕`��X�V
	CNumber *m_apNumber[MAX_DIGIT];		// �i���o�[�N���X�̃|�C���^
	float m_fSpace;						// �z�u�̊Ԋu
	int m_nScore;						// �X�R�A
	int m_nDigit;						// ����
	SCOREPLACEMENT m_scorePlacement;	// �X�R�A�z�u�̎��
};
#endif