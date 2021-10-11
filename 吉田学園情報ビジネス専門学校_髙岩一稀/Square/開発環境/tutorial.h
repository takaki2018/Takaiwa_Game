//=============================================================================
//
// ���[�h��ʃw�b�_�[ (tutorial.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CURSOR	(TUTORIALUI_MAX - 1)		// �ő�J�[�\����
#define MAX_ARROW	(2)							// �y�[�W����p���

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;			// �|���S��2D�N���X
class CLogo;			// ���S�N���X

//*****************************************************************************
// �`���[�g���A����ʃN���X(�h���N���X)
//*****************************************************************************
class CTutorial : public CScene
{
public:
	// �`���[�g���A���Ŏg�p����e�N�X�`���̎�ށ@
	typedef enum
	{
		TEXTURE_NONE = 0,	// �`���[�g���A���w�i
		TEXTURE_KEYBOARD,	// �L�[�{�[�h����
		TEXTURE_GAMEPAD,	// �Q�[���p�b�h����
		TEXTURE_GAMESCREEN,	// �Q�[�����
		TEXTURE_GAME,		// �Q�[�����e
		TEXTURE_CURSOR,		// �J�[�\��
		TEXTURE_ARROW,		// ���
		TEXTURE_MAX,
	}TutorialTexture;

	// �`���[�g���A���Ŏg�p����UI�̎�ށ@
	typedef enum
	{
		TUTORIALUI_NONE = 0,	// �`���[�g���A���w�i
		TUTORIALUI_KEYBOARD,	// �L�[�{�[�h����
		TUTORIALUI_GAMEPAD,		// �Q�[���p�b�h����
		TUTORIALUI_GAMESCREEN,	// �Q�[�����
		TUTORIALUI_GAME,		// �Q�[�����e
		TUTORIALUI_MAX,
	}TutorialUI;

	CTutorial(int nPriolity);
	~CTutorial();

	static HRESULT Load(void);
	static void Unload(void);
	static CTutorial *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURE_MAX];	// �`���[�g���A����ʂ̃e�N�X�`���̃|�C���^
	CScene2D *m_apTutorial[TUTORIALUI_MAX];				// �`���[�g���A�����̃|�C���^
	CScene2D *m_apCursor[MAX_CURSOR];					// �J�[�\�����̃|�C���^
	CScene2D *m_apArrow[MAX_ARROW];						// �y�[�W����p�����̃|�C���^
	CLogo *m_pLogo;										// ���S�̃|�C���^
	int m_nCursor;										// �J�[�\���̈ʒu
};

#endif
