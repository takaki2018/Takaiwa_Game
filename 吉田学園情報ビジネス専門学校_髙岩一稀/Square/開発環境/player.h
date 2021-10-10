//=============================================================================
//
// �v���C���[���� [player.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene2D.h"

//*****************************************************************************
// �v���C���[�N���X(�h���N���X)
//*****************************************************************************
class CPlayer : public CScene2D
{
public:
	// �v���C���[�̏��
	typedef enum
	{
		PLAYERSTATE_NORMAL = 0,		// �ʏ�
		PLAYERSTATE_INVINCIBLE,		// ���G
		PLAYERSTATE_HIT,			// �e��G����������
		PLAYERSTATE_MAX,
	}PlayerState;

	CPlayer(int nPriority);
	~CPlayer();

	static HRESULT Load(const char *TextureName);
	static void Unload(void);
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	PlayerState GetPlayerState(void) { return m_playerState; }
	void HitPlayer(void);											// �v���C���[�����������Ƃ�����

private:
	void Move(void);												// �ړ�����
	void Shot(void);												// �e���ˏ���
	D3DXVECTOR3 CollitionTile(D3DXVECTOR3 pos,D3DXVECTOR2 size);	// �^�C���Ƃ̂����蔻��
	bool CollitionEnemy(void);										// �G�Ƃ̓����蔻��
	bool CollitionBullet(void);										// �e�Ƃ̓����蔻��
	void CommonProcess(void);										// ���ʏ���
	void StateNormal(void);											// �ʏ��Ԃ̂Ƃ�
	void StateHit(void);											// �I�u�W�F�N�g�Ɠ���������Ԃ̂Ƃ�
	void StateInvincible(void);										// ���G��Ԃ̂Ƃ�

	static LPDIRECT3DTEXTURE9 m_pTexture;							// �e�N�X�`�����̃|�C���^
	D3DXVECTOR3 m_posOld;											// �O��̈ʒu
	D3DXVECTOR3 m_move;												// �ړ���
	PlayerState m_playerState;										// �v���C���[�̏��
	int m_nCntState;												// ��ԃJ�E���^
	int m_nCntDisp;													// �\���J�E���^
};
#endif