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
#define MAX_DATA	(256)
#define PLAYER_1	(0)			// �v���C���[1

//-------------------------------------------------------------------
// �L�[����̏��
//-------------------------------------------------------------------
typedef enum
{
	JPINFO_SHOOT = 0,		// �e����
	JPINFO_COLOR_CHANGE,	// �e�̐F�ύX		
	JPINFO_OKorJAMP,		// ���肩�W�����v
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
}JOYPADINFO;

//-------------------------------------------------------------------
// �X�e�B�b�N�̃v���X���
//-------------------------------------------------------------------
typedef enum
{
	STICKPRESS_UP = 0,	// ��
	STICKPRESS_DOWN,		// ��
	STICKPRESS_RIGHT,		// �E
	STICKPRESS_LEFT,		// ��
	STICKPRESS_MAX,
}STICKPRESS;	

//-------------------------------------------------------------------
// �X�e�B�b�N�̃g���K�[���
//-------------------------------------------------------------------
typedef enum
{
	STICKTRIGGER_UP = 0,		// ��
	STICKTRIGGER_DOWN,			// ��
	STICKTRIGGER_RIGHT,			// �E
	STICKTRIGGER_LEFT,			// ��
	STICKTRIGGER_MAX,
}STICKTRIGGER;	

//-------------------------------------------------------------------
// �\���L�[�̃v���X���
//-------------------------------------------------------------------
typedef enum
{
	CROSSKEYPRESS_UP = 0,	// ��
	CROSSKEYPRESS_DOWN,		// ��
	CROSSKEYPRESS_RIGHT,	// �E
	CROSSKEYPRESS_LEFT,		// ��
	CROSSKEYPRESS_MAX,
}CROSSKEYPRESS;

//-------------------------------------------------------------------
// �\���L�[�̃g���K�[���
//-------------------------------------------------------------------
typedef enum
{
	CROSSKEYTRIGGER_UP = 0,		// ��
	CROSSKEYTRIGGER_DOWN,		// ��
	CROSSKEYTRIGGER_RIGHT,		// �E
	CROSSKEYTRIGGER_LEFT,		// ��
	CROSSKEYTRIGGER_MAX,
}CROSSKEYTRIGGER;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);										// �Q�[���p�b�h�̏���������
void UninitGamepad(void);																	// �Q�[���p�b�h�̏I������
void UpdateGamepad(void);																	// �Q�[���p�b�h�̍X�V����

void StickPressInfo(int nCnt,DIJOYSTATE2 State);											// �X�e�B�b�N�̃v���X����
void StickTriggerInfo(int nCnt, DIJOYSTATE2 State);											// �X�e�B�b�N�̃g���K�[����

void CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State);										// �{�^���̃v���X����
void CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State);										// �{�^���̃g���K�[����

bool GetJoypadPress(int nCnt,int nButton);													// �{�^���̃v���X���̎擾
bool GetJoypadTrigger(int nCnt, int nButton);												// �{�^���̃g���K�[���̎擾
bool GetJoypadReleace(int nCnt, int nButton);												// �{�^���̃����[�X���̎擾

DIJOYSTATE2 GetControllerPless(int nCnt);													// �R���g���[���[�̃v���X���̎擾
bool *GetStickPress(int nCnt);																// �X�e�B�b�N�̃v���X���̎擾
bool *GetStickTrigger(int nCnt);															// �X�e�B�b�N�̃g���K�[���̎擾

bool *GetCrossKeyPress(int nCnt);															// �{�^���̃v���X���̎擾
bool *GetCrossKeyTrigger(int nCnt);															// �{�^���̃g���K�[���̎擾

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// �R�[���o�b�N�֐�
																							
int GetnCntController(void);																// �ڑ�����Ă���R���g���[���[���̎擾

#endif
