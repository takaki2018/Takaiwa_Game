//=============================================================================
//
// �G�̔���UI���� [spawn_enemyUI.h]
// Author : 
//
//=============================================================================
#ifndef _SPAWNENEMYUI_H_
#define _SPAWNENEMYUI_H_

#include "scene2D.h"

//-----------------------------------------------------------------------------
// �G�����N���X(�h���N���X)
//------------------ ----------------------------------------------------------
class CSpawnEnemyUI : public CScene2D
{
public:
	CSpawnEnemyUI(int nPriority);
	~CSpawnEnemyUI();
	static HRESULT Load(void);
	static void Unload(void);
	static CSpawnEnemyUI *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI,int nCntSpawnInterval,int nEnemyType);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, int nCntSpawnInterval, int nEnemyType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Blinking(void);							// �_�ŏ���
	void CreateEnemy(void);							// �G�̐���
	static LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`�����̃|�C���^
	int m_nCntSpawnInterval;						// �G����������܂ł̎���
	int m_nCounter;									// �J�E���^�[
	float m_fMoveAlpha;								// ���l�ړ��̊�l
	int m_nCntFlash;								// �_�ŉ�
	int m_nEnemyType;								// �G�̎�ގ��ʗp
	float m_fMoveAI;								// ��{�ړ���
};
#endif