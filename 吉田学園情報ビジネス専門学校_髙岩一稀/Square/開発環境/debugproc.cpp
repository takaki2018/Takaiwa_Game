//=============================================================================
//
// デバック表示処理 [debugproc.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "stdio.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[1024] = {};

//=============================================================================
// CDebugProcのコンストラクタ
//=============================================================================
CDebugProc::CDebugProc()
{
	
}

//=============================================================================
// CDebugProcのデストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
void CDebugProc::Init(void)
{
	// フォントの生成
	D3DXCreateFont(CManager::GetRenderer()->GetDevice(), 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	// バッファのクリア
	memset(&m_aStr[0], 0, 1024);
}

//=============================================================================
// 終了処理
//=============================================================================
void CDebugProc::Uninit(void)
{
	// フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// フォントの表示
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0, 0, 0));

	// バッファのクリア
	memset(&m_aStr[0], 0, 1024);
}

//=============================================================================
// デバック表示用の文字追加処理
//=============================================================================
void CDebugProc::Print(const char *fmt, ...)
{
	// 変数宣言
	va_list arg;
	char aStr[256];
	char aWard[64];

	// 初期化
	memset(aStr, 0, 256);
	memset(aWard, 0, 64);

	// 文字列の識別を開始
	va_start(arg, fmt);

	// 文字列が最後になるまで続ける
	for (int nCnt = 0; *fmt != '\0'; fmt++)
	{
		if (*fmt != '%')
		{
			wsprintf(&aWard[0], "%c", *fmt);
		}
		else
		{
			fmt++;
			switch (*fmt)
			{
			case 'd':
				wsprintf(&aWard[0], "%d", va_arg(arg, int));
				break;

			case 'f':
				sprintf(&aWard[0], "%.2f", va_arg(arg, double));
				break;

			case 's':
				wsprintf(&aWard[0], "%s", va_arg(arg, const char*));
				break;

			case 'c':
				wsprintf(&aWard[0], "%c", va_arg(arg, char));
				break;
			}
		}
		nCnt += wsprintf(&aStr[nCnt], "%s",&aWard[0]);
	}
	// 識別を終える
	va_end(arg);

	// 文字列を連結させる
	strcat(m_aStr, aStr);
}