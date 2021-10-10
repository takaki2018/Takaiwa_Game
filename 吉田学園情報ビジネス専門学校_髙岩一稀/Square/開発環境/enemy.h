//=============================================================================
//
// �G���� [enemy.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMYTYPE	(1)			// �G�̎��
#define BULLET_MOVE		(7.0f)		// �e�̈ړ���
#define BULLET_SIZE		(25.0f)		// �e�̃T�C�Y

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;			// �v���C���[�N���X

//*****************************************************************************
// �G�N���X(�h���N���X)
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
	// �G�̎��
	typedef enum
	{
		ENEMYTYPE_NORMAL = 0,	// �ʏ�
		ENEMYTYPE_SHOOT,		// �e�����G
		ENEMYTYPE_NAX,
	}EnemyType;

	CEnemy(int nPriority);
	~CEnemy();

	static HRESULT Load(void);
	static void Unload(void);
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,float fMoveAI,EnemyType enemyType);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, EnemyType enemyType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 CollitionTile(D3DXVECTOR3 pos, D3DXVECTOR2 size);	// �^�C���Ƃ̂����蔻��
	bool CollisionBullet(void);										// �e�Ƃ̓����蔻��
	bool CollisionPlayer(CPlayer *pPlayer);							// �v���C���[�Ƃ̓����蔻��
	void HitEnemy(void);
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMYTYPE_NAX];			// �e�N�X�`�����̃|�C���^
	D3DXVECTOR3 m_posOld;											// �O��̈ʒu
	float m_fMoveAI;												// ��{�ړ���
	D3DXVECTOR3 m_move;												// �ړ���
	EnemyType m_enemyType;											// �G�̎��
};
#endif