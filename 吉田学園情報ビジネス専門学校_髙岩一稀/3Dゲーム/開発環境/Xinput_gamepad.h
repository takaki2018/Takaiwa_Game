//-------------------------------------------------------------------
//
// Xinput�w�b�_�[ (Xinput_gamepad.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _XINPUTGAMEPAD_H_
#define _XINPUTGAMEPAD_H_

#include "main.h"

//-------------------------------------------------------------------
// �񋓌^
//-------------------------------------------------------------------
typedef enum
{
	XINPUTSTATE_SHOOT = 0,
	XINPUTSTATE_MAX,
}XinputState;

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	bool bPressA;					// A�{�^����������Ă邩�ǂ���
	bool bPressAold;				// �O��A�{�^����������Ă������ǂ���
	bool bPressLStick;				// ���X�e�B�b�N���X�������ǂ���
	float fAngle;					// ���X�e�B�b�N�̓��͊p�x
}XinputGamepad;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
void LoadGamepad(void);						// �Q�[���p�b�h���̓ǂݍ���
void XinputButtons(void);					// �{�^������
void XinputSticks(void);					// �X�e�B�b�N����
XinputGamepad *GetXinputGamepad(void);		// �Q�[���p�b�h�����擾

#endif