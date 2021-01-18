//-------------------------------------------------------------------
//
// プレイヤーヘッダー (player.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define PLAYER_SIZE_WIDTH	(45)		// プレイヤーのサイズ(幅)
#define PLAYER_SIZE_HEIGHT	(80)		// プレイヤーのサイズ(高さ)

//-------------------------------------------------------------------
// プレイヤーの状態
//-------------------------------------------------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,	// 通常状態	
	PLAYERSTATE_FLASHING,	// 点滅
	PLAYERSTATE_SHRINK,		// 縮んでいる状態
	PLAYERSTATE_MAX,
}PLAYERSTATE;

//-------------------------------------------------------------------
// プレイヤーの構造体
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posOld;	// 前の位置
	D3DXVECTOR3 move;	// 移動量
	D3DXCOLOR col;		// 色
	float fWidth;		// 幅
	float fHeight;		// 高さ
	int nCntAnim;		// アニメーションカウント
	int nPatternAnim;	// アニメーションパターン
	float fV;			// V値の移動
	bool bMove;			// 移動中かどうか
	bool bJump;			// ジャンプしたかどうか
	PLAYERSTATE state;	// 状態
	int nCounterState;	// 状態カウンタ
	int nCounterDisp;	// 表示カウンタ
	int nCounterSize;	// サイズを戻すカウント
} PLAYER;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitPlayer(void);						// プレイヤーの初期化処理
void UninitPlayer(void);						// プレイヤーの終了処理
void UpdatePlayer(void);						// プレイヤーの更新処理
void DrawPlayer(void);							// プレイヤーの描画処理

void SingleModePlayer(void);					// シングルモードのプレイヤー処理
void MultiModePlayer(void);						// マルチモードのプレイヤー処理

void SingleMovePlayer(void);					// シングルモードのプレイヤー移動処理
void MultiMovePlayer(int nCntPlayer);			// マルチモードのプレイヤー移動処理

void AnimationPlayer(int nCntPlayer);			// テクスチャアニメーションの設定
void HitBlock(int nCntPlayer);					// ブロックの当たり判定
void HitPlayer(int nCntPlayer,float fAngle);	// プレイヤーの当たり判定
void CollisionPlayer(int nCntPlayer);			// プレイヤー同士の当たり判定

void SetVertexPlayer(int nIdx);					// プレイヤーの頂点座標の設定

void PlayerNormalProcessing(int nCntPlayer);	// プレイヤー状態が通常のとき
void PlayerStateFlashing(int nCntPlayer);		// プレイヤー状態が点滅のとき
void PlayerStateShrink(int nCntPlayer);			// プレイヤー状態が縮んでいるとき
void ResultPlayer(int nCntPlayer);				// リザルト画面の処理

PLAYER *GetPlayer(void);						// プレイヤー情報の取得
int GetPlayerCnt(void);							// プレイヤー数の獲得

#endif