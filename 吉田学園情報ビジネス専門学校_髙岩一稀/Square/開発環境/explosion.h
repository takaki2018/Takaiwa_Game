//=============================================================================
//
// �������� [explosion.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "scene2D.h"

//*****************************************************************************
// �����N���X(�h���N���X)
//*****************************************************************************
class CExplosion : public CScene2D
{
public:
	// �����̎��
	typedef enum
	{
		EXPLOSIONTYPE_NONE = 0,	// �Ȃ�
		EXPLOSIONTYPE_PLAYER,	// �v���C���[
		EXPLOSIONTYPE_ENEMY,	// �G
		EXPLOSIONTYPE_WAVE,	// �Ռ��g
		EXPLOSIONTYPE_MAX,
	}ExplosionType;

	// �e�N�X�`���̎��
	typedef enum
	{
		EXPLOSION_TEXTURETYPE_NORMAL = 0,
		EXPLOSION_TEXTURETYPE_WAVE,
		EXPLOSION_TEXTURETYPE_MAX,
	}Explosion_TextureType;

	CExplosion(int nPriority);
	~CExplosion();

	static HRESULT Load(void);
	static void Unload(void);
	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXCOLOR col,float sizeMove, float fInertia,int nCntSwitch,
		float fMoveAlpha, ExplosionType explosionType,Explosion_TextureType textureType);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, float sizeMove, float fInertia, int nCntSwitch, 
		float fMoveAlpha,ExplosionType explosionType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �����G�t�F�N�g�̗v�f
	typedef struct
	{
		float sizeMove;					// �T�C�Y�ړ���
		float fInertia;					// ����
		int nCnt;						// �J�E���g
		int nCntSwitch;					// �؂�ւ��J�E���g
		ExplosionType explosionType;	// �����̎��
		bool bSwitch;					// �؂�ւ������ǂ���
		float fMoveAlpha;				// �����x�ړ���
	}Explosion;

	void NormalExplosion(void);											// �ʏ픚���̎�
	void WaveExplosion(void);											// �Ռ��g�̎�
	bool HitEnemy(void);												// �G�Ƃ̓����蔻��
	void CollosionTile(void);											// �^�C���Ƃ̂����蔻��

	static LPDIRECT3DTEXTURE9 m_pTexture[EXPLOSION_TEXTURETYPE_MAX];	// �e�N�X�`�����̃|�C���^
	Explosion m_explosion;												// �������
};
#endif