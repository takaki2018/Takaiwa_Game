//---------------------------------------------------------------- -
//
// フェード (fade.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "fade.h"
#include "renderer.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define REMOVE_ALFHA		(0.01f)				// α値の増減量

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1) // 頂点フォーマット
#define MAX_VERTEX (4)												// 頂点の数

//-----------------------------------------------------------------
// 構造体定義
//-----------------------------------------------------------------
typedef  struct
{
	D3DXVECTOR3 pos; // 頂点座標
	float rhw;	     // 1.0で固定
	D3DCOLOR col;    // 頂点カラー
	D3DXVECTOR2 tex;
} VERTEX_2D;

//-----------------------------------------------------------------
// 静的メンバ変数宣言
//-----------------------------------------------------------------
CFade::FADE CFade::m_fade = CFade::FADE_IN;
CManager::MODE CFade::m_modeNext = CManager::MODE_TITLE;

//-----------------------------------------------------------------
// フェードの初期化処理
//-----------------------------------------------------------------
HRESULT CFade::Init(CManager::MODE modeNext)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_fade = CFade::FADE_IN;
	m_modeNext = modeNext;
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuffFade,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuffFade->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// フェードの終了処理
//-----------------------------------------------------------------
void CFade::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuffFade != NULL)
	{
		m_pVtxBuffFade->Release();
		m_pVtxBuffFade = NULL;
	}
}

//-----------------------------------------------------------------
// フェードの更新処理
//-----------------------------------------------------------------
void CFade::Update(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// 頂点バッファをアンロックする
	m_pVtxBuffFade->Unlock();

	// フェード情報の分岐
	if (m_fade == FADE_IN)
	{// フェードインの時
		m_colorFade.a -= REMOVE_ALFHA;

		if (m_colorFade.a <= 0.0f)
		{// 0.0fになったとき何もしない
			m_fade = FADE_NONE;
		}
	}
	else if (m_fade == FADE_OUT)
	{// フェードアウトのとき
		m_colorFade.a += REMOVE_ALFHA;

		if (m_colorFade.a >= 1.0f)
		{// 1.0fになったときフェードイン
			m_fade = FADE_IN;

			CManager::SetMode(m_modeNext);
		}
	}
	else if (m_fade == FADE_NONE)
	{// 何もしていないとき
		m_colorFade.a = 0.0f;
	}
}

//-----------------------------------------------------------------
// フェードの描画処理
//-----------------------------------------------------------------
void CFade::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------
// フェード設定
//-----------------------------------------------------------------
void CFade::SetFade(FADE fade, CManager::MODE modeNext)
{
	m_fade = fade;
	m_modeNext = modeNext;
}

//-----------------------------------------------------------------
// フェード情報の取得
//-----------------------------------------------------------------
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}