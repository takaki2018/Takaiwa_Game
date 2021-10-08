//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"

#include "renderer.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "sound.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "fade.h"
#include "pause.h"
#include "logo.h"
#include "bg.h"
#include "number.h"
#include "tile.h"
#include "gamedata_keep_object.h"
#include "count_down.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CSound *CManager::m_pSound = NULL;
CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CManager::MODE CManager::m_mode = MODE_TITLE;
CFade *CManager::m_pFade = NULL;
bool CManager::m_bPause = false;
bool CManager::m_bTutorial = false;
CGameDataKeepObject *CManager::m_pGemeData = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;

//=============================================================================
// CManagerのコンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// CManagerのデストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, bool bWindow)
{
	// 現在時刻によってランダムに設定する
	srand((unsigned)time(NULL));

	// レンダラーの初期化処理
	m_pRenderer = new CRenderer;

	if (m_pRenderer != NULL)
	{
		m_pRenderer->Init(hWnd, bWindow);
	}

	// キーボードの初期化処理
	m_pInputKeyboard = new CInputKeyboard;

	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	// ジョイパッド情報の初期化処理
	m_pInputJoypad = new CInputJoypad;

	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Init(hInstance, hWnd);
	}

	// サウンドの初期化処理
	m_pSound = new CSound;

	if (m_pSound != NULL)
	{
		m_pSound->Init(hWnd);
	}

	// フェードの初期化処理
	m_pFade = new CFade;

	if (m_pFade != NULL)
	{
		m_pFade->Init(m_mode);
	}

	// ゲームデータを保存するオブジェクトの作成
	m_pGemeData = new CGameDataKeepObject;

	if (m_pGemeData != NULL)
	{
		m_pGemeData->Init();
	}

	// カメラの初期化処理
	m_pCamera = new CCamera;

	if (m_pCamera != NULL)
	{
		m_pCamera->Init();
	}

	// ライトの初期化処理
	m_pLight = new CLight;

	if (m_pLight != NULL)
	{
		m_pLight->Init();
	}

#ifdef _DEBUG
	CDebugProc *pDebug = new CDebugProc;
	pDebug->Init();
#endif

	// テクスチャのロード
	LoadTexture();

	// モードの呼び出し
	SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// テクスチャのアンロード
	UnloadTexture();

	// サウンドの終了処理
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// ジョイパッドの終了処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// キーボードの終了処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// フェードの終了処理
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	// レンダラの終了処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// オブジェクトの終了処理
	CScene::ReleaseAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:
		break;

	case MODE_GAME:
		// ポーズ処理
		if ((m_pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_PAUSE) || 
			m_pInputJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_PAUSE)) && 
			CCountDown::GetFinishCountDown())
		{
			SetPause();
		}
		break;
	}

	// キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// ジョイパッドの更新処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}

	// レンダラの更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// オブジェクトの更新処理
	CScene::UpdateAll();

	// フェードの更新処理
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	m_pCamera->Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// レンダラの描画処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{// タイトル画面の終了処理
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
		}
		// チュートリアル情報の終了処理
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
		}
		break;

		// ゲーム画面の終了処理
	case MODE_GAME:
		if (m_pGame!= NULL)
		{
			m_pGame->Uninit();
		}
		break;

		// リザルト画面の終了処理
	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
		}
		break;
	}
	// オブジェクトの削除
	CScene::ReleaseAll();

	// ポーズ状態をfalseにする
	SetPause(false);

	// モードの移行
	m_mode = mode;

	switch (mode)
	{// タイトル画面の初期化処理
	case MODE_TITLE:
		m_pTitle = new CTitle;
		if (m_pTitle != NULL)
		{
			m_pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
		break;

		// ゲーム画面の初期化処理
	case MODE_GAME:
		m_pGame = new CGame;
		if (m_pGame != NULL)
		{
			m_pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
		break;

		// リザルト画面の初期化処理
	case MODE_RESULT:
		m_pResult = new CResult;
		if (m_pResult != NULL)
		{
			m_pResult->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
		break;
	}
}

//=============================================================================
// テクスチャのロード
//=============================================================================
void CManager::LoadTexture(void)
{
	// ロゴのテクスチャロード
	CLogo::Load();

	// 背景のテクスチャロード
	CBg::Load();

	// 数字のテクスチャロード
	CNumber::Load();

	// タイルのテクスチャロード
	CTile::Load();

	// チュートリアルのテクスチャロード
	CTutorial::Load();
}

//=============================================================================
// テクスチャのアンロード
//=============================================================================
void CManager::UnloadTexture(void)
{
	// ロゴのテクスチャアンロード
	CLogo::Unload();

	// 背景のテクスチャアンロード
	CBg::Unload();

	// 数字のテクスチャアンロード
	CNumber::Unload();

	// タイルのテクスチャアンロード
	CTile::Unload();

	// チュートリアルのテクスチャアンロード
	CTutorial::Unload();
}

//=============================================================================
// レンダラー情報の取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// キーボード情報の取得
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// ジョイパッド情報の取得
//=============================================================================
CInputJoypad *CManager::GetInputJoypad(void)
{
	return m_pInputJoypad;
}

//=============================================================================
// ポーズの処理
//=============================================================================
void CManager::SetPause(void)
{
	if (m_pFade->GetFade() == CFade::FADE_NONE)
	{
		m_bPause = m_bPause ? false : true;

		if (m_bPause == true)
		{
			m_pSound->Play(CSound::SOUND_LABEL_SE_PAUSE_IN);
		}
		else if (m_bPause == false)
		{
			m_pSound->Play(CSound::SOUND_LABEL_SE_PAUSE_OUT);
		}
	}
}