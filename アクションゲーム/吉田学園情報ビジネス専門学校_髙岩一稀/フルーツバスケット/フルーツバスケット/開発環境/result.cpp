//-----------------------------------------------------------------
//
// リザルト (result.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result.h"
#include "result_bg.h"
#include "selectmode.h"
#include "player.h"
#include "gauge.h"
#include "result_logo.h"
#include "result_ann.h"
#include "ranking.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
int g_nSkipCnt;					// リザルトからタイトルに遷移できるまでのカウント
PLAYMODE g_Resultplaymode;		//　プレイモード

//-----------------------------------------------------------------
// リザルトの初期化処理
//-----------------------------------------------------------------
HRESULT InitResult(void)
{
	// 変数の初期化
	g_nSkipCnt = 0;
	g_Resultplaymode = GetPlayMode();

	// リザルト背景の初期化処理
	InitResultBg();

	// プレイヤーの初期化処理
	InitPlayer();

	// ゲージの初期化処理
	InitGauge();

	// リザルトロゴの初期化処理
	InitResultLogo();

	if (g_Resultplaymode == PLAYMODE_SINGLE)
	{// ランキングの初期化処理
		InitRanking();
	}

	// 結果発表の初期化処理
	InitResultAnn();

	return S_OK;
}

//-----------------------------------------------------------------
// リザルトの終了処理
//-----------------------------------------------------------------
void UninitResult(void)
{
	// サウンドの一時停止
	StopSound();

	// 結果発表の終了処理
	UninitResultAnn();

	if (g_Resultplaymode == PLAYMODE_SINGLE)
	{// ランキングの終了処理
		UninitRanking();
	}

	// リザルトロゴの終了胥吏
	UninitResultLogo();

	// ゲージの終了処理
	UninitGauge();

	// プレイヤーの終了処理
	UninitPlayer();

	// リザルト背景の終了処理
	UninitResultBg();
}

//-----------------------------------------------------------------
// リザルトの更新処理
//-----------------------------------------------------------------
void UpdateResult(void)
{
	// 変数宣言
	bool bRAUse = GetResultAnn();
	bool bSound = GetbGauge();

	// リザルト背景の更新処理
	UpdateResultBg();

	// プレイヤーの更新処理
	UpdatePlayer();

	// リザルトロゴの更新処理
	UpdateResultLogo();

	// 結果発表の更新処理
	UpdateResultAnn();

	if (bRAUse == false)
	{
		// ゲージの更新処理
		UpdateGauge();
	}

	if (bSound == true)
	{
		// カウントの更新
		g_nSkipCnt++;

		if (g_Resultplaymode == PLAYMODE_SINGLE)
		{// ランキングの更新処理
			UpdateRanking();
		}
	}
	if (g_nSkipCnt > 60)
	{
		if (GetJoypadTrigger(0, JPINFO_OKorJAMP) == true ||
			GetKeyboardTrigger(KEYINFO_OK) == true)
		{// 画面モードの初期化処理
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
}

//-----------------------------------------------------------------
// リザルトの描画処理
//-----------------------------------------------------------------
void DrawResult(void)
{
	// リザルト背景の描画処理
	DrawResultBg();

	// ゲージの描画処理
	DrawGauge();

	// プレイヤーの描画処理
	DrawPlayer();

	// リザルトロゴの描画処理
	DrawResultLogo();

	if (g_Resultplaymode == PLAYMODE_SINGLE)
	{// ランキングの描画処理
		DrawRanking();
	}

	// 結果発表の描画処理
	DrawResultAnn();
}