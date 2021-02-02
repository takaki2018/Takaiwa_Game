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
#include "bear.h"
#include "wood.h"
#include "fruits.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
bool g_bSkipTitle;				// タイトルを飛ばすときに使用

//-----------------------------------------------------------------
// タイトルの初期化処理
//-----------------------------------------------------------------
HRESULT InitTitle(void)
{
	// 変数の初期化
	g_bSkipTitle = false;

	// タイトル背景の描画処理
	InitTitleBg();

	// タイトルロゴの初期化処理
	InitTitlelogo();

	// セレクトモードの初期化処理
	InitSelectmode();

	// クマの初期化処理
	InitBear();

	// フルーツの初期化処理
	InitFruits();

	// 木の初期化処理
	InitWood();

	// サウンドの開始
	PlaySound(SOUND_LABEL_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------
// タイトルの終了処理
//-----------------------------------------------------------------
void UninitTitle(void)
{
	// サウンドの一時停止
	StopSound();

	// 木の終了処理
	UninitWood();

	// フルーツの終了処理
	UninitFruits();

	// クマの終了処理
	UninitBear();

	// セレクトモードの終了処理
	UninitSelectmode();

	// タイトルロゴの終了処理
	UninitTitlelogo();

	// タイトル背景の終了処理
	UninitTitleBg();
}

//-----------------------------------------------------------------
// タイトルの更新処理
//-----------------------------------------------------------------
void UpdateTitle(void)
{
	// 変数宣言
	bool bFall = GetFall();
	bool bTitleFruits = GetbTitleFruits();
	int nTitleCnt = GetTitleFruitsCnt();

	if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJAMP) == true || 
		GetKeyboardTrigger(KEYINFO_OK) == true)
	{// エンターキーまたはAボタンを押したらスキップ
		g_bSkipTitle = true;
	}
	// タイトル背景の更新処理
	UpdateTitleBg();

	if ((bTitleFruits == true && nTitleCnt < 0) || g_bSkipTitle == true)
	{
		// タイトルロゴの更新処理
		UpdateTitlelogo();

		// タイトルロゴが完全に落ちたとき処理
		if (bFall == true)
		{// セレクトモードの更新処理
			UpdateSelectmode();
		}
	}
	// クマの更新処理
	UpdateBear();

	// フルーツの更新処理
	UpdateFruits();

	// 木の更新処理
	UpdateWood();
}

//-----------------------------------------------------------------
// タイトルの描画処理
//-----------------------------------------------------------------
void DrawTitle(void)
{
	// 変数宣言
	bool bFall = GetFall();
	bool bTitleFruits = GetbTitleFruits();
	int nTitleCnt = GetTitleFruitsCnt();

	// タイトル背景の描画処理
	DrawTitleBg();

	if (g_bSkipTitle != true)
	{
		// クマの描画処理
		DrawBear();
	}
	// 木の描画処理
	DrawWood();

	if ((bTitleFruits == true && nTitleCnt < 0) || g_bSkipTitle == true)
	{
		// タイトルロゴの描画処理
		DrawTitlelogo();

		// タイトルロゴが完全に落ちたとき処理
		if (bFall == true)
		{// セレクトモードの描画処理
			DrawSelectmode();
		}
	}

	if (g_bSkipTitle != true)
	{
		// フルーツの描画処理
		DrawFruits();
	}
}

//-----------------------------------------------------------------
// タイトルの演出を飛ばしたかどうか
//-----------------------------------------------------------------
bool GetSkipTitle(void)
{
	return g_bSkipTitle;
}