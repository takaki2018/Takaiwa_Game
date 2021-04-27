//==============================================================================================================
//
// �Q�[���p�b�h (gamepad.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "gamepad.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_CONTROLLER	(1)			// �ڑ��\�ȃR���g���[���[�̍ő吔
#define DEADZONE        (2500)      // �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX       (10000)     // �L���͈͂̍ő�l
#define RANGE_MIN       (-10000)    // �L���͈͂̍ŏ��l

#define MAX_DIRECTION	(4)			// �L�[�����̍ő吔

//--------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevjoypad = NULL;	// �W���C�p�b�h�̃f�o�C�X���
	DIJOYSTATE2 State;						// �v���X
	DIJOYSTATE2 Trigger;					// �g���K�[
	DIJOYSTATE2 Release;					// �����[�X
}CONTROLLER;								// �R���g���[���[�̏��

typedef struct
{
	bool bLStickPress[LSTICKPRESS_MAX];				// L�X�e�B�b�N�̃v���X���
	bool bRStickPress[RSTICKPRESS_MAX];				// R�X�e�B�b�N�̃v���X���
	bool bStickTrigger[STICKTRIGGER_MAX];			// �X�e�B�b�N�̃g���K�[���
	bool bStickOldTrigger[STICKTRIGGER_MAX];		// �X�e�B�b�N�̃g���K�[����ۑ�
	bool bCrossKeyPress[CROSSKEYPRESS_MAX];			// �\���L�[�̃v���X���
	bool bCrossKeyTrigger[CROSSKEYTRIGGER_MAX];		// �\���L�[�̃g���K�[���
	bool bCrossKeyOldTrigger[CROSSKEYTRIGGER_MAX];	// �\���L�[�̃g���K�[����ۑ�
}JPINFO;											// �R���g���[���[���

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECTINPUT8 g_pInputGamepad = NULL;					// Dinput�̃|�C���^
CONTROLLER g_Controller[MAX_CONTROLLER];				// �R���g���[���[���̃|�C���^
int g_nCntController = 0;								// �g�p����R���g���[���[�̃J�E���g
JPINFO stick[MAX_CONTROLLER];							// �X�e�B�b�N���

//--------------------------------------------------------------------------------------------------------------
// ����������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// �ϐ��̏�����
	for (int nCntController = 0; nCntController < MAX_CONTROLLER; nCntController++)
	{
		for (int nCntDirection = 0; nCntDirection < MAX_DIRECTION; nCntDirection++)
		{
			stick[nCntController].bLStickPress[nCntDirection] = false;
			stick[nCntController].bRStickPress[nCntDirection] = false;
			stick[nCntController].bStickTrigger[nCntDirection] = false;
			stick[nCntController].bStickOldTrigger[nCntDirection] = false;
			stick[nCntController].bCrossKeyPress[nCntDirection] = false;
			stick[nCntController].bCrossKeyTrigger[nCntDirection] = false;
			stick[nCntController].bCrossKeyOldTrigger[nCntDirection] = false;
		}
	}

	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInputGamepad,
		NULL)))
	{
		return E_FAIL;

	}
	// �Q�[���p�b�h�f�o�C�X�̎擾
	if (FAILED(g_pInputGamepad->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < g_nCntController; nCnt++)
	{
		// �R���g���[���[���̏�����
		if (!g_Controller[nCnt].pDevjoypad)
		{
			return E_FAIL;
		}

		// �{�^���̏�����
		for (int nCntButton = 0; nCntButton < 13; nCntButton++)
		{
			g_Controller[nCnt].State.rgbButtons[nCntButton] = '\0';
			g_Controller[nCnt].Trigger.rgbButtons[nCntButton] = '\0';
			g_Controller[nCnt].Release.rgbButtons[nCntButton] = '\0';
		}

		// �\���L�[�̏�����
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			g_Controller[nCnt].State.rgdwPOV[0] = -1;
		}

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		if (FAILED(g_Controller[nCnt].pDevjoypad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return E_FAIL;
		}

		// �������[�h�̐ݒ�
		if (FAILED(g_Controller[nCnt].pDevjoypad->SetCooperativeLevel(
			hWnd,
			(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		// ���X�e�B�b�N�̏�����
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;

		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RX;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RY;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Z;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z��]�͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RZ;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �E�X�e�B�b�N�̏�����
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = 0;

		// X���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// X���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_RX;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_RY;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_Z;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z��]�͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_RZ;
		g_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// �A�N�Z�X���̎擾
		g_Controller[nCnt].pDevjoypad->Acquire();
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitGamepad(void)
{
	for (int nCnt = 0; nCnt < g_nCntController; nCnt++)
	{
		// ���̓f�o�C�X�̔j��
		if (g_Controller[nCnt].pDevjoypad != NULL)
		{
			// �A�N�Z�X���̔j��
			g_Controller[nCnt].pDevjoypad->Unacquire();

			g_Controller[nCnt].pDevjoypad->Release();
			g_Controller[nCnt].pDevjoypad = NULL;
		}
	}
	// DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInputGamepad != NULL)
	{
		g_pInputGamepad->Release();
		g_pInputGamepad = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateGamepad(void)
{
	// �ϐ��錾
	DIJOYSTATE2 ControllerState;	// �R���g���[���[�̃{�^�����
	HRESULT hr;						// ��������������
	int nCntButton;					// �{�^���̌�

	for (int nCnt = 0; nCnt < g_nCntController; nCnt++)
	{
		if (g_Controller[nCnt].pDevjoypad != NULL)
		{// �W���C�p�b�h���ڑ�����Ă�����|�[�����O�J�n
			hr = g_Controller[nCnt].pDevjoypad->Poll();

			// �f�o�C�X����f�[�^���擾
			if (SUCCEEDED(hr = g_Controller[nCnt].pDevjoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
			{// �X�e�B�b�N�ɏ�����
				LStickPressInfo(nCnt,ControllerState);						// L�X�e�B�b�N�̃v���X����
				RStickPressInfo(nCnt, ControllerState);						// R�X�e�B�b�N�̃v���X����
				StickTriggerInfo(nCnt, ControllerState);					// �X�e�B�b�N�̃g���K�[����
				CrossKeyPressInfo(nCnt, ControllerState);					// �{�^���̃v���X����
				CrossKeyTriggerInfo(nCnt, ControllerState);					// �{�^���̃g���K�[����

				// ���X�e�B�b�N�̏��
				g_Controller[nCnt].State.lY = ControllerState.lY;			// lY�̃v���X
				g_Controller[nCnt].State.lX = ControllerState.lX;			// lX�̃v���X
				g_Controller[nCnt].State.lRx = ControllerState.lRx;			// lRx�̃v���X
				g_Controller[nCnt].State.lRy = ControllerState.lRy;			// lRy�̃v���X
				g_Controller[nCnt].State.lZ = ControllerState.lZ;			// lZ�̃v���X
				g_Controller[nCnt].State.lRz = ControllerState.lRz;			// lRz�̃v���X

				for (nCntButton = 0; nCntButton < 13; nCntButton++)
				{// �g���K�[���̕ۑ�
					g_Controller[nCnt].Trigger.rgbButtons[nCntButton] =
						(g_Controller[nCnt].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					// �����[�X���̕ۑ�
					g_Controller[nCnt].Release.rgbButtons[nCntButton] =
						(ControllerState.rgbButtons[nCntButton] ^ g_Controller[nCnt].State.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					// �v���X���̕ۑ�
					g_Controller[nCnt].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
				}
			}
			else
			{// �A�N�Z�X���̎擾
				g_Controller[nCnt].pDevjoypad->Acquire();
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// L�X�e�B�b�N�̃v���X���
// ����		�FnCnt	- ���Ԗڂ̃R���g���[���[��
//			�FState	- �R���g���[���[���
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void LStickPressInfo(int nCnt,DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bLStickPress[LSTICKPRESS_MAX] = { false,false,false,false};

	if (State.lY >= 7000)
	{// ��
		bLStickPress[LSTICKPRESS_UP] = true;
	}
	else if (State.lY <= -7000)
	{// �E
		bLStickPress[LSTICKPRESS_DOWN] = true;
	}
	else if (State.lX >= 7000)
	{// ��
		bLStickPress[LSTICKPRESS_RIGHT] = true;
	}
	else if (State.lX <= -7000)
	{// �E
		bLStickPress[LSTICKPRESS_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < LSTICKPRESS_MAX; nCntPress++)
	{// �v���X���
		stick[nCnt].bLStickPress[nCntPress] = bLStickPress[nCntPress];
	}
}

//--------------------------------------------------------------------------------------------------------------
// R�X�e�B�b�N�̃v���X���
// ����		�FnCnt	- ���Ԗڂ̃R���g���[���[��
//			�FState	- �R���g���[���[���
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void RStickPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bRStickPress[LSTICKPRESS_MAX] = { false,false,false,false };

	if (State.lRz <= -7000)
	{// ��
		bRStickPress[LSTICKPRESS_UP] = true;
	}
	else if (State.lRz >= 7000)
	{// �E
		bRStickPress[LSTICKPRESS_DOWN] = true;
	}
	else if (State.lZ >= 7000)
	{// ��
		bRStickPress[LSTICKPRESS_RIGHT] = true;
	}
	else if (State.lZ <= -7000)
	{// �E
		bRStickPress[LSTICKPRESS_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < LSTICKPRESS_MAX; nCntPress++)
	{// �v���X���
		stick[nCnt].bRStickPress[nCntPress] = bRStickPress[nCntPress];
	}
}

//--------------------------------------------------------------------------------------------------------------
// �X�e�B�b�N�̃g���K�[���
// ����		�FnCnt	- ���Ԗڂ̃R���g���[���[��
//			�FState	- �R���g���[���[���
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void StickTriggerInfo(int nCnt, DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bStickTrigger[STICKTRIGGER_MAX] = {false,false,false,false};

	if (State.lY <= -7000)
	{// ��
		bStickTrigger[STICKTRIGGER_UP] = true;
	}
	else if (State.lY >= 7000)
	{// ��
		bStickTrigger[STICKTRIGGER_DOWN] = true;
	}
	else if (State.lX >= 7000)
	{// ��
		bStickTrigger[STICKTRIGGER_RIGHT] = true;
	}
	else if (State.lX <= -7000)
	{// �E
		bStickTrigger[STICKTRIGGER_LEFT] = true;
	}

	for (int nCntTrigger = 0; nCntTrigger < STICKTRIGGER_MAX; nCntTrigger++)
	{// �g���K�[���
		stick[nCnt].bStickTrigger[nCntTrigger] = (stick[nCnt].bStickOldTrigger[nCntTrigger] ^ bStickTrigger[nCntTrigger]) & bStickTrigger[nCntTrigger];

		// ����ۑ�
		stick[nCnt].bStickOldTrigger[nCntTrigger] = bStickTrigger[nCntTrigger];
	}
}

//--------------------------------------------------------------------------------------------------------------
// �\���L�[�̃v���X���
// ����		�FnCnt	- ���Ԗڂ̃R���g���[���[��
//			�FState	- �R���g���[���[���
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bCrossKeyPress[LSTICKPRESS_MAX] = { false,false ,false,false};

	if (State.rgdwPOV[0] >= 3000 && State.rgdwPOV[0] <= 15000)
	{// ��
		bCrossKeyPress[LSTICKPRESS_RIGHT] = true;
	}
	else if (State.rgdwPOV[0] >= 21000 && State.rgdwPOV[0] <= 33000)
	{// �E
		bCrossKeyPress[LSTICKPRESS_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < LSTICKPRESS_MAX; nCntPress++)
	{// �v���X���
		stick[nCnt].bCrossKeyPress[nCntPress] = bCrossKeyPress[nCntPress];
	}
}

//--------------------------------------------------------------------------------------------------------------
// �\���L�[�̃g���K�[���
// ����		�FnCnt	- ���Ԗڂ̃R���g���[���[��
//			�FState	- �R���g���[���[���
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bCrossKeyTrigger[STICKTRIGGER_MAX] = { false,false,false,false };

	if ((State.rgdwPOV[0] >= 0 && State.rgdwPOV[0] <= 6000) || (State.rgdwPOV[0] >= 30000 && State.rgdwPOV[0] < 36000))
	{// ��
		bCrossKeyTrigger[STICKTRIGGER_UP] = true;
	}
	else if (State.rgdwPOV[0] >= 12000 && State.rgdwPOV[0] <= 24000)
	{// ��
		bCrossKeyTrigger[STICKTRIGGER_DOWN] = true;
	}
	else if (State.rgdwPOV[0] >= 3000 && State.rgdwPOV[0] <= 15000)
	{// ��
		bCrossKeyTrigger[STICKTRIGGER_RIGHT] = true;
	}
	else if (State.rgdwPOV[0] >= 21000 && State.rgdwPOV[0] <= 33000)
	{// �E
		bCrossKeyTrigger[STICKTRIGGER_LEFT] = true;
	}

	for (int nCntTrigger = 0; nCntTrigger < STICKTRIGGER_MAX; nCntTrigger++)
	{// �g���K�[���
		stick[nCnt].bCrossKeyTrigger[nCntTrigger] = (stick[nCnt].bCrossKeyOldTrigger[nCntTrigger] ^ bCrossKeyTrigger[nCntTrigger]) & bCrossKeyTrigger[nCntTrigger];

		// ����ۑ�
		stick[nCnt].bCrossKeyOldTrigger[nCntTrigger] = bCrossKeyTrigger[nCntTrigger];
	}
}

//--------------------------------------------------------------------------------------------------------------
// �{�^�����͏��̎擾
// ����		�FnCnt		- ���Ԗڂ̃R���g���[���[��
//			�FnButton	- �ǂ̃{�^������������
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
bool GetJoypadPress(int nCnt, int nButton)
{// �v���X���̊m�F
	return (g_Controller[nCnt].State.rgbButtons[nButton] & 0x80) ? true : false;
}

bool GetJoypadTrigger(int nCnt, int nButton)
{// �g���K�[���̊m�F
	return (g_Controller[nCnt].Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

bool GetJoypadReleace(int nCnt, int nButton)
{// �����[�X���̊m�F
	return (g_Controller[nCnt].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//--------------------------------------------------------------------------------------------------------------
// �X�e�B�b�N���̎擾
// ����		�FnCnt		- ���Ԗڂ̃R���g���[���[��
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
DIJOYSTATE2 GetControllerPless(int nCnt)
{// �v���X���
	return g_Controller[nCnt].State;
}

bool *GetLStickPress(int nCnt)
{// L�X�e�B�b�N�̃v���X���
	return &stick[nCnt].bLStickPress[0];
}

bool *GetRStickPress(int nCnt)
{// R�X�e�B�b�N�̃v���X���̎擾
	return &stick[nCnt].bRStickPress[0];
}

bool *GetStickTrigger(int nCnt)
{// �g���K�[���
	return &stick[nCnt].bStickTrigger[0];
}

//--------------------------------------------------------------------------------------------------------------
// �\���L�[���̎擾
// ����		�FnCnt		- ���Ԗڂ̃R���g���[���[��
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
bool *GetCrossKeyPress(int nCnt)
{// �v���X���
	return &stick[nCnt].bCrossKeyPress[0];
}

bool *GetCrossKeyTrigger(int nCnt)
{// �g���K�[���
	return &stick[nCnt].bCrossKeyTrigger[0];
}

//--------------------------------------------------------------------------------------------------------------
// �f�o�C�X�������Ɏ��s�����
//--------------------------------------------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	// �ϐ��錾
	HRESULT hr;

	// �W���C�p�b�h���ڑ�����Ă��邩
	hr = g_pInputGamepad->CreateDevice(pdidInstance->guidInstance, &g_Controller[g_nCntController].pDevjoypad, NULL);

	// �ڑ�����Ă���R���g���[���[�̃J�E���g
	g_nCntController++;

	// ������
	return DIENUM_CONTINUE;
}

//--------------------------------------------------------------------------------------------------------------
// �ڑ����ꂽ�R���g���[���[�̃J�E���g
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fint	- �ڑ�����Ă���R���g���[���[����n��
//--------------------------------------------------------------------------------------------------------------
int GetnCntController(void)
{
	return g_nCntController;
}