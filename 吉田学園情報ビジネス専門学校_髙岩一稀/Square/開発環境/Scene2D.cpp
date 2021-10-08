//=============================================================================
//
// シーン2D処理 [scene2D.cpp]
// Author : 
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_X		(1)							// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)							// アニメーションパターンNo.の縦の最大数

//=============================================================================
// CSceneのコンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	// メンバ変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fAngle = atan2f(m_fWidth / 2.0f, m_fHeight / 2.0f);
	m_fLength = sqrtf(m_fWidth / 2.0f * m_fWidth / 2.0f + m_fHeight / 2.0f * m_fHeight / 2.0f);
}

//=============================================================================
// CSceneのデストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos , D3DXVECTOR2 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = size.x;
	m_fHeight = size.y;
	m_fAngle = atan2f(m_fWidth / 2.0f, m_fHeight /2.0f);
	m_fLength = sqrtf(m_fWidth / 2.0f * m_fWidth /2.0f + m_fHeight /2.0f * m_fHeight / 2.0f);

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
	pVtx[0].pos.x = m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y - cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y - cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
	pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	if (m_bDeath == true)
	{
		// 頂点バッファの開放
		if (m_pVtxBuff != NULL)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// テクスチャ情報の割り当て
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	 // テクスチャ情報を割り当てる
	m_pTexture = pTexture;
}

//=============================================================================
// 座標設定処理
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	// 座標設定
	m_pos = pos;

	// 頂点座標の設定
	SetVertex();
}

//=============================================================================
// サイズ設定処理
//=============================================================================
void CScene2D::SetSize(D3DXVECTOR2 size)
{
	// 座標設定
	m_fWidth = size.x;
	m_fHeight = size.y;

	// 角度と対角線の長さを再計算する
	m_fAngle = atan2f(m_fWidth / 2.0f, m_fHeight / 2.0f);
	m_fLength = sqrtf(m_fWidth / 2.0f * m_fWidth / 2.0f + m_fHeight / 2.0f * m_fHeight / 2.0f);

	// 頂点座標の設定
	SetVertex();
}

//=============================================================================
// 回転設定処理
//=============================================================================
void CScene2D::SetRotateVertex(float fAngle)
{
	// 回転設定
	m_rot.z = fAngle;

	D3DXVECTOR3 pos[4];
	pos[0].x = m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength;
	pos[0].y = m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength;
			   
	pos[1].x = m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength;
			   
	pos[2].y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;

	float x = pos[1].x - pos[0].x;
	float y = pos[0].y - pos[2].y;

	if (x < 0.0f)
	{
		x *= -1.0f;
	}
	if (y < 0.0f)
	{
		y *= -1.0f;
	}
	m_fWidth = x;
	m_fHeight = y;

	// 頂点座標の設定
	SetVertex();
}

//=============================================================================
// 頂点座標の設定処理
//=============================================================================
void CScene2D::SetVertex(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	pVtx[0].pos.x = m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y - cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y - cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点カラーの設定処理
//=============================================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	// 変数の更新
	m_col = col;

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の更新
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ座標の設定処理
//=============================================================================
void CScene2D::SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の更新
	pVtx[0].tex = D3DXVECTOR2(texUV.x, texUV.y + 1.0f / texAnimetionSheets.y);
	pVtx[1].tex = D3DXVECTOR2(texUV.x, texUV.y);
	pVtx[2].tex = D3DXVECTOR2(texUV.x + 1.0f / texAnimetionSheets.x, texUV.y + 1.0f / texAnimetionSheets.y);
	pVtx[3].tex = D3DXVECTOR2(texUV.x + 1.0f / texAnimetionSheets.x, texUV.y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}