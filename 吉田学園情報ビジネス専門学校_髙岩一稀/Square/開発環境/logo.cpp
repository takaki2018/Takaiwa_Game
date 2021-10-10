//=============================================================================
//
// ロゴ処理 [logo.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "logo.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "fade.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLogo::m_pTexture[TEXTURETYPE_MAX] = {};	// テクスチャ情報のポインタ

//=============================================================================
// CLogoのコンストラクタ
//=============================================================================
CLogo::CLogo()
{
	// メンバ変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_nTextureNumber = -1;
	m_bBlinking = false;
	m_nBlimkingInterval = 0;
	m_nBlinkingCounter = 0;
}

//=============================================================================
// CLogoのデストラクタ
//=============================================================================
CLogo::~CLogo()
{

}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CLogo::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo.png", &m_pTexture[TEXTURETYPE_TITLELOGO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_play.png", &m_pTexture[TEXTURETYPE_PLAY]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_tutorial.png", &m_pTexture[TEXTURETYPE_TUTORIAL]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_ranking.png", &m_pTexture[TEXTURETYPE_RANKING]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_option.png", &m_pTexture[TEXTURETYPE_OPTION]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_logo.png", &m_pTexture[TEXTURETYPE_SCORE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/highscore_logo.png", &m_pTexture[TEXTURETYPE_HIGHSCORE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &m_pTexture[TEXTURETYPE_PRESSENTER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_gamestart.png", &m_pTexture[TEXTURETYPE_GAMESTART]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_gameclear.png", &m_pTexture[TEXTURETYPE_GAMECLEAR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_gameover.png", &m_pTexture[TEXTURETYPE_GAMEOVER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_result.png", &m_pTexture[TEXTURETYPE_RESULTLOGO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_arcademode.png", &m_pTexture[TEXTURETYPE_ARCADEMODE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_yourscore.png", &m_pTexture[TEXTURETYPE_YOURSCORE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_newrecord.png", &m_pTexture[TEXTURETYPE_NEWRECORD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_maxcombo.png", &m_pTexture[TEXTURETYPE_MAXCOMBO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_killenemy.png", &m_pTexture[TEXTURETYPE_KILLENEMY]);

	return S_OK;
} 

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CLogo::Unload(void)
{
	// テクスチャの破棄
	for (int nCntLogo = 0; nCntLogo < TEXTURETYPE_MAX; nCntLogo++)
	{
		if (m_pTexture[nCntLogo] != NULL)
		{
			m_pTexture[nCntLogo]->Release();
			m_pTexture[nCntLogo] = NULL;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CLogo *CLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTextureNumber)
{
	// ポインタ変数の生成
	CLogo *pLogo;
	pLogo = new CLogo;

	// NULLチェック
	if (pLogo != NULL)
	{
		// 初期化処理
		pLogo->Init(pos, size, nTextureNumber);
	}

	return pLogo;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTextureNumber)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_size = size;
	m_nTextureNumber = nTextureNumber;

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLogo::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CLogo::Update(void)
{
	// 点滅するかどうか
	if (m_bBlinking == true)
	{
		Blinking();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLogo::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_nTextureNumber >= 0)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[m_nTextureNumber]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// ロゴの座標指定
//=============================================================================
void CLogo::SetPosition(D3DXVECTOR3 pos)
{
	// 座標の保存
	m_pos = pos;

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ロゴの頂点カラー指定
//=============================================================================
void CLogo::SetCol(D3DXCOLOR col)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの更新
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 点滅処理を始める
//=============================================================================
void CLogo::StartBlinking(int nBlimkingInterval)
{
	// 点滅を始める
	m_bBlinking = true;

	// 点滅間隔の保存
	m_nBlimkingInterval = nBlimkingInterval;
}

//=============================================================================
// 点滅処理
//=============================================================================
void CLogo::Blinking(void)
{
	// カウントの更新
	m_nBlinkingCounter++;

	// 点滅カウントと点滅間隔を割って余りが0のとき処理
	if (m_nBlinkingCounter % (m_nBlimkingInterval * 2) == 0)
	{
		// 色の変更
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (m_nBlinkingCounter % (m_nBlimkingInterval * 2) == m_nBlimkingInterval)
	{
		// 色の変更
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
}