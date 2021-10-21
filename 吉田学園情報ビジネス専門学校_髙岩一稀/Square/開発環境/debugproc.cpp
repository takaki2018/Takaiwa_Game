//=============================================================================
//
// デバック表示処理 [debugproc.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "debugproc.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPD3DXFONT CDebugProc::m_pFont = NULL;		// フォントのポインタ
char CDebugProc::m_aStr[1024] = {};			// 文字数を格納するバッファ

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
	// 文字を配置する位置の決定
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// フォントの表示
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0, 0));

	// バッファのクリア
	memset(&m_aStr[0], 0, 1024);
}

//=============================================================================
// デバック表示用の文字追加処理
// 引数		: *fmt	- 表示したい文字列
//			: ...	- 可変長引数
// 返り値	: void	- 何も返さない
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
			// 文字が%以外のとき文字を保存する
			wsprintf(&aWard[0], "%c", *fmt);
		}
		else
		{
			// 文字が%だったときポインタを進める
			fmt++;

			// 文字ごとに処理を変える
			switch (*fmt)
			{
				// dだったとき引数に入っているint型の変数の値に変える
			case 'd':
				wsprintf(&aWard[0], "%d", va_arg(arg, int));
				break;

				// fだったとき引数に入っているdouble型の変数の値に変える
			case 'f':
				sprintf(&aWard[0], "%.2f", va_arg(arg, double));
				break;

				// sだったとき引数に入っているconst char*型の変数の値に変える
			case 's':
				wsprintf(&aWard[0], "%s", va_arg(arg, const char*));
				break;

				// cだったとき引数に入っているchar型の変数の値に変える
			case 'c':
				wsprintf(&aWard[0], "%c", va_arg(arg, char));
				break;

				// 文字が上記以外のとき文字を保存する
			default:
				wsprintf(&aWard[0], "%c", *fmt);
				break;
			}
		}
		// 何文字保存したかカウントする
		nCnt += wsprintf(&aStr[nCnt], "%s",&aWard[0]);
	}
	// 識別を終える
	va_end(arg);

	// 文字列を連結させる
	strcat(m_aStr, aStr);
}