//=============================================================================
//
// カウントダウン処理 [count_down.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "count_down.h"
#include "number.h"
#include "logo.h"
#include "ui_bg.h"
#include "UI.h"
#include "manager.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
bool CCountDown::m_bFinishCountDown = false;	// カウントダウンが終わったかどうか

//=============================================================================
// CCountDownのコンストラクタ
//=============================================================================
CCountDown::CCountDown(int nPriority) : CScene(nPriority)
{
	// メンバ変数の初期化
	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber] = NULL;
	}
	m_pLogo = NULL;
	m_bFinishCountDown = false;
	m_nCntCountDown = 0;
	m_nNumCountDown = MAX_NUMBER;
}

//=============================================================================
// CCountDownのデストラクタ
//=============================================================================
CCountDown::~CCountDown()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CCountDown::Load(void)
{
	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CCountDown::Unload(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CCountDown *CCountDown::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CCountDown *pCountDown;
	pCountDown = new CCountDown(CScene::PRIORITY_COUNTDOWN);

	// NULLチェック
	if (pCountDown != NULL)
	{
		// 初期化処理
		pCountDown->Init(pos, size);

		// オブジェクトタイプの設定
		pCountDown->SetObjType(OBJTYPE_COUNTDOWN);
	}

	return pCountDown;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCountDown::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 数字情報の初期化
	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create(pos,size);

		// 数字のセット
		m_apNumber[nCntNumber]->SetNumber(nCntNumber + 1);

		// 色の初期化
		if (nCntNumber + 1 == m_nNumCountDown)
		{
			m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
	// ロゴ情報の初期化
	m_pLogo = CLogo::Create(pos,D3DXVECTOR2(400.0f,70.0f),CLogo::TEXTURETYPE_GAMESTART);

	// 色の初期化
	m_pLogo->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCountDown::Uninit(void)
{
	// 数字情報の終了処理
	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber]->Uninit();
	}

	// ロゴ情報の終了処理
	m_pLogo->Uninit();

	// ロゴ背景の終了処理
	if (m_bFinishCountDown == true)
	{
		CUI::GetLogoBg()->Uninit();
	}

	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCountDown::Update(void)
{
	// カウントダウンの更新
	if (m_nCntCountDown % 60 == 0)
	{
		//　カウントダウンカウントを減らす
		m_nNumCountDown--;

		// カウントダウンが0以上のとき
		if (m_nNumCountDown > 0)
		{
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				// カウントが一致していたらその数字を見えるようにする
				if ((nCntNumber + 1) == m_nNumCountDown)
				{
					m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{
					m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
			}
			// 効果音
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (m_nNumCountDown == 0)
		{
			// 見えなくする
			m_apNumber[m_nNumCountDown]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			// 0のときロゴを見えるようにする
			m_pLogo->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// 効果音
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_GAME_START);
		}
		else
		{
			// aBGM
			CManager::GetSound()->Play(CSound::SOUND_LABEL_GAME);

			// カウントダウンを終えたことを伝える
			m_bFinishCountDown = true;

			// 終了処理
			Uninit();
		}
	}
	// カウントの増加
	m_nCntCountDown++;
}

//=============================================================================
// 描画処理
//=============================================================================
void CCountDown::Draw(void)
{
	// 数字情報の描画処理
	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber]->Draw();
	}

	// ロゴ情報の描画処理
	m_pLogo->Draw();
}