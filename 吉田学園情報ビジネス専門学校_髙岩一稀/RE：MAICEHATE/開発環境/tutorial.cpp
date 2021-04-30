//==============================================================================================================
//
// チュートリアル画面 (tutorial.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial.h"
#include "tutorial_bg.h"
#include "tutorial_image.h"
#include "tutorial_marker.h"
#include "tutorial_press_ok.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画面の初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 各初期化処理ができたかどうか
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorial(void)
{
	// チュートリアル背景の初期化処理
	InitTutorialBg();

	// チュートリアル画像の初期化処理
	InitTutorialImage();

	// チュートリアルマーカーの初期化処理
	InitTutorialMarker();

	// チュートリアル用PressOKの初期化処理
	InitTutorialPressOK();

	// サウンド
	PlaySound(SOUND_LABEL_OE);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画面の終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitTutorial(void)
{
	// サウンドの一時停止
	StopSound();

	// チュートリアル用PressOKの終了処理
	UninitTutorialPressOK();

	// チュートリアルマーカーの終了処理
	UninitTutorialMarker();

	// チュートリアル画像の終了処理
	UninitTutorialImage();

	// チュートリアル背景の終了処理
	UninitTutorialBg();
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画面の更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorial(void)
{
	// ゲーム画面に遷移するかどうか
	TutorialPressOK tutorialPressOK = GetPressOKInfo();

	// チュートリアル背景の更新処理
	UpdateTutorialBg();

	// チュートリアル用PressOKの更新処理
	UpdateTutorialPressOK();

	// ゲーム画面に遷移するかどうか
	if (tutorialPressOK.bGameTrasition == true)
	{
		SetFade(FADE_OUT,MODE_GAME);
	}
	else if (tutorialPressOK.bPressOK == false)
	{
		// チュートリアル画像の更新処理
		UpdateTutorialImage();

		// チュートリアルマーカーの更新処理
		UpdateTutorialMarker();
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画面の描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawTutorial(void)
{
	// チュートリアル背景の描画処理
	DrawTutorialBg();

	// チュートリアル画像の描画処理
	DrawTutorialImage();

	// チュートリアルマーカーの描画処理
	DrawTutorialMarker();

	// チュートリアル用PressOKの描画処理
	DrawTutorialPressOK();
}