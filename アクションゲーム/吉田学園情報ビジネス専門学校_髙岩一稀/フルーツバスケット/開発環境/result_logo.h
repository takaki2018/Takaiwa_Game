//-------------------------------------------------------------------
//
// リザルトロゴヘッダー (result_logo.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include "main.h"

//-------------------------------------------------------------------
// 何位のテクスチャか
//-------------------------------------------------------------------
typedef enum
{
	RANK_FIRST = 0,	// 一位
	RANK_SECOND,	// 二位
	RANK_THIRD,		// 三位
	RANK_FOURTH,	// 四位
	RANK_MAX,
}RANK;

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXCOLOR col;		// 色
	RANK nTexType;		// テクスチャの種類
	float fHeight;		// ゲージの高さ
	bool bUse;			// 使用しているかどうか
}RESULTLOGO;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitResultLogo(void);										// リザルトロゴの初期化処理
void UninitResultLogo(void);										// リザルトロゴの終了処理
void UpdateResultLogo(void);										// リザルトロゴの更新処理
void DrawResultLogo(void);											// リザルトロゴの描画処理
void SetVErtexResultLogo(int nCntResultLogo);						// リザルトロゴの頂点座標の設定
void SetResultLogo(D3DXVECTOR3 pos,float fHeight, RANK nTexType);	// ロゴの設置

#endif