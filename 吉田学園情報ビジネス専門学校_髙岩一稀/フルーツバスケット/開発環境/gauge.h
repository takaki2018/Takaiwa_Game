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
void RankingGauge(void);			// ランキングの確定
float GetMoveGauge(int nCnt);	    // ゲージの移動量の取得
bool GetbGauge(void);				// ゲージが上がりきったかどうか

#endif
