//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScore;			// �X�R�A�N���X

//*****************************************************************************
// ���U���g��ʃN���X(�h���N���X)
//*****************************************************************************
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *GetScore(void) { return m_pScore; }
	static CScore *GetMaxCombo(void) { return m_pMaxCombo; }
	static CScore *GetKillEnemy(void) { return m_pKillEnemy; }

private:
	static CScore *m_pScore;		// �X�R�A���̃|�C���^
	static CScore *m_pMaxCombo;		// �ő�R���{�����̃|�C���^
	static CScore *m_pKillEnemy;	// �G���j�����̃|�C���^
};
#endif