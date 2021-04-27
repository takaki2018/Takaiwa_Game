//==============================================================================================================
//
// メインヘッダー (main.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>
#include "d3dx9.h"					// 描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"					// 入力処理に必要
#include "xaudio2.h"				// サウンド再生に必要
#include <Xinput.h>					// Xinput用

//--------------------------------------------------------------------------------------------------------------
// ライブラリのリンク
//--------------------------------------------------------------------------------------------------------------
#pragma comment(lib,"d3d9.lib")   // 描画処理に必要
#pragma comment(lib,"d3dx9.lib")  // 拡張ライブラリ
#pragma comment(lib,"dxguid.lib") // コンポーネント使用に必要
#pragma comment(lib,"dinput8.lib") // 入力処理に必要
#pragma comment(lib,"winmm.lib")
#pragma comment (lib, "xinput.lib") // Xinput用

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define SCREEN_WIDTH		(1280)													// スクリーンの幅
#define SCREEN_HEIGHT		(720)													// スクリーンの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1)					// 頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット
#define CLASS_NAME "WindowClass"													// ウィンドウクラスの名前
#define WINDOW_NAME "RE:MICE HATE"													// ウィンドウの名前
#define MAX_VERTEX			(4)														// 頂点の数

//------------------------------------------------------------------------------------------------------------
// 構造体の定義
//------------------------------------------------------------------------------------------------------------
typedef  struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 1.0で固定
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;
} VERTEX_2D;

typedef  struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;
} VERTEX_3D;

typedef enum
{
	MODE_TITLE = 0,		// タイトル画面
	MODE_TUTORIAL,		// ロード画面
	MODE_GAME,			// ゲーム画面
	MODE_RESULT,		// リザルト画面
	MODE_RANKING,		// ランキング画面
	MODE_MAX,
}MODE;

//------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//------------------------------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);	// デバイスの獲得
void SetMode(MODE mode);			// モードの設定
MODE GetMode(void);					// モードの獲得

#endif

