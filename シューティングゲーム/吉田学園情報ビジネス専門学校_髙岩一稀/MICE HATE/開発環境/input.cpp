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
LPDIRECTINPUT8 g_pInput = NULL;					// �C���v�b�g���̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		// �f�o�C�X�̂��C���^
BYTE g_aKeyState[MAX_DATA];						// �v���X���
BYTE g_aKeyStateTrigger[MAX_DATA];				// �g���K�[���
BYTE g_aKeyStateReleace[MAX_DATA];				// �����[�X���

int g_aKeyInfo[] =								// �L�[�{�[�h����ꗗ
{
	DIK_RETURN,		// ����
	DIK_W,			// ��ړ�
	DIK_S,			// ���ړ�
	DIK_D,			// �E�ړ�
	DIK_A,			// ���ړ�
	DIK_E,			// �E��]
	DIK_Q,			// ����]
	DIK_SPACE,		// �V���b�g
	DIK_K,			// �e�̐F�ύX
	DIK_H,			// �A�C�e��
	DIK_P			// �|�[�Y
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
		{// ���̕ۑ�
			// �g���K�[���
			g_aKeyStateTrigger[nKey] = (g_aKeyState[nKey] & aKeyState[nKey]) ^ aKeyState[nKey];

			// �����[�X���
			g_aKeyStateReleace[nKey] = (g_aKeyState[nKey] | aKeyState[nKey]) ^ aKeyState[nKey];

			// �v���X���
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
{// �v���X���
	return (g_aKeyState[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool GetKeyboardTrigger(int nKey)
{// �g���K�[���
	return (g_aKeyStateTrigger[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}

bool GetKeyboardReleace(int nKey)
{// �����[�X���
	return (g_aKeyStateReleace[g_aKeyInfo[nKey]] & 0x80) ? true : false;
}