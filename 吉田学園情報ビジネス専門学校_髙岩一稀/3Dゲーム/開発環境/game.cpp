//-----------------------------------------------------------------
//
// ゲームメイン (game.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "game.h"
#include "polygon.h"
#include "model.h"
#include "motion.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "ui.h"
#include "enemy.h"
#include "shadow.h"
#include "wall.h"
#include "coin.h"
#include "model_set.h"
#include "billboard.h"
#include "meshfield.h"
#include "effect.h"
#include "setparticle.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "input.h"
#include "gamepad.h"
#include "cross_product.h"

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
bool g_bPause = false;

//-----------------------------------------------------------------
// ベースの初期化処理
//-----------------------------------------------------------------
HRESULT InitGame(void)
{
	// ポリゴンの初期化処理
	InitPolygon();

	// 影の初期化処理
	InitShadow();

	// モデルの初期化処理
	//InitModel();

	// プレイヤーの初期化処理
	InitPlayer();

	// 敵の初期化処理
	InitEnemy();

	// 壁の初期化処理
	InitWall();

	// オブジェクトの初期化処理
	InitCoin();

	// モデル配置の初期化処理
	InitModelSet();

	// ビルボードの初期化処理
	InitBillboard();

	// メッシュフィールドの初期化処理
	InitMeshField();

	// 弾の初期化処理
	InitBullet();

	// エフェクトの初期化処理
	InitEffect();

	// 爆発の初期化処理
	InitExplosion();

	// UIの初期化処理
	InitUi();

	return S_OK;
}

//-----------------------------------------------------------------
// ベースの終了処理
//-----------------------------------------------------------------
void UninitGame(void)
{
	// UIの終了処理
	UninitUi();

	// 爆発の終了処理
	UninitExplosion();

	// エフェクトの終了処理
	UninitEffect();

	// 弾の終了処理
	UninitBullet();

	// メッシュフィールドの終了処理
	UninitMeshField();

	// ビルボードの終了処理
	UninitBillboard();

	// モデル配置の終了処理
	UninitModelSet();

	// オブジェクトの終了処理
	UninitCoin();

	// 壁の終了処理
	UninitWall();

	// 影の終了処理
	UninitShadow();

	// 敵の終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// モデルの終了処理
	//UninitModel();

	// ポリゴンの終了処理
	UninitPolygon();
}

//-----------------------------------------------------------------
// ベースの更新処理
//-----------------------------------------------------------------
void UpdateGame(void)
{
	// プレイヤー情報を取得
	Player *pPlayer = GetPlayer();

	if (GetKeyboardTrigger(KEYINFO_PAUSE) == true || GetJoypadTrigger(PLAYER_1,JPINFO_PAUSE))
	{
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == false && pPlayer->bFall == false)
	{
		// テスト
		//SetParticle();

		// エフェクトの更新処理
		UpdateEffect();

		// ポリゴンの更新処理
		UpdatePolygon();

		// モデルの更新処理
		//UpdateModel();

		// プレイヤーの更新処理
		UpdatePlayer();

		// 敵の更新処理
		UpdateEnemy();

		// 影の更新処理
		UpdateShadow();

		// 壁の更新処理
		UpdateWall();

		// オブジェクトの更新処理
		UpdateCoin();

		// モデル配置の更新処理
		UpdateModelSet();

		// ビルボードの更新処理
		UpdateBillboard();

		// メッシュフィールドの更新処理
		UpdateMeshField();

		// 弾の更新処理
		UpdateBullet();

		// 爆発の更新処理
		UpdateExplosion();
		
		// UIの更新処理
		UpdateUi();

		// 外積を用いた当たり判定の初期化
		CrossProductTest();
	}

	if (pPlayer->bFall == true)
	{// プレイヤーが落ちたら処理
		SetFade(FADE_OUT,MODE_RESULT);
	}
}

//-----------------------------------------------------------------
// ベースの描画処理
//-----------------------------------------------------------------
void DrawGame(void)
{
	// ポリゴンの描画処理
	//DrawPolygon();

	// モデルの描画処理
	//DrawModel();

	// プレイヤーの描画処理
	DrawPlayer();

	// 敵の描画処理
	DrawEnemy();

	// 壁の描画処理
	//DrawWall();

	// オブジェクトの描画処理
	DrawCoin();

	// モデル配置の描画処理
	DrawModelSet();

	// ビルボードの描画処理
	//DrawBillboard();

	// メッシュフィールドの描画処理
	DrawMeshField();

	// 弾の描画処理
	DrawBullet();

	// エフェクトの描画処理
	DrawEffect();

	// 爆発の描画処理
	DrawExplosion();

	// 影の描画処理
	DrawShadow();

	// UIの描画処理
	DrawUi();
}

//-------------------------------------------------------------------
// ポーズの移り替わり
//-------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}