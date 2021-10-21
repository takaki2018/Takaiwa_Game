//=============================================================================
//
// �L�[�{�[�h���� [input_keyboard.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DATA							(256)												// �ő�f�[�^��
#define KEY_LookAtPoint_LEFT				(CInputKeyboard::KEYINFO_LookAtPoint_LEFT)			// �����_����]
#define KEY_LookAtPoint_RIGHT				(CInputKeyboard::KEYINFO_LookAtPoint_RIGHT)			// �����_�E��]
#define KEY_Point_LEFT						(CInputKeyboard::KEYINFO_Point_LEFT)				// ���_����]
#define KEY_Point_RIGHT						(CInputKeyboard::KEYINFO_Point_RIGHT)				// ���_�E��]
#define KEY_MOVE_UP							(CInputKeyboard::KEYINFO_UP)						// ��ړ�
#define KEY_MOVE_DOWN						(CInputKeyboard::KEYINFO_DOWN)						// ���ړ�
#define KEY_MOVE_RIGHT						(CInputKeyboard::KEYINFO_RIGHT)						// �E�ړ�
#define KEY_MOVE_LEFT						(CInputKeyboard::KEYINFO_LEFT)						// ���ړ�
#define KEY_MOVE_LookAtPoint_UP				(CInputKeyboard::KEYINFO_LookAtPoint_UP)			// �����_��ړ�
#define KEY_MOVE_LookAtPoint_DOWN			(CInputKeyboard::KEYINFO_LookAtPoint_DOWN)			// �����_���ړ�
#define KEY_MOVE_Point_UP					(CInputKeyboard::KEYINFO_Point_UP)					// ���_��ړ�
#define KEY_MOVE_Point_DOWN					(CInputKeyboard::KEYINFO_Point_DOWN)				// ���_���ړ�

#define KEY_ADD_ALPHA						(CInputKeyboard::KEYINFO_ADD_ALPHA)					// ���l����
#define KEY_DECREASE_ALPHA					(CInputKeyboard::KEYINFO_DECREASE_ALPHA)			// ���l����
#define KEY_ADD_SIZE						(CInputKeyboard::KEYINFO_ADD_SIZE)					// �T�C�Y����
#define KEY_DECREASE_SIZE					(CInputKeyboard::KEYINFO_DECREASE_SIZE)				// �T�C�Y����
#define KEY_RESET							(CInputKeyboard::KEYINFO_LCORL)						// ���Z�b�g

//*****************************************************************************
// �L�[�{�[�h�N���X(�h���N���X)
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	// �L�[���͏��
	typedef enum
	{
		KEYINFO_OK = 0,				// ����
		KEYINFO_UP,					// ��I��
		KEYINFO_DOWN,				// ���I��
		KEYINFO_RIGHT,				// �E�ړ�
		KEYINFO_LEFT,				// ���ړ�
		KEYINFO_PAUSE,				// �|�[�Y
		KEYINFO_SHOT_UP,			// ������ɐi�ޒe�̔���
		KEYINFO_SHOT_DOWN,			// �������ɐi�ޒe�̔���
		KEYINFO_SHOT_RIGHT,			// �E�����ɐi�ޒe�̔���
		KEYINFO_SHOT_LEFT,			// �������ɐi�ޒe�̔���
		KEYINFO_LookAtPoint_LEFT,	// �����_����]
		KEYINFO_LookAtPoint_RIGHT,	// �����_�E��]
		KEYINFO_Point_LEFT,			// ���_����]
		KEYINFO_Point_RIGHT,		// ���_�E��]
		KEYINFO_LookAtPoint_UP,		// �����_��ړ�
		KEYINFO_LookAtPoint_DOWN,	// �����_���ړ�
		KEYINFO_Point_UP,			// ���_��ړ�
		KEYINFO_Point_DOWN,			// ���_���ړ�
		KEYINFO_LCORL,				// ���Z�b�g
		KEYINFO_ADD_ALPHA,			// ���l����
		KEYINFO_DECREASE_ALPHA,		// ���l����
		KEYINFO_ADD_SIZE,			// �T�C�Y����
		KEYINFO_DECREASE_SIZE,		// �T�C�Y����
		KEYINFO_MAX,
	}KEYINFO;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetReleace(int nKey);

private:
	BYTE m_aKeyState[MAX_DATA];				// �L�[���
	BYTE m_aKeyStateTrigger[MAX_DATA];		// �L�[�g���K-���
	BYTE m_aKeyStateReleace[MAX_DATA];		// �L�[�����[�X���

	int m_aKeyInfo[KEYINFO_MAX] =
	{
		DIK_RETURN,		// ����
		DIK_W,			// ��I��
		DIK_S,			// ���I�� 
		DIK_D,			// �E�ړ�
		DIK_A,			// ���ړ�
		DIK_P,			// �|�[�Y
		DIK_UP,			// ������ɐi�ޒe�̔���
		DIK_DOWN,		// �������ɐi�ޒe�̔���
		DIK_RIGHT,		// �E�����ɐi�ޒe�̔���
		DIK_LEFT,		// �������ɐi�ޒe�̔���
		DIK_Q,			// �����_����]
		DIK_E,			// �����_�E��]
		DIK_Z,			// ���_����]
		DIK_C,			// ���_�E��]
		DIK_T,			// �����_��ړ�
		DIK_B,			// �����_���ړ�
		DIK_R,			// ���_��ړ�
		DIK_V,			// ���_���ړ�
		DIK_LCONTROL,	// ���R���g���[��
		DIK_0,			// ���l����
		DIK_9,			// ���l����
		DIK_6,			// �T�C�Y����
		DIK_5,			// �T�C�Y����
	};
};
#endif