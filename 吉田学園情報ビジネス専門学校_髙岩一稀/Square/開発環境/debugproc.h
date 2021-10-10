//=============================================================================
//
// デバックプロックヘッダー (debugproc.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// デバック表示(基本クラス)
//*****************************************************************************
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);
	static void Print(const char *fmt, ...);	// デバック表示用の文字追加処理
	static void Draw(void);						// デバック表示処理

private:
	static LPD3DXFONT m_pFont;		// フォントのポインタ
	static char m_aStr[1024];		// 文字数を格納するバッファ
};

#endif