//-----------------------------------------------------------------
//
// Xinput (Xinput_gamepad.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "Xinput_gamepad.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
XINPUT_STATE g_state;					// Xinput�R���g���[���[�̏��
XinputGamepad g_XinputGamepad;			// �Q�[���p�b�h���

//-----------------------------------------------------------------
// �Q�[���p�b�h���̓ǂݍ���
//-----------------------------------------------------------------
void LoadGamepad(void)
{
	// �ϐ��̏�����
	g_XinputGamepad.bPressA = false;
	g_XinputGamepad.bPressLStick = false;
	g_XinputGamepad.fAngle = 0.0f;

	// ���̏�����
	ZeroMemory(&g_state, sizeof(XINPUT_STATE));

	// Simply get the state of the controller from XInput.
	DWORD dwResult = XInputGetState(0, &g_state);

	if (dwResult == ERROR_SUCCESS)
	{
		// �{�^����������Ă��邩�ǂ���
		XinputButtons();

		// �X�e�B�b�N����
		XinputSticks();
	}
	else
	{
		// �Q�[���p�b�h�̍X�V����
		UpdateGamepad();
	}
}

//-----------------------------------------------------------------
// �{�^������
//-----------------------------------------------------------------
void XinputButtons(void)
{
	if (g_state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{// A�{�^���������ꂽ��true�ɐݒ�
		g_XinputGamepad.bPressA = true;
	}
}

//-----------------------------------------------------------------
// �X�e�B�b�N����
//-----------------------------------------------------------------
void XinputSticks(void)
{
	// �f�b�h�]�[���̐ݒ�
	if ((g_state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		g_state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(g_state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			g_state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		g_state.Gamepad.sThumbLX = 0;
		g_state.Gamepad.sThumbLY = 0;
	}

	// JOYPAD�̃X�e�B�b�N����
	if (g_state.Gamepad.sThumbLX != 0 || g_state.Gamepad.sThumbLY != 0)
	{// �X�e�B�b�N�̓��͊p�x
		g_XinputGamepad.fAngle = atan2f((float)g_state.Gamepad.sThumbLX, (float)g_state.Gamepad.sThumbLY * -1);
	
		// �X�e�B�b�N���X�������Ƃ�`����
		g_XinputGamepad.bPressLStick = true;
	}
}

//-----------------------------------------------------------------
// �{�^���������ꂽ���ǂ����擾
//-----------------------------------------------------------------
XinputGamepad *GetXinputGamepad(void)
{
	return &g_XinputGamepad;
}