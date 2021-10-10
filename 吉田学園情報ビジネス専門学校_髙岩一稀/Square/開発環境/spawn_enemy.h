//=============================================================================
//
// �G�̔������� [spawn_enemy.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _SPAWNENEMY_H_
#define _SPAWNENEMY_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FHASE		(10)		// �ő�t�F�[�Y��
#define MAX_ENEMYSPAWN	(50)		// 1�t�F�[�Y�̓o�ꂷ��G�̍ő吔

//*****************************************************************************
// �G�����N���X(�h���N���X)
//***************************************************************************** ----------------------------------------------------------
class CSpawnEnemy : public CScene
{
public:
	CSpawnEnemy(int nPriority = 0);
	~CSpawnEnemy();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static int GetRemainingEnemy(void) { return m_nCntRemainingEnemy; }
	static void SetRemainingEnemy(int nCntRemainingEnemy) { m_nCntRemainingEnemy = nCntRemainingEnemy; }

private:
	bool LoadSpawnEnemyData(void);	// �G�z�u���ǂݍ���

	// �G���������܂Ƃ߂��\����
	typedef struct
	{
		int nNumTile;			// ���Ԗڂ̃^�C����
		D3DXVECTOR2 size;		// �T�C�Y���
		float fMoveAI;			// ��{�ړ���
		int nEnemyType;			// ����������G�̎��
		int nCntSpawnInterval;	// �G�𔭐�������܂ł̎���
		int nCntInterval;		// ���̓G�𔭐�������܂ł̎���
		bool bGroup;			// �W�c(�܂Ƃ߂�)�ŃX�|�[�����邩�ǂ���
		int nCntGroupEnemy;		// ���̓����ɏo�������邩
		bool bUse;				// �g�p���Ă��邩�ǂ���
	}SpawnEnemy_Info;

	static char SpawnEnemyText[MAX_FHASE][128];						// �G�������e�L�X�g�̃e�L�X�g���i�[�p
	SpawnEnemy_Info m_aSpawnEnemy_info[MAX_FHASE][MAX_ENEMYSPAWN];	// �G�������
	int m_nCounter;													// �J�E���^�[
	int m_nNumFhase;												// ���t�F�[�Y�ڂ��J�E���g
	int m_nCntEnemy;												// ���Ԗڂ̓G���J�E���g
	static int m_nCntRemainingEnemy;								// �c��G��
	int m_nNumEnemy[MAX_FHASE];										// 1�t�F�[�Y�̓G�̐�
};
#endif