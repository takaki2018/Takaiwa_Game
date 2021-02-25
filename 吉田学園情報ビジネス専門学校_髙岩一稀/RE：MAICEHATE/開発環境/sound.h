//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE = 0,		// �^�C�g��
	SOUND_LABEL_OE,				// �`���[�g���A��
	SOUND_LABEL_BATTLE,			// �o�g��
	SOUND_LABEL_RESULT,			// ���U���gBGM
	SOUND_LABEL_RANKING,		// �����L���O
	SOUND_LABEL_SE_JUMP,		// �W�����v��
	SOUND_LABEL_SE_STOMP,		// ���މ�
	SOUND_LABEL_SE_ENEMY,		// �G�ɓ��������Ƃ�
	SOUND_LABEL_SE_DAMAGE,		// �_���[�W���󂯂��Ƃ�
	SOUND_LABEL_SE_GETCOIN,		// �R�C���l��
	SOUND_LABEL_SE_DOWN,		// �I��(��)
	SOUND_LABEL_SE_UP,			// �I��(��)
	SOUND_LABEL_SE_PAUSE_IN,	// �|�[�Y���(��)
	SOUND_LABEL_SE_PAUSE_OUT,	// �|�[�Y���(�o)
	SOUND_LABEL_SE_OK,			// ����
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
