//==============================================================================================================
//
// ゲームメイン (game.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "game.h"
#include "motion.h"
#include "player.h"
#include "ui.h"
#include "enemy.h"
#include "shadow.h"
#include "coin.h"
#include "model_set.h"
#include "meshfield.h"
#include "3Deffect.h"
#include "setparticle.h"
#include "pause.h"
#include "timer.h"
#include "coin_sheets.h"
#include "score.h"
#include "fade.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"
#include "camera.h"

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
bool g_bPause = false;
int g_nCntResult;						// リザルトまでのカウント

//--------------------------------------------------------------------------------------------------------------
// ベースの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitGame(void)
{
	// 変数の初期化
	g_nCntResult = 0;

	// ゲームカメラの初期化
	InitGameCamera();

	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化処理
	InitPlayer();

	// 敵の初期化処理
	InitEnemy();

	// オブジェクトの初期化処理
	InitCoin();

	// モデル配置の初期化処理
	InitModelSet();

	// メッシュフィールドの初期化処理
	InitMeshField();

	// エフェクトの初期化処理
	Init3DEffect();

	// UIの初期化処理
	InitUi();

	// コイン獲得状況の初期化
	InitCoinSheets();

	// ポーズ画面の初期化処理
	InitPause();

	// サウンドの追加
	PlaySound(SOUND_LABEL_BATTLE);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// ベースの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitGame(void)
{
	// サウンドの一時停止
	StopSound();

	// ポーズ画面の終了処理
	UninitPause();

	// コイン獲得状況の終了処理
	UninitCoinSheets();

	// UIの終了処理
	UninitUi();

	// エフェクトの終了処理
	Uninit3DEffect();

	// メッシュフィールドの終了処理
	UninitMeshField();

	// モデル配置の終了処理
	UninitModelSet();

	// オブジェクトの終了処理
	UninitCoin();

	// 影の終了処理
	UninitShadow();

	// 敵の終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();
}

//--------------------------------------------------------------------------------------------------------------
// ベースの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateGame(void)
{
	// プレイヤー情報を取得
	Player *pPlayer = GetPlayer();
	FADE fade = GetFade();				// フェード情報
	bool bNocoin = GetbNoCoin();
	int nTimer = GetTimer();

	if ((GetKeyboardTrigger(KEYINFO_PAUSE) == true || GetJoypadTrigger(PLAYER_1,JPINFO_PAUSE)) && 
		pPlayer->bFall == false && fade == FADE_NONE && bNocoin == false && nTimer != 0)
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
	if (g_bPause == false && pPlayer->bFall == false && bNocoin == false && nTimer != 0)
	{
		// エフェクトの更新処理
		Update3DEffect();

		// オブジェクトの更新処理
		UpdateCoin();

		// モデル配置の更新処理
		UpdateModelSet();

		// メッシュフィールドの更新処理
		UpdateMeshField();

		// プレイヤーの更新処理
		UpdatePlayer();

		// 敵の更新処理
		UpdateEnemy();

		// 影の更新処理
		UpdateShadow();
	}
	else if (g_bPause == true && pPlayer->bFall == false && bNocoin == false && nTimer != 0)
	{
		// ポーズ画面の更新処理
		UpdatePause();
	}
	if (g_bPause == false  && pPlayer->bFall == false && bNocoin == false)
	{
		// UIの更新処理
		UpdateUi();
	}
	if (g_bPause == false && pPlayer->bFall == false && nTimer != 0)
	{
		// コイン獲得状況の更新処理
		UpdateCoinSheets();
	}

	if (pPlayer->bFall == true || bNocoin == true || nTimer == 0)
	{// リザルト画面までのカウント
		g_nCntResult++;

		if (g_nCntResult == 60)
		{
			if (nTimer != 0 && pPlayer->bFall == false)
			{
				// スコアの加算
				AddScore(100 * nTimer);
			}

			// 画面モードの初期化処理
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// ベースの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawGame(void)
{
	// プレイヤーの描画処理
	DrawPlayer();

	// 敵の描画処理
	DrawEnemy();

	// オブジェクトの描画処理
	DrawCoin();

	// モデル配置の描画処理
	DrawModelSet();

	// メッシュフィールドの描画処理
	DrawMeshField();

	// 影の描画処理
	DrawShadow();

	// UIの描画処理
	DrawUi();

	// コイン獲得状況の描画処理
	DrawCoinSheets();

	if (g_bPause == true)
	{
		// ポーズ画面の描画処理
		//DrawPause();
	}
	// エフェクトの描画処理
	Draw3DEffect();
}

//----------------------------------------------------------------------------------------------------------------
// ポーズの移り替わり
// 引数		：bPause	- ポーズしてるかどうか
// 返り値	：void		- 何も返さない
//----------------------------------------------------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}