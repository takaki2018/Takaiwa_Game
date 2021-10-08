//-------------------------------------------------------------------
//
// �Z�b�g�p�[�e�B�N���w�b�_�[ (setparticle.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETPARTICLE_H_
#define _SETPARTICLE_H_

#include "main.h"

//-------------------------------------------------------------------
// �p�[�e�B�N���N���X�i��{�N���X�j
//-------------------------------------------------------------------
class CParticle
{
public:
	typedef enum
	{
		BULLETMOVE_UP = 0,
		BULLETMOVE_DOWN,
		BULLETMOVE_RIGHT,
		BULLETMOVE_LEFT,
		BULLETMOVE_MAX,
	}BulletMove;

	static void SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR col,float fRemoveAlpha, D3DXVECTOR2 sizeSplit);			// ����
	static void SetFillTile(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR2 size,D3DXCOLOR col, BulletMove bulletMove,D3DXVECTOR2 sizeSplit);			// �^�C���̔j��
	static void SetRevivalTile(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR col, BulletMove bulletMove, D3DXVECTOR2 sizeSplit);	// �^�C���̕���
	static void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, D3DXCOLOR col);														// �O��
};

#endif
