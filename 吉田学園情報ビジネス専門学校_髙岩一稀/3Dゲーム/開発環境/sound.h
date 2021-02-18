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
	SOUND_LABEL_RESULT_CLEAR,	// �N���ABGM
	SOUND_LABEL_RESULT_OVER,	// ���sBGM
	SOUND_LABEL_RANKING,		// �����L���O
	SOUND_LABEL_SE_HIT,			// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_SHOOT,		// ���ˉ�
	SOUND_LABEL_SE_DOWN,		// �I��(��)
	SOUND_LABEL_SE_PAUSE_IN,	// �I��(��)
	SOUND_LABEL_SE_PAUSE_OUT,	// �I��(��)
	SOUND_LABEL_SE_PLAYER_BRAEK,// �I��(��)
	SOUND_LABEL_SE_ITEM,		// �I��(��)
	SOUND_LABEL_SE_OK,			// �I��(��)
	SOUND_LABEL_SE_ALARM,		// �x��
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
