//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE = 0,		// タイトル
	SOUND_LABEL_OE,				// チュートリアル
	SOUND_LABEL_BATTLE,			// バトル
	SOUND_LABEL_RESULT,			// リザルトBGM
	SOUND_LABEL_RANKING,		// ランキング
	SOUND_LABEL_SE_COUNTDOWN,	// カウントダウン
	SOUND_LABEL_SE_START,		// スタート
	SOUND_LABEL_SE_TIMEUP,		// タイムアップ
	SOUND_LABEL_SE_JUMP,		// ジャンプ音
	SOUND_LABEL_SE_STOMP,		// 踏む音
	SOUND_LABEL_SE_GET,			// フルーツ獲得音
	SOUND_LABEL_SE_GET_GOLD,	// 金色フルーツ獲得音
	SOUND_LABEL_SE_ENEMY,		// 敵に当たったとき
	SOUND_LABEL_SE_DAMAGE,		// ダメージを受けたとき
	SOUND_LABEL_SE_CROW,		// カラスの鳴き声
	SOUND_LABEL_SE_DOWN,		// 選択(下)
	SOUND_LABEL_SE_UP,			// 選択(上)
	SOUND_LABEL_SE_PAUSE_IN,	// ポーズ画面(入)
	SOUND_LABEL_SE_PAUSE_OUT,	// ポーズ画面(出)
	SOUND_LABEL_SE_OK,			// 決定
	SOUND_LABEL_SE_THROW,		// 投げる
	SOUND_LABEL_SE_DORAM,		// ドラムロール
	SOUND_LABEL_SE_RANK_1,		// 一位
	SOUND_LABEL_SE_RANK_2,		// 二位
	SOUND_LABEL_SE_RANK_3,		// 三位
	SOUND_LABEL_SE_RANK_4,		// 四位
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
