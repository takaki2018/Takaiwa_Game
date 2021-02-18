//-------------------------------------------------------------------
//
// ランクロゴヘッダー (rank_logo.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RANKLOGO_H_
#define _RANKLOGO_H_

#include "main.h"

//-------------------------------------------------------------------
// ランキング順位
//-------------------------------------------------------------------
typedef enum
{
	RANK_1 = 0,		// 1位
	RANK_2,			// 2位
	RANK_3,			// 3位
	RANK_4,			// 4位
	RANK_5,			// 5位
	RANK_MAX,
}RANK;

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	RANK nTexType;		// テクスチャの種類
	float fWidth;		// 幅
	float fHeight;		// 高さ
	bool bUse;			// 使用しているかどうか
}RANKLOGO;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitRankLogo(void);														// ランクロゴの初期化処理
void UninitRankLogo(void);														// ランクロゴの終了処理
void UpdateRankLogo(void);														// ランクロゴの更新処理
void DrawRankLogo(void);														// ランクロゴの描画処理
void SetVErtexRankLogo(int nCntRankLogo);										// ランクロゴの頂点座標の設定
void SetRankLogo(D3DXVECTOR3 pos, float fWidth,float fHeight, RANK nTexType);	// ロゴの設置
void CangeRankLogo(int nCntRankLogo, RANK nTexType);							// ランクロゴの変更

#endif