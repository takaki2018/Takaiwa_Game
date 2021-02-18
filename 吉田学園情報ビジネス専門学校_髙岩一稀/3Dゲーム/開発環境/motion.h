//-------------------------------------------------------------------
//
// モーションヘッダー (motion.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	float fPosX;			// 位置X
	float fPosY;			// 位置Y
	float fPosZ;			// 位置Z
	float fRotX;			// 向きX
	float fRotY;			// 向きY
	float fRotZ;			// 向きZ
}KEY;						// キーの構造体

typedef struct
{
	int nFrame;				// フレーム数
	KEY aKey[10];			// 各モデルのキー要素
}KEY_INFO;					// キーの構造体

typedef struct
{
	bool bLoop;				// ループするかどうか
	int nNumKey;			// キーの総数
	KEY_INFO aKeyInfo[5];	// キー情報
}MOTION_INFO;				// モーションの構造体

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
void SetMotion(void);				// モーションの決定処理
void NeutralMotion(void);			// ニュートラルモーション

void LoadPlayerdata(void);				// プレイヤー情報の読み込み

#endif