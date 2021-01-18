//-------------------------------------------------------------------
//
// フルーツヘッダー (fruits.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _FRUITS_H_
#define _FRUITS_H_

#include "main.h"

//-------------------------------------------------------------------
// 列挙型
//-------------------------------------------------------------------
typedef enum
{
	FRUITSTYPE_APPLE = 0,	// リンゴ
	FRUITSTYPE_BLUEAPPLE,	// 青リンゴ
	FRUITSTYPE_PEACH,		// ピーチ
	FRUITSTYPE_ORANGE,		// オレンジ
	FRUITSTYPE_GOLDAPPLE,	// 金リンゴ
	FRUITSTYPE_MAX,			// フルーツのの種類数
}FRUITSTYPE;				// フルーツの種類

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posOld;	// 前の位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 回転
	int nCntBound;		// 跳ね返った回数
	float fWidth;		// 幅
	float fHeight;		// 高さ
	FRUITSTYPE nType;	// タイプ
	bool bLand;			// ブロックに乗ったかどうか
	bool bUse;			// 使用しているかどうか
}FRUITS;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitFruits(void);											// フルーツの初期化処理
void UninitFruits(void);											// フルーツの終了処理
void UpdateFruits(void);											// フルーツの更新処理
void DrawFruits(void);												// フルーツの描画処理

void TitleFruits(void);												// タイトル画面のフルーツ
void GameFruits(void);												// ゲーム画面のフルーツ

void SetFruitsTitle(void);											// タイトル画面の配置情報
void SetFruitsGame(void);											// ゲーム画面のフルーツの配置情報

void HitFruits(int nCntFruits);										// フルーツの当たり判定

void SetFruits(D3DXVECTOR3 pos, D3DXVECTOR3 move, FRUITSTYPE type);	// フルーツの配置
void SetVertexFruits(int nIdx);										// 頂点座標の設定
FRUITS *GetFruits(void);											// フルーツ情報の取得
bool GetbTitleFruits(void);											// タイトル画面のフルーツカウントの取得
int GetTitleFruitsCnt(void);										// タイトル画面のフルーツカウントの取得

#endif
