//=============================================================================
//
// �V�[������ [scene.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���_�t�H�[�}�b�g
#define MAX_VERTEX (4)												// ���_�̐�

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef  struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;
} VERTEX_3D;

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CScene
{
public:
	// �I�u�W�F�N�g�̎��
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_BULLET,			// �e
		OBJTYPE_ENEMY,			// �G
		OBJTYPE_SCORE,			// �X�R�A
		OBJTYPE_BG,				// �w�i
		OBJTYPE_EXPLOSION,		// ����
		OBJTYPE_TILE,			// �^�C��
		OBJTYPE_LIFE,			// ���C�t
		OBJTYPE_SPAWNENEMYUI,	// �G�����pUI
		OBJTYPE_LOGO,			// ���S
		OBJTYPE_COUNTDOWN,		// �J�E���g�_�E��
		OBJTYPE_GAMECLEAR,		// �Q�[���N���A
		OBJTYPE_EFFECT,			// �G�t�F�N�g
		OBJTYPE_TUTORIAL,		// �`���[�g���A��
		OBJTYPE_PAUSE,			// �|�[�Y
		OBJTYPE_MODEL,			// ���f��
		OBJTYPE_MAX,	
	}OBJTYPE;			

	// �`��̗D�揇��
	typedef enum
	{
		PRIORITY_NONE = 0,		// �Ȃ�
		PRIORITY_BG,			// �w�i
		PRIORITY_MODEL,			// ���f��
		PRIORITY_TILE,			// �^�C��
		PRIORITY_TILEREVIVAL,	// �^�C�������G�t�F�N�g
		PRIORITY_EFFECT,		// �G�t�F�N�g
		PRIORITY_BULLET,		// �e
		PRIORITY_PLAYER,		// �v���C���[
		PRIORITY_ENEMY,			// �G
		PRIORITY_UI,			// UI
		PRIORITY_COUNTDOWN,		// �J�E���g�_�E��
		PRIORITY_TUTORIAL,		// �`���[�g���A��
		PRIORITY_PAUSE,			// �|�[�Y
		PRIORITY_MAX,			
	}PRIORITY;

	CScene();
	CScene(int nPriolity);
	virtual ~CScene();
	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	OBJTYPE GetObjType(void) { return m_objType; }
	void SetObjType(OBJTYPE objType) { m_objType = objType; }
	CScene *GetScene(int nPriority) { return m_pTop[nPriority]; }
	CScene *GetScene(int nPriority,int nCnt);
	CScene *GetNext(void) { return m_pNext; }
	bool GetDeath(void) { return m_bDeath; }

protected:
	void Release(void);

private:
	static void UpdateTitle(void);			// �^�C�g���ł̍X�V����
	static void UpdateGame(void);			// �Q�[���ł̍X�V����
	static void UpdateResult(void);			// ���U���g�ł̍X�V����
	static void ReleaseScene(void);			// �j������
	OBJTYPE m_objType;						// �I�u�W�F�N�g�̎��
	static CScene *m_pTop[PRIORITY_MAX];	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene *m_pCur[PRIORITY_MAX];	// ����(�Ō��)�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_pPrev;						// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_pNext;						// ���̃I�u�W�F�N�g�ւ̃|�C���^
	bool m_bDeath;							// ���S�t���O
};
#endif