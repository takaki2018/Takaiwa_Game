//-----------------------------------------------------------------
//
// リザルト背景 (result_bg.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "result_bg.h"
#include "bg.h"
#include "block.h"
#include "crow.h"
#include "wood.h"
#include "wood_back.h"
#include "result_score.h"
#include "result_score_bg.h"

//-----------------------------------------------------------------
// リザルト背景の初期化処理
//-----------------------------------------------------------------
HRESULT InitResultBg(void)
{
	// 背景の初期化処理
	InitBg();

	// カラスの初期化処理
	InitCrow();

	// 木の裏の初期化処理
	InitWoodBack();

	// 木の初期化処理
	InitWood();

	// ブロックの初期化処理
	InitBlock();

	// リザルトスコア背景の初期化処理
	InitResultScoreBg();

	// リザルトスコアの初期化処理
	InitResultScore();

	// ブロックの配置
	SetBlockInfo(MODESTATE_RESULT);

	return S_OK;
}

//-----------------------------------------------------------------
// リザルト背景の終了処理
//-----------------------------------------------------------------
void UninitResultBg(void)
{
	// リザルトスコアの終了処理
	UninitResultScore();

	// リザルトスコア背景の終了処理
	UninitResultScoreBg();

	// ブロックの終了処理
	UninitBlock();

	// 木の終了処理
	UninitWood();

	// 木の裏の終了処理
	UninitWoodBack();

	// カラスの終了処理
	UninitCrow();

	// 背景の終了処理
	UninitBg();
}

//-----------------------------------------------------------------
// リザルト更新の更新処理
//-----------------------------------------------------------------
void UpdateResultBg(void)
{
	// 背景の更新処理
	UpdateBg();

	// カラスの更新処理
	UpdateCrow();

	// 木の裏の更新処理
	UpdateWoodBack();

	// 木の更新処理
	UpdateWood();

	// ブロックの更新処理
	UpdateBlock();

	// リザルトスコア背景の更新処理
	UpdateResultScoreBg();

	// リザルトスコアの更新処理
	UpdateResultScore();
}

//-----------------------------------------------------------------
// リザルト背景の描画処理
//-----------------------------------------------------------------
void DrawResultBg(void)
{
	// 背景の描画処理
	DrawBg();

	// カラスの描画処理
	DrawCrow();

	// 木の裏の描画処理
	DrawWoodBack();

	// ブロックの描画処理
	DrawBlock();

	// 木の描画処理
	DrawWood();

	// リザルトスコア背景の描画処理
	DrawResultScoreBg();

	// リザルトスコアの描画処理
	DrawResultScore();
}