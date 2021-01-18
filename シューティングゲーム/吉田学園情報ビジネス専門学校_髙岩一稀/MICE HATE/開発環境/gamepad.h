//-------------------------------------------------------------------
//
// �Q�[���p�b�h�w�b�_�[ (gamepad.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include "main.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_DATA (256)

//-------------------------------------------------------------------
// �L�[����̏��
//-------------------------------------------------------------------
typedef enum
{
	JPINFO_SHOOT = 0,		// �e����
	JPINFO_COLOR_CHANGE,	// �e�̐F�ύX		
	JPINFO_OK,				// ����
	JPINFO_ITEM,			// �A�C�e���g�p
	JPINFO_LEFT_TURN,		// ����]
	JPINFO_RIGHT_TURN,		// �E��]
	JPINFO_UP,				// ��
	JPINFO_DOWN,			// ��
	JPINFO_LS,				// �g�p���Ȃ�
	JPINFO_RS,				// �g�p���Ȃ�
	JPINFO_BACK,			// �g�p���Ȃ�
	JPINFO_PAUSE,			// �|�[�Y
	JPINFO_GUIDE			// �g�p���Ȃ�
}JOYPADINFO;				// JOYPAD�̃L�[���

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);										// �Q�[���p�b�h�̏���������
void UninitGamepad(void);																	// �Q�[���p�b�h�̏I������
void UpdateGamepad(void);																	// �Q�[���p�b�h�̍X�V����
void JoypadTriggerInfo(DIJOYSTATE2 State);													// ���X�e�B�b�N�̃g���K�[���̕ۑ�
bool GetJoypadPress(int nButton);															// �{�^���̃v���X���
bool GetJoypadTrigger(int nButton);															// �{�^���̃g���K�[���
bool GetJoypadReleace(int nButton);															// �{�^���̃����[�X���
bool *GetControllerTrigger(void);															// ���X�e�B�b�N�̃g���K�[���
DIJOYSTATE2 GetControllerPless(void);														// �X�e�B�b�N�̃v���X���	
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// �R�[���o�b�N�֐�


#endif
