//-----------------------------------------------------------------
//
// チュートリアル (tutorial.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "tutorial.h"
#include "bg.h"
#include "oe.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "effect.h"
#include "shockwave.h"
#include "object.h"
#include "ui.h"
#include "timer.h"
#include "score.h"
#include "pause.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"

//-----------------------------------------------------------------
// チュートリアルの初期化処理
//-----------------------------------------------------------------
HRESULT InitTutorial(void)
{
	// 背景の初期化処理
	InitBg();

	// 操作説明の初期化処理
	InitOe();

	// プレイヤーの初期化処理
	InitPlayer();

	// 弾の初期化処理
	InitBullet();

	// 敵の初期化処理
	InitEnemy();

	// オブジェクトの初期化処理
	InitObject();

	// エフェクトの初期化処理
	InitEffect();

	// 衝撃波の初期化処理
	InitShockwave();

	// UIの初期化処理
	InitUi();

	// スコアの初期化
	InitScore();

	// タイマーの初期化
	InitTimer();

	return S_OK;
}

//-----------------------------------------------------------------
// チュートリアルの終了処理
//-----------------------------------------------------------------
void UninitTutorial(void)
{
	// タイマーの終了処理
	UninitTimer();

	// スコアの終了処理
	UninitScore();

	// UIの終了処理
	UninitUi();

	// 衝撃波の終了処理
	UninitShockwave();

	// エフェクトの終了処理
	UninitEffect();

	// オブジェクトの終了処理
	UninitObject();

	// 敵の終了処理
	UninitEnemy();

	// 弾の終了処理
	UninitBullet();

	// ポリゴンの終了処理
	UninitPlayer();

	// 操作説明の終了処理
	UninitOe();

	// 背景の終了処理
	UninitBg();
}

//-----------------------------------------------------------------
// チュートリアルの更新処理
//-----------------------------------------------------------------
void UpdateTutorial(void)
{
	// 背景の更新処理
	UpdateBg();

	// 操作説明の更新処理処理
	UpdateOe();

	// ポリゴンの更新処理
	UpdatePlayer();

	// 弾の更新処理
	UpdateBullet();

	// オブジェクトの更新処理
	UpdateObject();

	// エフェクトの更新処理
	UpdateEffect();

	// 衝撃波の更新処理
	UpdateShockwave();

	// スコアの更新処理
	UpdateScore();

	// UIの更新処理
	UpdateUi();

	// 敵の更新処理
	//UpdateEnemy();

	// タイトル画面に遷移
	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(JPINFO_OK) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//-----------------------------------------------------------------
// チュートリアルの描画処理
//-----------------------------------------------------------------
void DrawTutorial(void)
{
	// 背景の描画処理
	DrawBg();

	// 操作説明の描画処理
	DrawOe();

	// 弾の描画
	DrawBullet();

	// オブジェクトの描画処理
	DrawObject();

	// エフェクトの描画
	DrawEffect();

	// ポリゴンの描画処理
	DrawPlayer();

	// 敵の描画
	DrawEnemy();

	// 衝撃波の描画処理
	DrawShockwave();

	// UIの描画処理
	DrawUi();

	// スコアの描画処理
	DrawScore();

	// タイマーの描画処理
	DrawTimer();
}