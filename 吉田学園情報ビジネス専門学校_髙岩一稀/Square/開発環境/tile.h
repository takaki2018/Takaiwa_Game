//=============================================================================
//
// �^�C������ [tile.h]
// Author : 
//
//=============================================================================
#ifndef _TILE_H_
#define _TILE_H_

#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TILE_SIZE		(45.0f)		// �^�C���̑傫��

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CBullet;		// �e�N���X
class CTileRevival;	// �^�C�������N���X

//-----------------------------------------------------------------------------
// �^�C���N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CTile : public CScene2D
{
public:
	CTile(int nPriority);
	~CTile();

	// �^�C���̎��
	typedef enum
	{
		TILETYPE_NONE = 0,
		TILETYPE_BLACK,			// ��
		TILETYPE_WHITE,			// ��
		TILETYPE_INVISIBLE,		// ����(�X�e�[�W�O���ɏo�Ȃ��悤�ɂ��邽�ߎg�p)
		TILETYPE_CHANGE_BLACK,	// ���ɕς��Ƃ�
		TILETYPE_CHANGE_WHITE,	// ���ɕς��Ƃ�
		TILETYPE_MAX,
	}TILETYPE;

	static HRESULT Load(void);
	static void Unload(void);
	static CTile *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type,int nNum);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE type, int nNum);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void FillTileTypeBlack(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void FillTileTypeWhite(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void FillTileTypeChangeBlack(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void CircleCollision(D3DXVECTOR3 pos, D3DXVECTOR2 size, TILETYPE tileType);
	void EffectReflection(D3DXVECTOR3 *pPos,D3DXVECTOR3 *pMove, D3DXVECTOR2 size);
	bool CollisionObject(D3DXVECTOR3 *pos, D3DXVECTOR3 posOld, D3DXVECTOR3 *move, D3DXVECTOR2 size);
	void FillTileforBullet(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR3 posOld,D3DXCOLOR colEffect,TILETYPE tileType, CBullet *pBullet);
	void FillTileforEnemy(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 posOld, D3DXCOLOR colEffect, TILETYPE tileType);
	void CollisionPlayer(void);
	void CollisionEnemy(CBullet *pBullet);
	void FillTile(TILETYPE tileType);
	TILETYPE GetTileType(void) { return m_tileType; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;								// �e�N�X�`�����̃|�C���^
	TILETYPE m_tileType;												// �^�C���̎��
	CTileRevival *m_pTileRevival;										// �^�C���������̃|�C���^
	int m_nNum;															// ���Ԗڂ̃^�C�����ۑ��p
	int m_nCntRevival;													// �����܂ł̃C���^�[�o���p�J�E���g
};
#endif