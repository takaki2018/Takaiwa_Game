//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : 
//
//=============================================================================
#include "renderer.h"
#include "scene.h"	
#include "manager.h"
#include "fade.h"
#include "debugproc.h"
#include "input_keyboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1) // 頂点フォーマット
#define MAX_VERTEX (4)												// 頂点の数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef  struct
{
	D3DXVECTOR3 pos; // 頂点座標
	float rhw;	     // 1.0で固定
	D3DCOLOR col;    // 頂点カラー
	D3DXVECTOR2 tex;
} VERTEX_2D;


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_X		(1)							// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)							// アニメーションパターンNo.の縦の最大数

#define DRAW_TYPE		(1)							// 描画方法
#define POLYGON_SIZE	(50.0f + m_fMovementLength)	// ポリゴンサイズ
#define MOVE_ROT		(0.02f)						// 回転角の移動量

#define EXPANTION_COUNT	(50)						// 拡大カウント
#define SHRINK_COUNT	(100)						// 縮小カウント
#define MOVE_LENGTH		(0.5f)						// ポリゴンサイズの移動量

//=============================================================================
// CRendererのコンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	
}

//=============================================================================
// CRendererのデストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドの合成
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ソースの合成方法の設定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// デスティネーションの合成方法の説明

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// 縮小時に補間
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 拡大時に補間
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U値の繰り返し
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V値の繰り返し

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // α値の合成
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	// テクスチャレンダリング用テクスチャの初期化
	InitRenderTex();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// バッファ破棄
		if (m_apBuffMT[nCnt] != NULL)
		{
			m_apBuffMT[nCnt]->Release();
			m_apBuffMT[nCnt] = NULL;
		}

		// インターフェース破棄
		if (m_apRenderMT[nCnt] != NULL)
		{
			m_apRenderMT[nCnt]->Release();
			m_apRenderMT[nCnt] = NULL;
		}

		// テクスチャ破棄
		if (m_apTextureMT[nCnt] != NULL)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = NULL;
		}
	}

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	// フェードバックエフェクト有効時のみ処理
	if (m_bFadeBackEffect == true)
	{
		// カウントアップ
		m_nCnt++;

		if (m_nCnt <= 3)
		{
			// ポリゴン拡大
			m_size.x += 1.6f * 3.0f;
			m_size.y += 0.9f * 3.0f;
		}
		else if (m_nCnt > 3 && m_nCnt <= 9)
		{
			// ポリゴン縮小
			m_size.x -= 1.6f * 1.5f;
			m_size.y -= 0.9f * 1.5f;
		}
		else if (m_nCnt > 9)
		{
			// 初期値に戻す
			m_nCnt = 0;
			m_fAlpha = 0.0f;
			m_size.x = SCREEN_WIDTH / 2.0f;
			m_size.y = SCREEN_HEIGHT / 2.0f;

			// フェードバックエフェクトを終える
			m_bFadeBackEffect = false;
		}

		// 構造体のポインタ変数
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f + m_size.y - 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f - m_size.y - 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f + m_size.y - 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f - m_size.y - 0.5f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);

		// 頂点バッファをアンロックする
		m_pVtxBuffMT->Unlock();
	}
#ifdef _DEBUG
	CDebugProc::Print("\n--- フェードバックエフェクト情報 ---\n");
	CDebugProc::Print("α値   : %f\n",m_fAlpha);
	CDebugProc::Print("サイズ : (%f,%f)\n", m_size.x,m_size.y);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	// フェード情報の取得
	CFade *pFade = CManager::GetFade();

	// 変数宣言
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;
	D3DVIEWPORT9 viewportDef;
	LPDIRECT3DSURFACE9 pRenderWk;
	LPDIRECT3DTEXTURE9 pTextureWk;

	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// 現在の情報を保存
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);
		m_pD3DDevice->GetViewport(&viewportDef);

		// レンダリングターゲットの設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

		// Zバッファの設定
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[0]);

		// テクスチャレンダリング用ビューポートの設定
		m_pD3DDevice->SetViewport(&m_viewportMT);

		// バックバッファ＆Ｚバッファのクリア
		m_pD3DDevice->Clear(
			0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// オブジェクトの描画処理
		CScene::DrawAll();

		// 頂点バッファをデータストリームに設定
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// フィードバック用ポリゴンにテクスチャ[1]を張り付ける
		m_pD3DDevice->SetTexture(0, m_apTextureMT[1]);
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// 全て戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
		m_pD3DDevice->SetViewport(&viewportDef);

		// α値を不透明にする
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

		// 頂点バッファをデータストリームに設定
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// フィードバック用ポリゴンにテクスチャ[0]を張り付ける
		m_pD3DDevice->SetTexture(0, m_apTextureMT[0]);
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// 設定を戻す
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// テクスチャ[0]とテクスチャ[1]を入れ替える
		pRenderWk = m_apRenderMT[0];
		m_apRenderMT[0] = m_apRenderMT[1];
		m_apRenderMT[1] = pRenderWk;

		pTextureWk = m_apTextureMT[0];
		m_apTextureMT[0] = m_apTextureMT[1];
		m_apTextureMT[1] = pTextureWk;

		// フェードの描画処理
		pFade->Draw();

#ifdef _DEBUG
		// FPS表示
		DrawFPS();

		CDebugProc::Draw();
#endif
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// フェードバックエフェクト
//=============================================================================
void CRenderer::SetFadeBackEffect(void)
{
	// フィードバックエフェクトの許可
	m_bFadeBackEffect = true;

	// 変数の初期化
	m_nCnt = 0;
	m_fAlpha = 0.7f;
	m_size.x = SCREEN_WIDTH / 2.0f;
	m_size.y = SCREEN_HEIGHT / 2.0f;
}

//=============================================================================
// テクスチャレンダリング用テクスチャの初期化
//=============================================================================
HRESULT CRenderer::InitRenderTex(void)
{
	// メンバ変数の初期化
	m_fAlpha = 0.0f;
	m_size = D3DXVECTOR2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	m_bFadeBackEffect = false;
	m_nCnt = 0;

	// 変数宣言
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// レンダリングターゲット用のテクスチャの生成
		m_pD3DDevice->CreateTexture(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_apTextureMT[nCnt],
			NULL);

		// テクスチャレンダリング用のインターフェース生成
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);

		// テクスチャレンダリング用Zバッファの生成
		m_pD3DDevice->CreateDepthStencilSurface(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_apBuffMT[nCnt],
			NULL);

		// 現在のレンダリングターゲットを取得
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// 現在のZバッファの取得
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		// レンダリングターゲットを設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		// Zバッファの設定
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[nCnt]);

		// バックバッファ＆Ｚバッファのクリア
		m_pD3DDevice->Clear(
			0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// レンダリングターゲットを設定
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Zバッファの設定
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	// テクスチャレンダリング用ビューポートの設定
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	// フィードバック用ポリゴンの生成
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX,					// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f + m_size.y - 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f - m_size.y - 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f + m_size.y - 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f - m_size.y - 0.5f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuffMT->Unlock();

	return S_OK;
}

#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void CRenderer::DrawFPS(void)
{
	RECT rect = { 1230, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];
	int nCntFPS = GetCountFPS();

	wsprintf(str, "FPS:%d\n", nCntFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0, 0, 0));
}
#endif


