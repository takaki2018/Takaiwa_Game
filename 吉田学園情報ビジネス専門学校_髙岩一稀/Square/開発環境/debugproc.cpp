//=============================================================================
//
// �f�o�b�N�\������ [debugproc.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "debugproc.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPD3DXFONT CDebugProc::m_pFont = NULL;		// �t�H���g�̃|�C���^
char CDebugProc::m_aStr[1024] = {};			// ���������i�[����o�b�t�@

//=============================================================================
// CDebugProc�̃R���X�g���N�^
//=============================================================================
CDebugProc::CDebugProc()
{
	
}

//=============================================================================
// CDebugProc�̃f�X�g���N�^
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
// ����������
//=============================================================================
void CDebugProc::Init(void)
{
	// �t�H���g�̐���
	D3DXCreateFont(CManager::GetRenderer()->GetDevice(), 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	// �o�b�t�@�̃N���A
	memset(&m_aStr[0], 0, 1024);
}

//=============================================================================
// �I������
//=============================================================================
void CDebugProc::Uninit(void)
{
	// �t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDebugProc::Draw(void)
{
	// ������z�u����ʒu�̌���
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// �t�H���g�̕\��
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0, 0));

	// �o�b�t�@�̃N���A
	memset(&m_aStr[0], 0, 1024);
}

//=============================================================================
// �f�o�b�N�\���p�̕����ǉ�����
// ����		: *fmt	- �\��������������
//			: ...	- �ϒ�����
// �Ԃ�l	: void	- �����Ԃ��Ȃ�
//=============================================================================
void CDebugProc::Print(const char *fmt, ...)
{
	// �ϐ��錾
	va_list arg;
	char aStr[256];
	char aWard[64];

	// ������
	memset(aStr, 0, 256);
	memset(aWard, 0, 64);

	// ������̎��ʂ��J�n
	va_start(arg, fmt);

	// �����񂪍Ō�ɂȂ�܂ő�����
	for (int nCnt = 0; *fmt != '\0'; fmt++)
	{
		if (*fmt != '%')
		{
			// ������%�ȊO�̂Ƃ�������ۑ�����
			wsprintf(&aWard[0], "%c", *fmt);
		}
		else
		{
			// ������%�������Ƃ��|�C���^��i�߂�
			fmt++;

			// �������Ƃɏ�����ς���
			switch (*fmt)
			{
				// d�������Ƃ������ɓ����Ă���int�^�̕ϐ��̒l�ɕς���
			case 'd':
				wsprintf(&aWard[0], "%d", va_arg(arg, int));
				break;

				// f�������Ƃ������ɓ����Ă���double�^�̕ϐ��̒l�ɕς���
			case 'f':
				sprintf(&aWard[0], "%.2f", va_arg(arg, double));
				break;

				// s�������Ƃ������ɓ����Ă���const char*�^�̕ϐ��̒l�ɕς���
			case 's':
				wsprintf(&aWard[0], "%s", va_arg(arg, const char*));
				break;

				// c�������Ƃ������ɓ����Ă���char�^�̕ϐ��̒l�ɕς���
			case 'c':
				wsprintf(&aWard[0], "%c", va_arg(arg, char));
				break;

				// ��������L�ȊO�̂Ƃ�������ۑ�����
			default:
				wsprintf(&aWard[0], "%c", *fmt);
				break;
			}
		}
		// �������ۑ��������J�E���g����
		nCnt += wsprintf(&aStr[nCnt], "%s",&aWard[0]);
	}
	// ���ʂ��I����
	va_end(arg);

	// �������A��������
	strcat(m_aStr, aStr);
}