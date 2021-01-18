//-------------------------------------------------------------------
//
// ゲーム画面処理 (game.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "game.h"
#include "player.h"
#include "bg.h"
#include "block.h"
#include "fruits.h"
#include "enemy.h"
#include "crow.h"
#include "rat.h"
#include "wood.h"
#include "wood_back.h"
#include "timer.h"
#include "bear.h"
#include "ui.h"
#include "pause.h"
#include "count_down.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "effect.h"
#include "sound.h"

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
bool g_bPause = false;	// ポーズ
int g_nCounterItem;		// アイテムの効果時間
int g_nCntResult;		// リザルト画面までのカウント

//-------------------------------------------------------------------
// ゲーム画面の初期化処理
//-------------------------------------------------------------------
HRESULT InitGame(void)
{
	// 変数の初期化
	g_nCntResult = 0;

	// 背景の初期化
	InitBg();

	// プレイヤーの初期化
	InitPlayer();

	// ブロックの初期化処理
	InitBlock();

	// フルーツの初期化処理
	InitFruits();

	// 敵の初期化処理
	InitEnemy();

	// カラスの初期化処理
	InitCrow();

	// ネズミの初期化処理
	InitRat();

	// 木の初期化処理
	InitWood();

	// 木の裏の初期化処理
	InitWoodBack();

	// タイマーの初期か処理
	InitTimer();

	// クマの初期化処理
	InitBear();

	// ポーズの初期化処理
	InitPause();

	// エフェクトの初期化処理
	InitEffect();

	// UIの初期化処理
	InitUI();

	// カウントダウンの初期化処理
	InitCountDown();

	// ブロックの配置
	SetBlockInfo(MODESTATE_GAME_FOREST);

	return S_OK;
}

//-------------------------------------------------------------------
// ゲーム画面の終了処理
//-------------------------------------------------------------------
void UninitGame(void)
{
	// サウンドの一時停止
	StopSound();

	// カウントダウンの終了処理
	UninitCountDown();

	// UIの終了処理
	UninitUI();

	// エフェクトの終了処理
	UninitEffect();

	// ポーズの終処理
	UninitPause();

	// クマの終了処理
	UninitBear();

	// タイマーの終了処理
	UninitTimer();

	// 木の裏の終了処理
	UninitWoodBack();

	// 木の終了処理
	UninitWood();

	// ネズミの終了処理
	UninitRat();

	// カラスの終了処理
	UninitCrow();

	// 敵の終了処理
	UninitEnemy();

	// フルーツの終了処理
	UninitFruits();

	// ブロックの終了処理
	UninitBlock();

	// プレイヤーの終了処理
	UninitPlayer();

	// 背景の終了処理
	UninitBg();
}

//-------------------------------------------------------------------
// ゲーム画面の更新処理
//-------------------------------------------------------------------
void UpdateGame(void)
{
	// 変数宣言
	int nCountDown = GetCountDown();	// カウントダウンの取得
	int nTimer = GetTimer();			// 残り時間の取得
	FADE fade = GetFade();				// フェード情報

	if (nTimer != 0)
	{
		// ポーズの処理
		if ((GetJoypadTrigger(PLAYER_1, JPINFO_PAUSE) == true || 
			GetKeyboardTrigger(KEYINFO_PAUSE) == true) &&
			fade == FADE_NONE && nCountDown == 0)
		{
			g_bPause = g_bPause ? false : true;

			if (g_bPause == true)
			{// ポーズ画面を開いたとき
				// SE
				PlaySound(SOUND_LABEL_SE_PAUSE_IN);
			}
			else if (g_bPause == false)
			{// ポーズ画面を閉じたとき
				// SE
				PlaySound(SOUND_LABEL_SE_PAUSE_OUT);
			}
		}

		if (g_bPause == false)
		{// 背景の更新処理
			UpdateBg();

			// ブロックの更新処理
			UpdateBlock();

			if (nCountDown == 0)
			{// フルーツの更新処理
				UpdateFruits();

				// 敵の更新処理
				UpdateEnemy();

				// カラスの更新処理
				UpdateCrow();

				// ネズミの更新処理
				UpdateRat();

				//// プレイヤーの更新処理
				UpdatePlayer();

				// 木の更新処理
				UpdateWood();

				// 木の裏の更新処理
				UpdateWoodBack();

				// クマの更新処理
				UpdateBear();

				// エフェクトの更新処理
				UpdateEffect();

				// UIの更新処理
				UpdateUI();
			}
		}
		else if (g_bPause == true && fade == FADE_NONE)
		{// ポーズの更新処理
			UpdatePause();
		}
	}
	if (g_bPause == false && nCountDown == 0)
	{// タイマーの更新処理
		UpdateTimer();
	}
	// カウントダウンの更新処理
	UpdateCountDown();

	// 画面遷移
	if (nTimer == 0)
	{
		g_nCntResult++;

		if (g_nCntResult == 60)
		{// 画面モードの初期化処理

			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//-------------------------------------------------------------------
// ゲーム画面の描画処理
//-------------------------------------------------------------------
void DrawGame(void)
{
	// 背景の描画処理
	DrawBg();

	// カラスの描画処理
	DrawCrow();

	// ネズミの描画処理
	DrawRat();

	// 木の裏の描画処理
	DrawWoodBack();

	// プレイヤーの描画処理
	DrawPlayer();

	// フルーツの描画処理
	DrawFruits();

	// 敵の描画処理
	DrawEnemy();

	// ブロックの描画処理
	DrawBlock();

	// エフェクトの描画処理
	DrawEffect();

	// クマの描画処理
	DrawBear();

	// 木の描画処理
	DrawWood();

	// タイマーの描画処理
	DrawTimer();

	// UIの描画処理
	DrawUI();

	// カウントダウンの描画処理
	DrawCountDown();

	if (g_bPause == true)
	{// ポーズの描画処理
		DrawPause();
	}
}

//-------------------------------------------------------------------
// ポーズの移り替わり
//-------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}