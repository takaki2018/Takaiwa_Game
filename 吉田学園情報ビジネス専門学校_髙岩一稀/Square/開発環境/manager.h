//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

#include <stdlib.h>
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GET_KEYBOARD		(CManager::GetInputKeyboard())		// �L�[���̎擾

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;			// �����_���N���X
class CInputKeyboard;		// �L�[�{�[�h�N���X
class CInputJoypad;			// �W���C�p�b�h�N���X
class CSound;				// �T�E���h�N���X
class CPlayer;				// �v���C���[�N���X
class CScore;				// �X�R�A�N���X
class CLife;				// ���C�t�N���X
class CTitle;				// �^�C�g���N���X
class CTutorial;			// �`���[�g���A���N���X
class CGame;				// �Q�[���N���X
class CResult;				// ���U���g�N���X
class CFade;				// �t�F�[�h�N���X
class CGameDataKeepObject;	// �Q�[���f�[�^�ۑ��p�I�u�W�F�N�g�N���X
class CCamera;				// �J�����N���X
class CLight;				// ���C�g�N���X

//*****************************************************************************
// �}�l�[�W���[�N���X(��{�N���X)
//*****************************************************************************
class CManager
{
public:
	// ���[�h�̎��
	typedef enum
	{
		MODE_TITLE = 0,		// �^�C�g��
		MODE_GAME,			// �Q�[��
		MODE_RESULT,		// ���U���g
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadTexture(void);
	void UnloadTexture(void);
	static void SetMode(MODE mode);

	static MODE GetMode(void) { return m_mode; }
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
	static CFade *GetFade(void) { return m_pFade; }
	static CGameDataKeepObject *GetGameData(void) { return m_pGemeData; }
	static CSound *GetSound(void) { return m_pSound; }
	static bool GetPause(void) { return m_bPause; }
	static void SetPause(void);
	static void SetPause(bool bPause) { m_bPause = bPause; }
	static bool GetTutorial(void) { return m_bTutorial; }
	static void SetTutorial(bool bTutorial) { m_bTutorial = bTutorial; }
	static CCamera *GetCamera(void) { return m_pCamera; }

private:
	static CRenderer *m_pRenderer;				// �����_�����̃|�C���^
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h���̃|�C���^
	static CInputJoypad *m_pInputJoypad;		// �W���C�p�b�h���̃|�C���^
	static CSound *m_pSound;					// �T�E���h���̃|�C���^
	static CTitle *m_pTitle;					// �^�C�g�����̃|�C���^
	static CTutorial *m_pTutorial;				// �`���[�g���A�����̃|�C���^
	static CGame *m_pGame;						// �Q�[�����̃|�C���^
	static CResult *m_pResult;					// ���U���g���̃|�C���^
	static MODE m_mode;							// ���[�h���
	static CFade *m_pFade;						// �t�F�[�h���̃|�C���^
	static bool m_bPause;						// �|�[�Y���邩�ǂ���
	static bool m_bTutorial;					// �`���[�g���A����ʂ��ǂ���
	static CGameDataKeepObject *m_pGemeData;	// �Q�[���f�[�^�ۑ��p�I�u�W�F�N�g���̃|�C���^
	static CCamera *m_pCamera;					// �J�������̃|�C���^
	static CLight *m_pLight;					// ���C�g���̃|�C���^
};
#endif