//-------------------------------------------------------------------
//
// クマヘッダー (bear.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _BEAR_H_
#define _BEAR_H_

#include "main.h"

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posOld;	// 前回の位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 回転
	float fWidth;		// 幅
	float fHeight;		// 高さ
	float fTexU;		// テクスチャ座標Uの値
	float fTexV;		// テクスチャ座標Vの値
	int nAppTime;		// 現れる時間
	int nJampCnt;		// ジャンプするまでのカウント
	int nTurnCnt;		// ターンするまでのカウント
	bool bJump;			// ジャンプしているかどうか
	bool bLand;			// ブロックに乗っているかどうか
	bool bEvent;		// イベントかどうか
	bool bAppear;		// 登場したかどうか
	bool bUse;			// 使用しているかどうか
}BEAR;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitBear(void);										// クマの初期化処理
void UninitBear(void);										// クマの終了処理
void UpdateBear(void);										// クマの更新処理
void DrawBear(void);										// クマの描画処理

void TitleBear(void);										// タイトル画面のクマ
void GameBear(void);										// ゲーム画面のクマ
void HitBear(void);											// クマの当たり判定
void SetBear(D3DXVECTOR3 pos, D3DXVECTOR3 move);			// クマの配置
void SetVertexBear(void);									// 頂点座標の設定

float GetShaking(void);										// 画面揺れ情報の取得

#endif