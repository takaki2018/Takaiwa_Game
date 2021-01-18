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
// プレイヤーの状態
//-------------------------------------------------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,	// 通常状態	
	PLAYERSTATE_HIT,		// プレイヤーが敵に当たった状態
	PLAYERSTATE_FLASHING,	// 点滅
	PLAYERSTATE_MAX,
}PLAYERSTATE;

//-------------------------------------------------------------------
// プレイヤーの構造体
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 位置
	D3DXVECTOR3 move;	// 移動量
	PLAYERSTATE state;	// 状態
	int nCounterState;	// 状態カウンタ
	int nCounterDisp;	// 表示カウンタ
	bool bDisp;			// 表示ON/OFF 
} PLAYER;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitPlayer(void);						// プレイヤーの初期化処理
void UninitPlayer(void);						// プレイヤーの終了処理
void UpdatePlayer(void);						// プレイヤーの更新処理
void DrawPlayer(void);							// プレイヤーの描画処理
void MovePlayer(void);							// プレイヤーの移動処理
void PlayerStateNormal(void);					// プレイヤー状態が通常のとき
void PlayerStateHit(void);						// プレイヤー状態が攻撃を受けたとき
void PlayerStateFlashing(void);					// プレイヤー状態が点滅のとき
bool HitPlayer(int nIdx);						// 敵に当たったかどうか
PLAYER *GetPlayer(void);						// プレイヤー情報の取得

#endif