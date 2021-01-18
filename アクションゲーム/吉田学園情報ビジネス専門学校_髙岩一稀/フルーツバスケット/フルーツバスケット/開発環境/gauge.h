//-------------------------------------------------------------------
//
// ゲージヘッダー (gauge.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

//-------------------------------------------------------------------
// 列挙型
//-------------------------------------------------------------------
typedef enum
{
	RANKING_FIRST = 0,		// 一位
	RANKING_SECOND,			// 二位
	RANKING_THIRD,			// 三位
	RANKING_FOURTH,			// 四位
	RANKING_FIFTH,			// 五位
	RANKING_MAX,
}RANKING;					// 順位

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float fMoveHeight;		// 高さの移動量
	int nScore;				// スコアを保存
	int nRank;				// 順位
	int nCntScore;			// スコアのカウント
	int nCntTex;			// 何位のテクスチャをはるか
	bool bReach;			// 到達したかどうか
}GAUGE;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitGauge(void);			// ゲージの初期化処理
void UninitGauge(void);				// ゲージの終了処理
void UpdateGauge(void);				// ゲージの更新処理
void DrawGauge(void);				// ゲージの描画処理
float GetMoveGauge(int nCnt);	    // ゲージの移動量の取得
bool GetbGauge(void);				// ゲージが上がりきったかどうか
void RankingGauge(void);					// ランキングの確定

#endif
