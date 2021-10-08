//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : 
//
//=============================================================================
#include "result.h"
#include "bg.h"
#include "result_logo.h"
#include "score.h"
#include "manager.h"
#include "input_keyboard.h"
#include "fade.h"
#include "logo.h"
#include "gamedata_keep_object.h"
#include "input_joypad.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CScore *CResult::m_pScore = NULL;
CScore *CResult::m_pMaxCombo = NULL;
CScore *CResult::m_pKillEnemy = NULL;

//=============================================================================
// CGameのコンストラクタ
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// CManagerのデストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 背景の生成
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		CBg::TEXTURETYPE_GAME);

	// リザルトロゴの生成
	CResultLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 100.0f, 0.0f), D3DXVECTOR2(500.0f, 100.0f));

	// ロゴ情報の取得
	CLogo **pResultLogo = CResultLogo::GetResultLogo();

	// スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, pResultLogo[CResultLogo::LOGOTYPE_YOURSCORE]->GetPosition().y + 100.0f,0.0f),
		D3DXVECTOR2(40.0f,80.0f),
		40.0f,
		CScore::SCOREPLACEMENT_RIGHT);

	// スコアの設定
	m_pScore->SetScore(CGameDataKeepObject::GetScore());

	// BGM
	CManager::GetSound()->Play(CSound::SOUND_LABEL_RESULT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// サウンド停止
	CManager::GetSound()->Stop();

	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	// CInputKeyboardインスタンスの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	// 画面遷移
	if ((pInputKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK)  || pJoypad->GetJoypadTrigger(PLAYER_1,CInputJoypad::JPINFO_SHOT_DOWN))&& 
		CFade::GetFade() == CFade::FADE_NONE)
	{
		CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{

}