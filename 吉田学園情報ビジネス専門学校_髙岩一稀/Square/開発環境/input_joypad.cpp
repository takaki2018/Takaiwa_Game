//=============================================================================
//
// �W���C�p�b�h���� (input_joypad.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#include "input_joypad.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DEADZONE        (2500)      // �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX       (10000)     // �L���͈͂̍ő�l
#define RANGE_MIN       (-10000)    // �L���͈͂̍ŏ��l

#define MAX_DIRECTION	(4)			// �L�[�����̍ő吔

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************	
CInputJoypad::CONTROLLER CInputJoypad::m_Controller[MAX_CONTROLLER] = {};		// �R���g���[���[���̃|�C���^
CInputJoypad::JPINFO CInputJoypad::m_Stick[MAX_CONTROLLER] = {};				// �X�e�B�b�N���
int CInputJoypad::m_nNumController = 0;											// �g�p����R���g���[���[�̃J�E���g

//=============================================================================	
// CManager�̃R���X�g���N�^
//=============================================================================	
CInputJoypad::CInputJoypad()
{

}

//=============================================================================	
// CManager�̃f�X�g���N�^
//=============================================================================	
CInputJoypad::~CInputJoypad()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �C���v�b�g�����̋��ʏ���������
	CInput::Init(hInstance,hWnd);

	// �ϐ��̏�����
	for (int nCntController = 0; nCntController < MAX_CONTROLLER; nCntController++)
	{
		for (int nCntDirection = 0; nCntDirection < MAX_DIRECTION; nCntDirection++)
		{
			m_Stick[nCntController].bLStickPress[nCntDirection] = false;
			m_Stick[nCntController].bRStickPress[nCntDirection] = false;
			m_Stick[nCntController].bStickTrigger[nCntDirection] = false;
			m_Stick[nCntController].bStickOldTrigger[nCntDirection] = false;
			m_Stick[nCntController].bCrossKeyPress[nCntDirection] = false;
			m_Stick[nCntController].bCrossKeyTrigger[nCntDirection] = false;
			m_Stick[nCntController].bCrossKeyOldTrigger[nCntDirection] = false;
		}
	}

	// �Q�[���p�b�h�f�o�C�X�̎擾
	if (FAILED(m_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < m_nNumController; nCnt++)
	{
		// �R���g���[���[���̏�����
		if (!m_Controller[nCnt].pDevjoypad)
		{
			return E_FAIL;
		}

		// �{�^���̏�����
		for (int nCntButton = 0; nCntButton < 13; nCntButton++)
		{
			m_Controller[nCnt].State.rgbButtons[nCntButton] = '\0';
			m_Controller[nCnt].Trigger.rgbButtons[nCntButton] = '\0';
			m_Controller[nCnt].Release.rgbButtons[nCntButton] = '\0';
		}

		// �\���L�[�̏�����
		//for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			m_Controller[nCnt].State.rgdwPOV[0] = -1;
		}

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		if (FAILED(m_Controller[nCnt].pDevjoypad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return E_FAIL;
		}

		// �������[�h�̐ݒ�
		if (FAILED(m_Controller[nCnt].pDevjoypad->SetCooperativeLevel(
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
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RX;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RY;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Z;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z��]�͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RZ;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �E�X�e�B�b�N�̏�����
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = 0;

		// X���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// X���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_RX;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_RY;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z���͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_Z;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z��]�͈̔͂�ݒ�
		dipdw.diph.dwObj = DIJOFS_RZ;
		m_Controller[nCnt].pDevjoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// �A�N�Z�X���̎擾
		m_Controller[nCnt].pDevjoypad->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputJoypad::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nNumController; nCnt++)
	{
		// ���̓f�o�C�X�̔j��
		if (m_Controller[nCnt].pDevjoypad != NULL)
		{
			// �A�N�Z�X���̔j��
			m_Controller[nCnt].pDevjoypad->Unacquire();

			m_Controller[nCnt].pDevjoypad->Release();
			m_Controller[nCnt].pDevjoypad = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputJoypad::Update(void)
{
	// �ϐ��錾
	DIJOYSTATE2 ControllerState;	// �R���g���[���[�̃{�^�����
	HRESULT hr;						// ��������������
	int nCntButton;					// �{�^���̌�

	for (int nCnt = 0; nCnt < m_nNumController; nCnt++)
	{
		if (m_Controller[nCnt].pDevjoypad != NULL)
		{// �W���C�p�b�h���ڑ�����Ă�����|�[�����O�J�n
			hr = m_Controller[nCnt].pDevjoypad->Poll();

			// �f�o�C�X����f�[�^���擾
			if (SUCCEEDED(hr = m_Controller[nCnt].pDevjoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
			{// �X�e�B�b�N�ɏ�����
				LStickPressInfo(nCnt, ControllerState);						// L�X�e�B�b�N�̃v���X����
				RStickPressInfo(nCnt, ControllerState);						// R�X�e�B�b�N�̃v���X����
				StickTriggerInfo(nCnt, ControllerState);					// �X�e�B�b�N�̃g���K�[����
				CrossKeyPressInfo(nCnt, ControllerState);					// �{�^���̃v���X����
				CrossKeyTriggerInfo(nCnt, ControllerState);					// �{�^���̃g���K�[����

				// ���X�e�B�b�N�̏��
				m_Controller[nCnt].State.lY = ControllerState.lY;			// lY�̃v���X
				m_Controller[nCnt].State.lX = ControllerState.lX;			// lX�̃v���X
				m_Controller[nCnt].State.lRx = ControllerState.lRx;			// lRx�̃v���X
				m_Controller[nCnt].State.lRy = ControllerState.lRy;			// lRy�̃v���X
				m_Controller[nCnt].State.lZ = ControllerState.lZ;			// lZ�̃v���X
				m_Controller[nCnt].State.lRz = ControllerState.lRz;			// lRz�̃v���X

				for (nCntButton = 0; nCntButton < 13; nCntButton++)
				{// �g���K�[���̕ۑ�
					m_Controller[nCnt].Trigger.rgbButtons[nCntButton] =
						(m_Controller[nCnt].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					// �����[�X���̕ۑ�
					m_Controller[nCnt].Release.rgbButtons[nCntButton] =
						(ControllerState.rgbButtons[nCntButton] ^ m_Controller[nCnt].State.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					// �v���X���̕ۑ�
					m_Controller[nCnt].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
				}
			}
			else
			{// �A�N�Z�X���̎擾
				m_Controller[nCnt].pDevjoypad->Acquire();
			}
		}
	}
}

//=============================================================================
// L�X�e�B�b�N�̃v���X���
//=============================================================================
void CInputJoypad::LStickPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bLStickPress[STICK_AND_CROSSKEY_MAX] = { false,false,false,false };

	if (State.lY >= 7000)
	{// ��
		bLStickPress[STICK_AND_CROSSKEY_UP] = true;
	}
	else if (State.lY <= -7000)
	{// �E
		bLStickPress[STICK_AND_CROSSKEY_DOWN] = true;
	}
	else if (State.lX >= 7000)
	{// ��
		bLStickPress[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.lX <= -7000)
	{// �E
		bLStickPress[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < STICK_AND_CROSSKEY_MAX; nCntPress++)
	{// �v���X���
		m_Stick[nCnt].bLStickPress[nCntPress] = bLStickPress[nCntPress];
	}
}

//=============================================================================
// R�X�e�B�b�N�̃v���X���
//=============================================================================
void CInputJoypad::RStickPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bRStickPress[STICK_AND_CROSSKEY_MAX] = { false,false,false,false };

	if (State.lRz <= -7000)
	{// ��
		bRStickPress[STICK_AND_CROSSKEY_UP] = true;
	}
	else if (State.lRz >= 7000)
	{// �E
		bRStickPress[STICK_AND_CROSSKEY_DOWN] = true;
	}
	else if (State.lZ >= 7000)
	{// ��
		bRStickPress[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.lZ <= -7000)
	{// �E
		bRStickPress[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < STICK_AND_CROSSKEY_MAX; nCntPress++)
	{// �v���X���
		m_Stick[nCnt].bRStickPress[nCntPress] = bRStickPress[nCntPress];
	}
}

//=============================================================================
// �X�e�B�b�N�̃g���K�[���
//=============================================================================
void CInputJoypad::StickTriggerInfo(int nCnt, DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bStickTrigger[STICK_AND_CROSSKEY_MAX] = { false,false,false,false };

	if (State.lY <= -7000)
	{// ��
		bStickTrigger[STICK_AND_CROSSKEY_UP] = true;
	}
	else if (State.lY >= 7000)
	{// ��
		bStickTrigger[STICK_AND_CROSSKEY_DOWN] = true;
	}
	else if (State.lX >= 7000)
	{// ��
		bStickTrigger[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.lX <= -7000)
	{// �E
		bStickTrigger[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntTrigger = 0; nCntTrigger < STICK_AND_CROSSKEY_MAX; nCntTrigger++)
	{// �g���K�[���
		m_Stick[nCnt].bStickTrigger[nCntTrigger] = (m_Stick[nCnt].bStickOldTrigger[nCntTrigger] ^ bStickTrigger[nCntTrigger]) & bStickTrigger[nCntTrigger];

		// ����ۑ�
		m_Stick[nCnt].bStickOldTrigger[nCntTrigger] = bStickTrigger[nCntTrigger];
	}
}

//=============================================================================
// �\���L�[�̃v���X���
//=============================================================================
void CInputJoypad::CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bCrossKeyPress[STICK_AND_CROSSKEY_MAX] = { false,false ,false,false };

	if (State.rgdwPOV[0] >= 3000 && State.rgdwPOV[0] <= 15000)
	{// ��
		bCrossKeyPress[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.rgdwPOV[0] >= 21000 && State.rgdwPOV[0] <= 33000)
	{// �E
		bCrossKeyPress[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntPress = 0; nCntPress < STICK_AND_CROSSKEY_MAX; nCntPress++)
	{// �v���X���
		m_Stick[nCnt].bCrossKeyPress[nCntPress] = bCrossKeyPress[nCntPress];
	}
}

//=============================================================================
// �\���L�[�̃g���K�[���
//=============================================================================
void CInputJoypad::CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State)
{
	// �ϐ��̏�����
	bool bCrossKeyTrigger[STICK_AND_CROSSKEY_MAX] = { false,false,false,false };

	if ((State.rgdwPOV[0] >= 0 && State.rgdwPOV[0] <= 6000) || (State.rgdwPOV[0] >= 30000 && State.rgdwPOV[0] < 36000))
	{// ��
		bCrossKeyTrigger[STICK_AND_CROSSKEY_UP] = true;
	}
	else if (State.rgdwPOV[0] >= 12000 && State.rgdwPOV[0] <= 24000)
	{// ��
		bCrossKeyTrigger[STICK_AND_CROSSKEY_DOWN] = true;
	}
	else if (State.rgdwPOV[0] >= 3000 && State.rgdwPOV[0] <= 15000)
	{// ��
		bCrossKeyTrigger[STICK_AND_CROSSKEY_RIGHT] = true;
	}
	else if (State.rgdwPOV[0] >= 21000 && State.rgdwPOV[0] <= 33000)
	{// �E
		bCrossKeyTrigger[STICK_AND_CROSSKEY_LEFT] = true;
	}

	for (int nCntTrigger = 0; nCntTrigger < STICK_AND_CROSSKEY_MAX; nCntTrigger++)
	{// �g���K�[���
		m_Stick[nCnt].bCrossKeyTrigger[nCntTrigger] = (m_Stick[nCnt].bCrossKeyOldTrigger[nCntTrigger] ^ bCrossKeyTrigger[nCntTrigger]) & bCrossKeyTrigger[nCntTrigger];

		// ����ۑ�
		m_Stick[nCnt].bCrossKeyOldTrigger[nCntTrigger] = bCrossKeyTrigger[nCntTrigger];
	}
}

//=============================================================================
// �{�^�����͏��̎擾
//=============================================================================
bool CInputJoypad::GetJoypadPress(int nCnt, int nButton)
{// �v���X���̊m�F
	return (m_Controller[nCnt].State.rgbButtons[nButton] & 0x80) ? true : false;
}

bool CInputJoypad::GetJoypadTrigger(int nCnt, int nButton)
{// �g���K�[���̊m�F
	return (m_Controller[nCnt].Trigger.rgbButtons[nButton] & 0x80) ? true : false;
}

bool CInputJoypad::GetJoypadReleace(int nCnt, int nButton)
{// �����[�X���̊m�F
	return (m_Controller[nCnt].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �X�e�B�b�N���̎擾
//=============================================================================
DIJOYSTATE2 CInputJoypad::GetControllerPless(int nCnt)
{// �v���X���
	return m_Controller[nCnt].State;
}

bool *CInputJoypad::GetLStickPress(int nCnt)
{// L�X�e�B�b�N�̃v���X���
	return &m_Stick[nCnt].bLStickPress[0];
}

bool *CInputJoypad::GetRStickPress(int nCnt)
{// R�X�e�B�b�N�̃v���X���̎擾
	return &m_Stick[nCnt].bRStickPress[0];
}

bool *CInputJoypad::GetStickTrigger(int nCnt)
{// �g���K�[���
	return &m_Stick[nCnt].bStickTrigger[0];
}

//=============================================================================
// �\���L�[���̎擾
//=============================================================================
bool *CInputJoypad::GetCrossKeyPress(int nCnt)
{// �v���X���
	return &m_Stick[nCnt].bCrossKeyPress[0];
}

bool *CInputJoypad::GetCrossKeyTrigger(int nCnt)
{// �g���K�[���
	return &m_Stick[nCnt].bCrossKeyTrigger[0];
}

//=============================================================================
// �f�o�C�X�������Ɏ��s�����
//=============================================================================
BOOL CALLBACK CInputJoypad::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	// �ϐ��錾
	HRESULT hr;

	// �W���C�p�b�h���ڑ�����Ă��邩
	hr = m_pInput->CreateDevice(pdidInstance->guidInstance, &m_Controller[m_nNumController].pDevjoypad, NULL);

	// �ڑ�����Ă���R���g���[���[�̃J�E���g
	m_nNumController++;

	// ������
	return DIENUM_CONTINUE;
}

//=============================================================================
// �ڑ����ꂽ�R���g���[���[�̃J�E���g
//=============================================================================
int CInputJoypad::GetnCntController(void)
{
	return m_nNumController;
}