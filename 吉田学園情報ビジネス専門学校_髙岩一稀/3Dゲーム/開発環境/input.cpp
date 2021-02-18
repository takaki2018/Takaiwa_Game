//-------------------------------------------------------------------
//
// �C���v�b�g�t�@�C�� (input.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "input.h"

//-------------------------------------------------------------------
// �O���[�o���ϐ�
//-------------------------------------------------------------------
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[MAX_DATA];
BYTE g_aKeyStateTrigger[MAX_DATA];
BYTE g_aKeyStateReleace[MAX_DATA];
BYTE g_aKeyStateRepeat[MAX_DATA];

int g_aKeyInfo[] =
{
	DIK_RETURN,		// ����
	DIK_W,			// ��I��
	DIK_S,			// ���I��
	DIK_D,			// �E�ړ�
	DIK_A,			// ���ړ�
	DIK_E,			// ���_�E��]
	DIK_Q,			// ���_����]
	DIK_Z,			// �����_�E��]
	DIK_C,			// �����_����]
	DIK_T,			// ���_��ړ�
	DIK_B,			// ���_���ړ�
	DIK_R,			// �����_��ړ�
	DIK_V,			// �����_���ړ�
	DIK_UP,			// ��I��
	DIK_DOWN,		// ���I��
	DIK_RIGHT,		// �E�ړ�
	DIK_LEFT,		// ���ړ�
	DIK_RSHIFT,		// ���f���E��]
	DIK_LSHIFT,		// ���f������]
	DIK_P,			// �|�[�Y
	DIK_SPACE,		// �e�̔���
};

//-------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X�̐���
	if (FAILED(g_pInput->CreateDevice(
		GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h�̐ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �A�N�Z�X���̎擾
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//-------------------------------------------------------------------
// �I������
//-------------------------------------------------------------------
void UninitKeyboard(void)
{
	// ���̓f�o�C�X�̔j��
	if (g_pDevKeyboard != NULL)
	{
		// �A�N�Z�X���̔j��
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//-------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------
void UpdateKeyboard(void)
{
	// �ϐ��錾
	BYTE aKeyState[MAX_DATA];

	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{	
		for (int nKey = 0; nKey < MAX_DATA; nKey++)
		{
			g_aKeyStateTrigger[nKey] = (g_aKeyState[nKey] & aKeyState[nKey]) ^ aKeyState[nKey];

			g_aKeyStateReleace[nKey] = (g_aKeyState[nKey] | aKeyState[nKey]) ^ aKeyState[nKey];

			g_aKeyState[nKey] = aKeyState[nKey]; 
		}
	}
	else
	{// �A�N�Z�X���̎擾
		g_pDevKeyboard->Acquire();
	}
}

//-------------------------------------------------------------------
// �L�[�{�[�h���̎擾
//-------------------------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool GetKeyboardReleace(int nKey)
{
	return (g_aKeyStateReleace[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}