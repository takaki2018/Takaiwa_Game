//-------------------------------------------------------------------
//
// バレットヘッダー (bullet.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//-----------------------------------------------------------------
// 弾の状態
//-----------------------------------------------------------------
typedef enum
{
	BULLETCOLOR_WHITE = 0,	// 白
	BULLETCOLOR_RED,		// 赤
	BULLETCOLOR_BLUE,		// 青
	BULLETCOLOR_MAX,
}BULLETCOLOR;

typedef enum
{
	BULLETTYPE_PLAYER = 0,	// プレイヤーが使用
	BULLETTYPE_ENEMY,		// 敵が使用
	BULLETTYPE_MAX,
}BULLETTYPE;
//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	int nColType;			// 色のタイプ
	BULLETTYPE type;		// 種類
	bool bUse;				// 使用しているかどうか
} BULLET;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitBullet(void);																		// 弾の初期化処理
void UninitBullet(void);																		// 弾の終了処理
void UpdateBullet(void);																		// 弾の更新処理
void DrawBullet(void);																			// 弾の描画処
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,int nBulletColor,int nLife,BULLETTYPE type);	// 弾の設定
void SetVertexBullet(int nIdx);																	// 弾の頂点座標
BULLET *GetBullet(void);																		// 弾情報の取得
int GetBulletcol(void);																			// 現在の弾の色

#endif