//=============================================================================
//
// �W���C�p�b�h���� [input_joypad.h]
// Author : 
//
//=============================================================================
#ifndef _INPUTJOYPAD_H_
#define _INPUTJOYPAD_H_

#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DATA		(256)		// �ő�f�[�^��
#define MAX_CONTROLLER	(1)			// �ڑ��\�ȃR���g���[���[�̍ő吔
#define PLAYER_1		(0)			// �v���C���[1

//-----------------------------------------------------------------------------
// �W���C�p�b�h�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CInputJoypad : public CInput
{
public:
	typedef enum
	{
		JPINFO_SHOT_LEFT = 0,	// �������ɐi�ޒe������
		JPINFO_SHOT_UP,			// ������ɐi�ޒe������	
		JPINFO_SHOT_DOWN,		// �������ɐi�ޒe������
		JPINFO_SHOT_RIGHT,		// �E�����ɐi�ޒe������
		JPINFO_LB,				// �g�p���Ȃ�
		JPINFO_RB,				// �g�p���Ȃ�
		JPINFO_LT,				// �g�p���Ȃ�
		JPINFO_RT,				// �g�p���Ȃ�
		JPINFO_LS,				// �g�p���Ȃ�
		JPINFO_RS,				// �g�p���Ȃ�
		JPINFO_BACK,			// �g�p���Ȃ�
		JPINFO_PAUSE,			// �|�[�Y
		JPINFO_GUIDE			// �g�p���Ȃ�
	}JOYPADINFO;

	typedef enum
	{
		LSTICKPRESS_UP = 0,	// ��
		LSTICKPRESS_DOWN,		// ��
		LSTICKPRESS_RIGHT,		// �E
		LSTICKPRESS_LEFT,		// ��
		LSTICKPRESS_MAX,
	}LSTICKPRESS;

	typedef enum
	{
		RSTICKPRESS_UP = 0,		// ��
		RSTICKPRESS_DOWN,		// ��
		RSTICKPRESS_RIGHT,		// �E
		RSTICKPRESS_LEFT,		// ��
		RSTICKPRESS_MAX,
	}RSTICKPRESS;

	typedef enum
	{
		STICKTRIGGER_UP = 0,		// ��
		STICKTRIGGER_DOWN,			// ��
		STICKTRIGGER_RIGHT,			// �E
		STICKTRIGGER_LEFT,			// ��
		STICKTRIGGER_MAX,
	}STICKTRIGGER;

	typedef enum
	{
		CROSSKEYPRESS_UP = 0,	// ��
		CROSSKEYPRESS_DOWN,		// ��
		CROSSKEYPRESS_RIGHT,	// �E
		CROSSKEYPRESS_LEFT,		// ��
		CROSSKEYPRESS_MAX,
	}CROSSKEYPRESS;

	typedef enum
	{
		CROSSKEYTRIGGER_UP = 0,		// ��
		CROSSKEYTRIGGER_DOWN,		// ��
		CROSSKEYTRIGGER_RIGHT,		// �E
		CROSSKEYTRIGGER_LEFT,		// ��
		CROSSKEYTRIGGER_MAX,
	}CROSSKEYTRIGGER;

	CInputJoypad();
	~CInputJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	void LStickPressInfo(int nCnt, DIJOYSTATE2 State);											// L�X�e�B�b�N�̃v���X����
	void RStickPressInfo(int nCnt, DIJOYSTATE2 State);											// R�X�e�B�b�N�̃v���X����
	void StickTriggerInfo(int nCnt, DIJOYSTATE2 State);											// �X�e�B�b�N�̃g���K�[����

	void CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State);										// �{�^���̃v���X����
	void CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State);										// �{�^���̃g���K�[����

	bool GetJoypadPress(int nCnt, int nButton);													// �{�^���̃v���X���̎擾
	bool GetJoypadTrigger(int nCnt, int nButton);												// �{�^���̃g���K�[���̎擾
	bool GetJoypadReleace(int nCnt, int nButton);												// �{�^���̃����[�X���̎擾

	DIJOYSTATE2 GetControllerPless(int nCnt);													// �R���g���[���[�̃v���X���̎擾
	bool *GetLStickPress(int nCnt);																// L�X�e�B�b�N�̃v���X���̎擾
	bool *GetRStickPress(int nCnt);																// R�X�e�B�b�N�̃v���X���̎擾
	bool *GetStickTrigger(int nCnt);															// �X�e�B�b�N�̃g���K�[���̎擾

	bool *GetCrossKeyPress(int nCnt);															// �{�^���̃v���X���̎擾
	bool *GetCrossKeyTrigger(int nCnt);															// �{�^���̃g���K�[���̎擾

	int GetnCntController(void);

private:
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// �R�[���o�b�N�֐�

	struct CONTROLLER	// �R���g���[���[�̏��
	{
		LPDIRECTINPUTDEVICE8 pDevjoypad = NULL;	// �W���C�p�b�h�̃f�o�C�X���
		DIJOYSTATE2 State;						// �v���X
		DIJOYSTATE2 Trigger;					// �g���K�[
		DIJOYSTATE2 Release;					// �����[�X
	};								

	struct JPINFO	// �R���g���[���[�̑�����
	{
		bool bLStickPress[LSTICKPRESS_MAX];				// L�X�e�B�b�N�̃v���X���
		bool bRStickPress[RSTICKPRESS_MAX];				// R�X�e�B�b�N�̃v���X���
		bool bStickTrigger[STICKTRIGGER_MAX];			// �X�e�B�b�N�̃g���K�[���
		bool bStickOldTrigger[STICKTRIGGER_MAX];		// �X�e�B�b�N�̃g���K�[����ۑ�
		bool bCrossKeyPress[CROSSKEYPRESS_MAX];			// �\���L�[�̃v���X���
		bool bCrossKeyTrigger[CROSSKEYTRIGGER_MAX];		// �\���L�[�̃g���K�[���
		bool bCrossKeyOldTrigger[CROSSKEYTRIGGER_MAX];	// �\���L�[�̃g���K�[����ۑ�
	};											

	static CONTROLLER m_Controller[MAX_CONTROLLER];			// �R���g���[���[���̃|�C���^
	static JPINFO m_Stick[MAX_CONTROLLER];					// �X�e�B�b�N���
	static int m_nNumController;							// �g�p����R���g���[���[�̃J�E���g
};
#endif