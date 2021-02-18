//-----------------------------------------------------------------
//
// タイトル (title.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "title.h"
#include "title_bg.h"
#include "titlelogo.h"
#include "selectmode.h"
#include "sound.h"
#include "setparticle.h"

//-----------------------------------------------------------------
// タイトルの初期化処理
//-----------------------------------------------------------------
HRESULT InitTitle(void)
{
	// 背景の初期化処理
	InitTitleBg();

	// タイトルロゴの初期化処理
	InitTitlelogo();

	// セレクトモードの初期化処理
	InitSelectmode();

	// サウンドの開始
	//PlaySound(SOUND_LABEL_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------
// タイトルの終了処理
//-----------------------------------------------------------------
void UninitTitle(void)
{
	// サウンドの一時停止
	StopSound();

	// 背景の終了処理
	UninitTitleBg();

	// タイトルロゴの終了処理
	UninitTitlelogo();

	// セレクトモードの終了処理
	UninitSelectmode();
}

//-----------------------------------------------------------------
// タイトルの更新処理
//-----------------------------------------------------------------
void UpdateTitle(void)
{
	// 変数宣言
	bool bFall = GetFall();

	// 背景の更新処理
	UpdateTitleBg();

	// タイトルロゴの更新処理
	UpdateTitlelogo();

	// タイトルロゴが完全に落ちたとき処理
	//if (bFall == true)
	{// セレクトモードの更新処理
		UpdateSelectmode();
	}
}

//-----------------------------------------------------------------
// タイトルの描画処理
//-----------------------------------------------------------------
void DrawTitle(void)
{
	// 変数宣言
	bool bFall = GetFall();

	// 背景の描画処理
	DrawTitleBg();

	// タイトルロゴの描画処理
	DrawTitlelogo();

	// タイトルロゴが完全に落ちたとき処理
	//if (bFall == true)
	{// セレクトモードの描画処理
		DrawSelectmode();
	}
}