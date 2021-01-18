//-------------------------------------------------------------------
//
// タイトルヘッダー (title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SELECTMODE_H_
#define _SELECTMODE_H_

#include "main.h"

//-------------------------------------------------------------------
// 列挙型
//-------------------------------------------------------------------
typedef enum
{
	PLAYMODE_SINGLE = 0,	// シングル
	PLAYMODE_MULTI,			// マルチ
	PLAYMODE_MAX,			// プレイモードの最大数
}PLAYMODE;

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
}SELECTMODE;

//-------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------
HRESULT InitSelectmode(void);			// セレクトモードの初期化処理
void UninitSelectmode(void);			// セレクトモードの終了処理
void UpdateSelectmode(void);			// セレクトモードの更新処理
void DrawSelectmode(void);				// セレクトモードの描画処理

void SelectMode(void);					// モード選択中
void DecideMode(void);					// モード選択完了

void SetVertexLogo(int nCntLogo);		// 頂点座標の設定
PLAYMODE GetPlayMode(void);				// 何のモードを選んだか

#endif
