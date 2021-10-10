//=============================================================================
//
// �e���� [bullet.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _BUllET_H_
#define _BUllET_H_

#include "scene2D.h"

//*****************************************************************************
// �e�N���X(�h���N���X)
//*****************************************************************************
class CBullet : public CScene2D
{
public:
	CBullet(int nPriority);
	~CBullet();

	// �e�̎��
	typedef enum
	{
		BULETTTYPE_NONE = 0,
		BULLETTYPE_PLEYER,
		BULLETTYPE_ENEMY,
		BULLETTYPE_MAX,
	}BULLETTYPE;

	// �e�����˂������
	typedef enum
	{
		BULLETDIRECTION_UP = 0,
		BULLETDIRECTION_DOWN,
		BULLETDIRECTION_RIGHT,
		BULLETDIRECTION_LEFT,
		BULLETDIRECTION_MAX,
	}BULLETDIRECTION;

	static HRESULT Load(void);
	static void Unload(void);
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, BULLETTYPE type,BULLETDIRECTION direction);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, BULLETTYPE type, BULLETDIRECTION direction);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetOldPosition(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	BULLETTYPE GetBulletType(void) { return m_bulletType; }

private:
	void Move(void);						// �ړ�����
	void CollisionTile(void);				// �^�C���Ƃ̂����蔻��

	D3DXVECTOR3 m_move;						// �ړ���
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`�����̃|�C���^
	int m_nLife;							// �e�̎���
	BULLETTYPE m_bulletType;				// �e�̎��
	D3DXVECTOR3 m_posOld;					// �O��̈ʒu
};
#endif