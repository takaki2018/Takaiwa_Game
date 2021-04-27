//==============================================================================================================
//
// タイトル (title.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "title.h"
#include "title_bg.h"
#include "titlelogo.h"
#include "press_enter.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// タイトルの初期化処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTitle(void)
{
	// 背景の初期化処理
	InitTitleBg();

	// タイトルロゴの初期化処理
	InitTitlelogo();

	// プレスエンターの初期化処理
	InitPressEnter();

	// サウンドの開始
	PlaySound(SOUND_LABEL_TITLE);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// タイトルの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitTitle(void)
{
	// サウンドの一時停止
	StopSound();

	// 背景の終了処理
	UninitTitleBg();

	// タイトルロゴの終了処理
	UninitTitlelogo();

	// プレスエンターの終了処理
	UninitPressEnter();
}

//--------------------------------------------------------------------------------------------------------------
// タイトルの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateTitle(void)
{
	// 変数宣言
	bool bFallTitleLogo = GetbFall();
	bool bTransition = bGetTutorialTransition();

	// 背景の更新処理
	UpdateTitleBg();

	// タイトルロゴの更新処理
	UpdateTitlelogo();

	// タイトルロゴが完全に落ちたとき処理
	if (bFallTitleLogo == true)
	{// プレスエンターの更新処理
		UpdatePressEnter();
	}

	// チュートリアルに遷移
	if (bTransition == true)
	{
		SetFade(FADE_OUT,MODE_TUTORIAL);
	}
}

//--------------------------------------------------------------------------------------------------------------
// タイトルの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawTitle(void)
{
	// 変数宣言
	bool bFallTitleLogo = GetbFall();

	// 背景の描画処理
	DrawTitleBg();

	// タイトルロゴの描画処理
	DrawTitlelogo();

	// タイトルロゴが完全に落ちたとき処理
	if (bFallTitleLogo == true)
	{// プレスエンターの描画処理
		DrawPressEnter();
	}
}