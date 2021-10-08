//=============================================================================
//
// �f�o�b�N�v���b�N�w�b�_�[ (debugproc.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// �f�o�b�N�\��(��{�N���X)
//*****************************************************************************
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);
	static void Print(const char *fmt, ...);
	static void Draw(void);

private:
	static LPD3DXFONT m_pFont;		// �t�H���g�̃|�C���^
	static char m_aStr[1024];		// ���������i�[����o�b�t�@
};

#endif