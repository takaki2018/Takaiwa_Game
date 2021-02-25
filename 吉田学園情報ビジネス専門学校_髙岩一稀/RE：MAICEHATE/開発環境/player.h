//-------------------------------------------------------------------
//
// プレイヤーヘッダー (player.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "motion.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_FILENAME (64)		// Xファイルネームの最大文字数

//-------------------------------------------------------------------
// プレイヤー状態(列挙型)
//-------------------------------------------------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		// 通常
	PLAYERSTATE_FLOATING,		// 浮遊
	PLAYERSTATE_DAMAGE,			// ダメージ
	PLAYERSTATE_MAX,			// 状態の最大数
}PLAYERSTATE;

typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// ニュートラルモーション
	MOTIONTYPE_MOVE,		// 移動モーション
	MOTIONTYPE_JUMP,		// ジャンプモーション
	MOTIONTYPE_LAND,		// 着地モーション
	MOTIONTYPE_DAMAGE,		// のけぞりモーション
	MOTYONTYPE_MAX,			// モーションの最大値
}MOTIONTYPE;				// モーションの種類

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMesh;				// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;			// マテリアル(材質情報)へのポインタ
	DWORD nNumMat = 0;				// マテリアルの数
	LPDIRECT3DTEXTURE9 pTexture;	// テクスチャへのポインタ
	char aFileName[MAX_FILENAME];	// ファイルの名前
	D3DXVECTOR3 pos;				// 中心座標
	D3DXVECTOR3 rot;				// 回転角
	D3DXMATRIX mtxWorld;			// ワールドマトリックス
	int nIdxModelParent;			// 親モデルのインデックス
	D3DXVECTOR3 minVecModel;		// 頂点の最小値
	D3DXVECTOR3 maxVecModel;		// 頂点の最大値
}PlayerModel;

typedef struct
{
	D3DXVECTOR3 pos;							// 中心座標
	D3DXVECTOR3 posOld;							// 前の中心座標
	D3DXVECTOR3 rot;							// 回転角
	D3DXVECTOR3 rotDest;						// 目的の回転角
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 move;							// 移動量
	bool bJump;									// ジャンプしたかどうか
	PLAYERSTATE state;							// プレイヤーの状態
	int nStateCounter;							// 状態カウンタ
	PlayerModel aModel[10];						// モデル
	int nNumModel;								// モデル数
	D3DXVECTOR3 minVecPlayer;					// 頂点の最小値
	D3DXVECTOR3 maxVecPlayer;					// 頂点の最大値
	int nIdx;									// 何番目の影か
	D3DXVECTOR3 posParent;						// 親の座標
	MOTION_INFO aMotionInfo[MOTYONTYPE_MAX];	// モーション情報
	int nNumMotion;								// モーション数
	MOTIONTYPE MotionType;						// モーションタイプ
	MOTIONTYPE MotionTypeOld;					// 前回のモーションタイプ
	bool bLoopMotion;							// ループするかどうか
	int nNumKey;								// キー数
	int nKey;									// キーNo.
	int nCounterMotion;							// モーションカウンタ
	bool bFall;									// 落ちたかどうか
}Player;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitPlayer(void);			// プレイヤーの初期化処理
void UninitPlayer(void);			// プレイヤーの終了処理
void UpdatePlayer(void);			// プレイヤーの更新処理
void DrawPlayer(void);				// プレイヤーの描画処理

Player *GetPlayer(void);			// プレイヤー情報の取得

void StateManagerPlayer(void);		// プレイヤー状態の判別
void PlayerStateNormal(void);		// プレイヤー状態が通常の時
void PlayerStateFloating(void);		// プレイヤー状態が浮遊の時
void PlayerStateDamage(void);		// プレイヤーがダメージ状態のとき

void MovePlayer(void);				// プレイヤーの移動処理
void MovePlayerKeyboard(void);		// プレイヤーの移動処理(キーボード)
void MovePlayerGamepad(void);		// プレイヤーの移動処理(ゲームパッド)

void VecPlayer(void);				// プレイヤーの頂点座標の補正

void PlayerMotion(void);			// プレイヤモーションの設定

#endif