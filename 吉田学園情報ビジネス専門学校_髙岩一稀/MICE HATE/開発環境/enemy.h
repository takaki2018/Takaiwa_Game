//-------------------------------------------------------------------
//
// エネミーヘッダー (enemy.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_ENEMY				(256)									// 敵の最大数
#define ENEMY_X					(15.0f)									// 中心座標のX座標
#define ENEMY_Y					(15.0f)									// 中心座標のY座標

//-------------------------------------------------------------------
// 列挙型
//-------------------------------------------------------------------
// 敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	// 通常状態
	ENEMYSTATE_ANYHIT,		// 違う弾が当たった状態
	ENEMYSTATE_ITEMEFFECT,	// アイテムの効果を受けている状態
	ENEMTSTATE_HITOBJECT,	// 敵かオブジェクトに当たった状態
	ENEMYSTATE_MAX,
}ENEMYSTATE;

// 敵の種類
typedef enum
{
	ENEMYTYPE_WHITE = 0,	// 白
	ENEMYTYPE_RED,			// 赤
	ENEMYTYPE_BLUE,			// 青
	ENEMYTYPE_MAX,
}ENEMYTYPE;

//-------------------------------------------------------------------
// 敵の構造体
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	ENEMYTYPE nType;	// 種類
	D3DXCOLOR col;		// 色
	ENEMYSTATE state;	// 状態
	float fAngle;		// オブジェクトまでの角度
	int nCounterState;	// 状態カウンタ
	int nCntBullet;		// 弾を撃つカウント
	int nLife;			// 体力
	int nCntFlash;		// 警告の回数
	int nFlashCounter;	// フラッシュカウンター
	bool bUse;			// 使用しているかどうか
} ENEMY;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitEnemy(void);														// 敵の初期化処理
void UninitEnemy(void);															// 敵の終了処理
void UpdateEnemy(void);															// 敵の更新処理
void DrawEnemy(void);															// 敵の描画処理

void EnemyInfoWhite(int nCntEnemy);												// 敵情報(白)
void EnemyInfoRed(int nCntEnemy);												// 敵情報(赤)
void EnemyInfoBlue(int nCntEnemy);												// 敵情報(青)
void EnemyState(int nCntEnemy);													// 敵の状態

bool HitEnemy(int nIdx, int nCntBullet);										// 敵の当たり判定がtrueのとき
bool GetHitObject(void);														// オブジェクトの当たり判定の取得

void SetEnemyRandam(void);														// 敵をランダムに配置
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, float angle, ENEMYTYPE nType);	// 敵の設定
void SetVertexEnemy(int nIdx);													// 敵の頂点座標
ENEMY *GetEnemy(void);															// 敵情報の取得
int GetEnemyIn(void);															// 敵が範囲内に来た数の取得

#endif