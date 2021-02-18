//-------------------------------------------------------------------
//
// �C���v�b�g�w�b�_�[ (input.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_

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
	KEYINFO_OK = 0,					// ����
	KEYINFO_UP,						// ��I��
	KEYINFO_DOWN,					// ���I��
	KEYINFO_RIGHT,					// �E�ړ�
	KEYINFO_LEFT,					// ���ړ�
	KEYINFO_ROT_CAMERA_RIGHT,		// ���_�E��]
	KEYINFO_ROT_CAMERA_LEFT,		// ���_����]
	KEYINFO_ROT_RIGHT,				// �����_�E��]
	KEYINFO_ROT_LEFT,				// �����_����]
	KEYINFO_ROT_CAMERA_UP,			// ���_�E��]
	KEYINFO_ROT_CAMERA_DOWN,		// ���_����]
	KEYINFO_ROT_UP,					// �����_�E��]
	KEYINFO_ROT_DOWN,				// �����_����]
	KEYINFO_MODEL_UP,				// ��I��
	KEYINFO_MODEL_DOWN,				// ���I��
	KEYINFO_MODEL_RIGHT,			// �E�ړ�
	KEYINFO_MODEL_LEFT,				// ���ړ�
	KEYINFO_MODEL_RIGHT_TURN,		// �E��]
	KEYINFO_MODEL_LEFT_TURN,		// ����]
	KEYINFO_PAUSE,					// �|�[�Y
	KEYINFO_SHOOT,					// �e�̔���
	KEYINFO_MAX,
}KEYINFO;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardReleace(int nKey);

#endif