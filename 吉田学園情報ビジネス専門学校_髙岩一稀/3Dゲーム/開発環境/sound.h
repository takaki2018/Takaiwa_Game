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
	SOUND_LABEL_RESULT_CLEAR,	// クリアBGM
	SOUND_LABEL_RESULT_OVER,	// 失敗BGM
	SOUND_LABEL_RANKING,		// ランキング
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_SHOOT,		// 発射音
	SOUND_LABEL_SE_DOWN,		// 選択(下)
	SOUND_LABEL_SE_PAUSE_IN,	// 選択(下)
	SOUND_LABEL_SE_PAUSE_OUT,	// 選択(下)
	SOUND_LABEL_SE_PLAYER_BRAEK,// 選択(下)
	SOUND_LABEL_SE_ITEM,		// 選択(下)
	SOUND_LABEL_SE_OK,			// 選択(下)
	SOUND_LABEL_SE_ALARM,		// 警報
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
