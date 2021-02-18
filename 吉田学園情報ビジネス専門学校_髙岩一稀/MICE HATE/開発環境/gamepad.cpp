//-------------------------------------------------------------------
//
// �Q�[���p�b�h (gamepad.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "gamepad.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define DEADZONE        2500            // �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX       10000           // �L���͈͂̍ő�l
#define RANGE_MIN       -10000          // �L���͈͂̍ŏ��l

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevjoypad = NULL;	// �W���C�p�b�h�̃f�o�C�X���
	DIJOYSTATE2 State;						// �v���X
	DIJOYSTATE2 Trigger;					// �g���K�[
	DIJOYSTATE2 Release;					// �����[�X
}CONTROLLER;								// �R���g���[���[�̏��

//-------------------------------------------------------------------
// �O���[�o���ϐ�
//-------------------------------------------------------------------
LPDIRECTINPUT8 g_pInputGamepad = NULL;	// Dinput�̃|�C���^
CONTROLLER g_Controller;				// �R���g���[���[���̃|�C���^
bool g_bTrigger[4];						// �X�e�B�b�N��|������
bool g_bStick[4];

//-------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// �ϐ��̏�����
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		g_bTrigger[nCnt] = false;
		g_bStick[nCnt] = false;
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

	// �R���g���[���[���̏�����
	if (!g_Controller.pDevjoypad)
	{
		return E_FAIL;
	}

	// �{�^���̏�����
	for (int nCntButton = 0; nCntButton < 13; nCntButton++)
	{
		g_Controller.State.rgbButtons[nCntButton] = '\0';
		g_Controller.Trigger.rgbButtons[nCntButton] = '\0';
		g_Controller.Release.rgbButtons[nCntButton] = '\0';
	}

	// �\���L�[�̏�����
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		g_Controller.State.rgdwPOV[nCnt] = -1;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(g_Controller.pDevjoypad->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	// �������[�h�̐ݒ�
	if (FAILED(g_Controller.pDevjoypad->SetCooperativeLevel(
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
	g_Controller.pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Y���͈̔͂�ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	g_Controller.pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Z���͈̔͂�ݒ�
	diprg.diph.dwObj = DIJOFS_Z;
	g_Controller.pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Z��]�͈̔͂�ݒ�
	diprg.diph.dwObj = DIJOFS_RZ;
	g_Controller.pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);


	// �E�X�e�B�b�N�̏�����
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = DEADZONE;

	// X���͈̔͂�ݒ�
	dipdw.diph.dwObj = DIJOFS_X;
	g_Controller.pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	// Y���͈̔͂�ݒ�
	dipdw.diph.dwObj = DIJOFS_Y;
	g_Controller.pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	// Z���͈̔͂�ݒ�
	dipdw.diph.dwObj = DIJOFS_Z;
	g_Controller.pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	// Z��]�͈̔͂�ݒ�
	dipdw.diph.dwObj = DIJOFS_RZ;
	g_Controller.pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	// �A�N�Z�X���̎擾
	g_Controller.pDevjoypad->Acquire();

	return S_OK;
}

//-------------------------------------------------------------------
// �I������
//-------------------------------------------------------------------
void UninitGamepad(void)
{
	// ���̓f�o�C�X�̔j��
	if (g_Controller.pDevjoypad != NULL)
	{
		// �A�N�Z�X���̔j��
		g_Controller.pDevjoypad->Unacquire();

		g_Controller.pDevjoypad->Release();
		g_Controller.pDevjoypad = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (g_Controller.pDevjoypad != NULL)
	{
		g_Controller.pDevjoypad->Release();
		g_Controller.pDevjoypad = NULL;
	}
}

//-------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------
void UpdateGamepad(void)
{
	// �ϐ��錾
	DIJOYSTATE2 ControllerState;	// �R���g���[���[�̃{�^�����
	HRESULT hr;						// ��������������
	int nCntButton;					// �{�^���̌�

	if (g_Controller.pDevjoypad != NULL)
	{// �W���C�p�b�h���ڑ�����Ă�����|�[�����O�J�n
		hr = g_Controller.pDevjoypad->Poll();

		// �f�o�C�X����f�[�^���擾
		if (SUCCEEDED(hr = g_Controller.pDevjoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
		{// �X�e�B�b�N�ɏ�����
			// ���X�e�B�b�N�̃g���K�[���
			JoypadTriggerInfo(ControllerState);
		
			// ���X�e�B�b�N�̏��
			g_Controller.State.lY = ControllerState.lY;			// lY�̃v���X
			g_Controller.State.lX = ControllerState.lX;			// lX�̃v���X
			g_Controller.State.lZ = ControllerState.lZ;			// lZ�̃v���X
			g_Controller.State.lRz = ControllerState.lRz;		// lRz�̃v���X

			for (int nCnt = 0; nCnt < 3; nCnt++) 
			{// �v���X���̕ۑ�
				g_Controller.State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];
			}

			for (nCntButton = 0; nCntButton < 13; nCntButton++)
			{// �g���K�[���̕ۑ�
				g_Controller.Trigger.rgbButtons[nCntButton] =
					(g_Controller.State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
			
				// �����[�X���̕ۑ�
				g_Controller.Release.rgbButtons[nCntButton] = 
					(ControllerState.rgbButtons[nCntButton] ^ g_Controller.State.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
			
				// �v���X���̕ۑ�
				g_Controller.State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
			}
		}
		else
		{// �A�N�Z�X���̎擾
			g_Controller.pDevjoypad->Acquire();
		}
	}
}

//-------------------------------------------------------------------
// �X�e�B�b�N�̃g���K�[���
//-------------------------------------------------------------------
void JoypadTriggerInfo(DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bTrigger[4] = {false,false,false,false};

	if (State.lY <= -7000)
	{// ��
		bTrigger[0] = true;
	}
	else if (State.lY >= 7000)
	{// ��
		bTrigger[1] = true;
	}
	else if (State.lX >= 7000)
	{// ��
		bTrigger[2] = true;
	}
	else if (State.lX <= -7000)
	{// �E
		bTrigger[3] = true;
	}

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{// �g���K�[���
		g_bStick[nCnt] = (g_bTrigger[nCnt] ^ bTrigger[nCnt]) & bTrigger[nCnt];

		g_bTrigger[nCnt] = bTrigger[nCnt];
	}
}

//-------------------------------------------------------------------
// ���͏��̎擾
//-------------------------------------------------------------------
bool GetJoypadPress(int nButton)
{// �v���X���̊m�F
	return (g_Controller.State.rgbButtons[nButton] & 0x80) ? true : false;
}

bool GetJoypadTrigger(int nButton)
{// �g���K�[���̊m�F
	return (g_Controller.Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

bool GetJoypadReleace(int nButton)
{// �����[�X���̊m�F
	return (g_Controller.Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//-------------------------------------------------------------------
// �W���C�p�b�h���̎擾
//-------------------------------------------------------------------
DIJOYSTATE2 GetControllerPless(void)
{// �v���X���
	return g_Controller.State;
}

bool *GetControllerTrigger(void)
{// �g���K�[���
	return &g_bStick[0];
}

//-------------------------------------------------------------------
// �f�o�C�X�������Ɏ��s�����
//-------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	// �ϐ��錾
	HRESULT hr;

	// �W���C�p�b�h���ڑ�����Ă��邩
	hr = g_pInputGamepad->CreateDevice(pdidInstance->guidInstance, &g_Controller.pDevjoypad, NULL);

	// ������
	return DIENUM_CONTINUE;
}