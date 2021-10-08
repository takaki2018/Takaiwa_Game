//=============================================================================
//
// 敵発生UI処理 [spawn_enemyUI.cpp]
// Author : 
//
//=============================================================================
#include "spawn_enemyUI.h"
#include "enemy.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
#define INTERVAL_BASE		(40)		// インターバルの基準値
#define MOVEALPHA_BASE		(0.05f)		// α値移動量の基準値

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSpawnEnemyUI::m_pTexture = NULL;

//=============================================================================
// CSpawnEnemyUIのコンストラクタ
//=============================================================================
CSpawnEnemyUI::CSpawnEnemyUI(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CSpawnEnemyUIのデストラクタ
//=============================================================================
CSpawnEnemyUI::~CSpawnEnemyUI()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CSpawnEnemyUI::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spawn_enemy.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CSpawnEnemyUI::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CSpawnEnemyUI *CSpawnEnemyUI::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, int nCntSpawnInterval, int nEnemyType)
{
	// ポインタ変数の生成
	CSpawnEnemyUI *pSpawnEnemyUI;
	pSpawnEnemyUI = new CSpawnEnemyUI(CScene::PRIORITY_ENEMY);

	// NULLチェック
	if (pSpawnEnemyUI != NULL)
	{
		// 初期化処理
		pSpawnEnemyUI->Init(pos, size,fMoveAI,nCntSpawnInterval,nEnemyType);

		// テクスチャ設定
		pSpawnEnemyUI->BindTexture(m_pTexture);

		// オブジェクトの種類設定
		pSpawnEnemyUI->SetObjType(CScene::OBJTYPE_SPAWNENEMYUI);
	}

	return pSpawnEnemyUI;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpawnEnemyUI::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, int nCntSpawnInterval, int nEnemyType)
{
	// 変数の初期化(基準値)
	m_nCntSpawnInterval = nCntSpawnInterval;
	m_nCounter = 0;			
	m_nCntFlash = 0;
	m_nEnemyType = nEnemyType;

	// 基準値と引数を比較し、α値の移動量を決める
	float fInterbalBase = (float)INTERVAL_BASE / (float)m_nCntSpawnInterval;
	m_fMoveAlpha = fInterbalBase * MOVEALPHA_BASE;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// 頂点カラー情報の取得
	D3DXCOLOR col = GetCol();

	// α値を設定
	col.a = m_fMoveAlpha;

	// 頂点カラーの設定
	SetCol(col);

	// 基本移動量の設定
	m_fMoveAI = fMoveAI;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSpawnEnemyUI::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSpawnEnemyUI::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();

	// カウンターの更新
	m_nCounter++;

	// 点滅処理
	Blinking();

	// 点滅回数が3回になったら敵を出現させる
	if (m_nCntFlash == 3)
	{
		CreateEnemy();

		return;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CSpawnEnemyUI::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 点滅処理
//=============================================================================
void CSpawnEnemyUI::Blinking(void)
{
	// 頂点カラー情報の取得
	D3DXCOLOR col = GetCol();

	// 条件の数値を計算
	float fCommon = (float)(m_nCounter % m_nCntSpawnInterval);
	float fHalf = (float)m_nCntSpawnInterval / 2.0f;

	// 点滅処理
	if (fCommon < fHalf)
	{
		// α値の増加
		col.a += m_fMoveAlpha;

		if (col.a >= 1.0f)
		{
			col.a = 1.0f;
		}
	}
	else if (fCommon >= fHalf && fCommon < m_nCntSpawnInterval)
	{
		// α値の減算
		col.a -= m_fMoveAlpha;

		if (col.a <= 0.0f)
		{
			col.a = 0.0f;
		}
	}
	if (fCommon == m_nCntSpawnInterval - 1)
	{
		// 点滅回数をカウント
		m_nCntFlash++;
	}

	// 頂点カラーの設定
	SetCol(col);
}

//=============================================================================
// 敵の生成処理
//=============================================================================
void CSpawnEnemyUI::CreateEnemy(void)
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// サイズの取得
	D3DXVECTOR2 size = GetSize();

	switch (m_nEnemyType)
	{
	case CEnemy::ENEMYTYPE_NORMAL:
		// 敵の生成
		CEnemy::Create(pos, size,m_fMoveAI, CEnemy::ENEMYTYPE_NORMAL);
		break;
	}

	// 削除
	Uninit();
}