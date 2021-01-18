//-------------------------------------------------------------------
//
// ゲーム画面処理 (game.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "game.h"
#include "selectmode.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "gamepad.h"
#include "bullet.h"
#include "object.h"
#include "enemy.h"
#include "effect.h"
#include "shockwave.h"
#include "item.h"
#include "ui.h"
#include "score.h"
#include "timer.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"
#include "pause_oe.h"

//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
bool g_bPause = false;	// ポーズ
int g_nCounterItem;		// アイテムの効果時間
int g_nCntResult;		// リザルト画面までのカウント
int g_Timer;			// タイマー
bool g_bHit;		// オブジェクトと敵が当たったかどうか

//-------------------------------------------------------------------
// ゲーム画面の初期化処理
//-------------------------------------------------------------------
HRESULT InitGame(void)
{
	// 変数の初期化
	g_nCounterItem = 0;
	g_nCntResult = 0;

	// 背景の初期化処理
	InitBg();

	// ポリゴンの初期化処理
	InitPlayer();

	// オブジェクトの初期化処理
	InitObject();

	// 弾の初期化処理
	InitBullet();

	// 敵の初期化処理
	InitEnemy();

	// エフェクトの初期化処理
	InitEffect();

	// 衝撃波の初期化処理
	InitShockwave();

	// UIの初期化処理
	InitUi();

	// タイマーの初期化
	InitTimer();

	// ポーズの初期化処理
	InitPause();

	// ポーズOEの初期化処理
	InitPauseOe();

	// BGM
	PlaySound(SOUND_LABEL_BATTLE);

	return S_OK;
}

//-------------------------------------------------------------------
// ゲーム画面の終了処理
//-------------------------------------------------------------------
void UninitGame(void)
{
	// サウンドの一時停止
	StopSound();

	// ポーズOEの終了処理
	UninitPauseOe();

	// ポーズの終了処理
	UninitPause();

	// タイマーの終了処理
	UninitTimer();

	// UIの終了処理
	UninitUi();

	// 衝撃波の終了処理
	UninitShockwave();

	// エフェクトの終了処理
	UninitEffect();

	// 敵の終了処理
	UninitEnemy();

	// 弾の終了処理
	UninitBullet();

	// オブジェクトの終了処理
	UninitObject();

	// ポリゴンの終了処理
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
	g_Timer = GetTimer();				// タイマー
	g_bHit = GetHitObject();			// 敵とオブジェクトの当たり判定
	int SelectMode = GetSelectMode();	// セレクトモード
	FADE fade = GetFade();				// フェード情報

	// ポーズの処理
	if ((GetKeyboardTrigger(KEYINFO_PAUSE) == true || GetJoypadTrigger(JPINFO_PAUSE) == true) && fade == FADE_NONE)
	{
		g_bPause = g_bPause ? false : true;

		if (g_bPause == true)
		{// サウンド
			PlaySound(SOUND_LABEL_SE_PAUSE_IN);
		}
		else if (g_bPause == false)
		{// サウンド
			PlaySound(SOUND_LABEL_SE_PAUSE_OUT);
		}
	}
	// 選択したモードに応じて処理
	switch (SelectMode)
	{// ノーマルモードの処理
	case SELECTMODE_NORMAL:
		NormalMode();
		break;

		// エンドレスモードの処理
	case SELECTMODE_ENDLESS:
		EndlessMode();
		break;
	}

	if (g_bHit == false && g_bPause == false)
	{// タイマーの更新処理
		UpdateTimer();
	}
}

//-------------------------------------------------------------------
// ゲーム画面の描画処理
//-------------------------------------------------------------------
void DrawGame(void)
{
	// 変数宣言
	FADE fade = GetFade();

	// 背景の描画処理
	DrawBg();

	// 弾の描画
	DrawBullet();

	// エフェクトの描画
	DrawEffect();

	// ポリゴンの描画処理
	DrawPlayer();

	// オブジェクトの描画処理
	DrawObject();

	// 敵の描画
	DrawEnemy();

	// 衝撃波の描画処理
	DrawShockwave();

	// UIの描画処理
	DrawUi();

	// タイマーの描画処理
	DrawTimer();

	if (g_bPause == true)
	{// ポーズの描画処理
		DrawPause();

		// ポーズOEの初期化処理
		DrawPauseOe();
	}
}

//-------------------------------------------------------------------
// ノーマルモードの処理
//-------------------------------------------------------------------
void NormalMode(void)
{
	if (g_bPause == false && g_Timer != 0)
	{// 敵の更新処理
		UpdateEnemy();

		if (g_bHit == false)
		{// 背景の更新処理
			UpdateBg();

			// ポリゴンの更新処理
			UpdatePlayer();

			// オブジェクトの更新処理
			UpdateObject();

			// 弾の更新処理
			UpdateBullet();

			// エフェクトの更新処理
			UpdateEffect();

			// 衝撃波の更新処理
			UpdateShockwave();

			// UIの更新処理
			UpdateUi();
		}
	}
	else if (g_bPause == true)
	{// ポーズの更新処理
		UpdatePause();

		// ポーズOEの更新処理
		UpdatePauseOe();
	}
	// 敵がオブジェクトに当たったらリザルト画面に遷移
	if ((g_Timer == 0) | (g_bHit == true))
	{// リザルト画面までのカウント
		g_nCntResult++;

		if (g_nCntResult == 60)
		{// 画面モードの初期化処理
			int nCntIn = GetEnemyIn();
			AddScore(nCntIn * 100);
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
	
}

//-------------------------------------------------------------------
// エンドレスモードの処理
//-------------------------------------------------------------------
void EndlessMode(void)
{
	if (g_bPause == false)
	{// 敵の更新処理
		UpdateEnemy();

		if (g_bHit == false)
		{// 背景の更新処理
			UpdateBg();

			// ポリゴンの更新処理
			UpdatePlayer();

			// オブジェクトの更新処理
			UpdateObject();

			// 弾の更新処理
			UpdateBullet();

			// エフェクトの更新処理
			UpdateEffect();

			// 衝撃波の更新処理
			UpdateShockwave();

			// UIの更新処理
			UpdateUi();
		}
	}
	else if (g_bPause == true)
	{// ポーズの更新処理
		UpdatePause();

		// ポーズOEの更新処理
		UpdatePauseOe();
	}
	// 敵がオブジェクトに当たったらリザルト画面に遷移
	if (g_bHit == true)
	{// リザルト画面までのカウント
		g_nCntResult++;

		if (g_nCntResult == 60)
		{// 画面モードの初期化処理
			int nCntIn = GetEnemyIn();
			int nTimer = GetTimer();
			AddScore(nCntIn * 100);
			AddScore(nTimer * 100);
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//-------------------------------------------------------------------
// ポーズの移り替わり
//-------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}