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
	KEYINFO_OK = 0,			// ����
	KEYINFO_UP,				// ��ړ�
	KEYINFO_DOWN,			// ���ړ�
	KEYINFO_RIGHT,			// �E�ړ�
	KEYINFO_LEFT,			// ���ړ�
	KEYINFO_RIGHT_TURN,		// �E��]
	KEYINFO_LEFT_TURN,		// ����]
	KEYINFO_SHOOT,			// �V���b�g
	KEYINFO_COLOR_CHANGE,	// �e�̐F�ύX
	KEYINFO_ITEM,			// �A�C�e��
	KEYINFO_PAUSE,			// �|�[�Y
	KEYINFO_MAX,
}KEYINFO;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);		// �L�[�{�[�h�̏���������
void UninitKeyboard(void);									// �L�[�{�[�h�̏I������
void UpdateKeyboard(void);									// �L�[�{�[�h�̍X�V����
bool GetKeyboardPress(int nKey);							// �v���X���
bool GetKeyboardTrigger(int nKey);							// �g���K�[���
bool GetKeyboardReleace(int nKey);							// �����[�X���

#endif