//=============================================================================
//
// �Q�[����ʏ��� [game.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;				// �v���C���[�N���X
class CUI;					// UI�N���X
class CSpawnEnemy;			// �G�����N���X
class CGameClear;			// �Q�[���N���A�N���X
class CModel;				// ���f���N���X

//-----------------------------------------------------------------------------
// �Q�[����ʃN���X(�h���N���X)
//-----------------------------------------------------------------------------
class CGame : public CScene
{
public:
	CGame();
	~CGame()
		;
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *GetPlayer(void);
	static CUI *GetUI(void);
	static CModel *GetModel(void) { return m_pModel; }

private:
	void TileFrameEffectCreate(void);			// �^�C���t���[���G�t�F�N�g�̐���
	static CPlayer *m_pPlayer;					// �v���C���[���̃|�C���^
	static CUI *m_pUI;							// UI���̃|�C���^
	static CSpawnEnemy  *m_pSpawnEnemy;			// �G�������̃|�C���^
	static CGameClear *m_pGameClear;			// �Q�[���N���A���̃|�C���^
	static CModel *m_pModel;					// ���f�����̃|�C���^
	static int m_nCntTileFrameEffect;			// �^�C���t���[���G�t�F�N�g���o���J�E���^�[
};
#endif