//-----------------------------------------------------------------
//
// タイトルロゴ (titlelogo.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "titlelogo.h"
#include "selectmode.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN     (2)							// アニメーションパターンNo.の最大数

#define TITLELOGO_POS_Y	(200.0f)					// 配置するY座標
#define TITLELOGO_X		(800.0f)					// タイトルロゴ幅
#define TITLELOGO_Y		(200.0f)					// タイトルロゴの高さ
#define HEIGHT_Y		(100.0f * -1.0f)			// タイトルロゴの初期中心Y座標

#define MOVE_TITLELOGO	(5.0f)						// タイトルロゴの移動量

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;		// タイトルロゴの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;			// タイトルロゴのテクスチャのポインタ
TITLELOGO g_TitleLogo;									// タイトルロゴの情報
D3DXCOLOR g_color;										// 色
bool g_bFall;											// タイトルロゴが完全に落ちたか判別
int g_TLCnt;											// タイトルロゴの回転カウント

//-----------------------------------------------------------------
// タイトルロゴの初期化処理
//-----------------------------------------------------------------
HRESULT InitTitlelogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_TitleLogo.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	g_TitleLogo.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_TitleLogo.fWidth = TITLELOGO_X / 2.0f;
	g_TitleLogo.fHeight = TITLELOGO_Y / 2.0f;
	g_TitleLogo.fAngle = atan2f(g_TitleLogo.fWidth, g_TitleLogo.fHeight);
	g_TitleLogo.fLength = sqrtf(g_TitleLogo.fWidth * g_TitleLogo.fWidth + g_TitleLogo.fHeight * g_TitleLogo.fHeight);
	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_bFall = false;
	g_TLCnt = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo.png", &g_pTextureTitleLogo);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = g_TitleLogo.pos.x + sinf(g_TitleLogo.rot.z + (D3DX_PI * 0.00f - g_TitleLogo.fAngle)) * g_TitleLogo.fLength;
	pVtx[0].pos.y = g_TitleLogo.pos.y + cosf(g_TitleLogo.rot.z + (D3DX_PI * 0.00f - g_TitleLogo.fAngle)) * g_TitleLogo.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_TitleLogo.pos.x + sinf(g_TitleLogo.rot.z + (g_TitleLogo.fAngle + D3DX_PI * 1.00f)) * g_TitleLogo.fLength;
	pVtx[1].pos.y = g_TitleLogo.pos.y + cosf(g_TitleLogo.rot.z + (g_TitleLogo.fAngle + D3DX_PI * 1.00f)) * g_TitleLogo.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_TitleLogo.pos.x + sinf(g_TitleLogo.rot.z + (g_TitleLogo.fAngle + D3DX_PI * 0.00f)) * g_TitleLogo.fLength;
	pVtx[2].pos.y = g_TitleLogo.pos.y + cosf(g_TitleLogo.rot.z + (g_TitleLogo.fAngle + D3DX_PI * 0.00f)) * g_TitleLogo.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_TitleLogo.pos.x + sinf(g_TitleLogo.rot.z + (D3DX_PI * 1.00f - g_TitleLogo.fAngle)) * g_TitleLogo.fLength;
	pVtx[3].pos.y = g_TitleLogo.pos.y + cosf(g_TitleLogo.rot.z + (D3DX_PI * 1.00f - g_TitleLogo.fAngle)) * g_TitleLogo.fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = g_color;
	pVtx[1].col = g_color;
	pVtx[2].col = g_color;
	pVtx[3].col = g_color;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// タイトルロゴの終了処理
//-----------------------------------------------------------------
void UninitTitlelogo(void)
{
	// セレクトモードの終了処理
	UninitSelectmode();

	// 頂点バッファの開放
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}

	// テクスチャの開放
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
}

//-----------------------------------------------------------------
// タイトルロゴの更新処理
//-----------------------------------------------------------------
void UpdateTitlelogo(void)
{
	// フェード情報の取得
	int nFade = GetFade();

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	if (GetJoypadTrigger(PLAYER_1,JPINFO_OKorJAMP) == true || GetKeyboardTrigger(KEYINFO_OK) == true)
	{// エンターキーまたはAボタンを押したとき
		if (nFade == FADE_IN)
		{// フェードインの時
			SetFade(FADE_NONE, MODE_TITLE);
			g_TitleLogo.pos.y = TITLELOGO_POS_Y;
			g_bFall = true;
		}
		else if (g_TitleLogo.pos.y <= TITLELOGO_POS_Y)
		{// タイトルロゴが落ちてきているとき
			g_TitleLogo.pos.y = TITLELOGO_POS_Y;
			g_bFall = true;
		}
	}
	// 更新処理
	if (nFade == FADE_NONE && g_TitleLogo.pos.y != TITLELOGO_POS_Y)
	{// タイトルロゴの移動量の更新
		g_TitleLogo.pos.y += MOVE_TITLELOGO;
	}
	else if (g_TitleLogo.pos.y >= TITLELOGO_POS_Y)
	{// タイトルロゴが落ちてきているとき
		g_TitleLogo.pos.y = TITLELOGO_POS_Y;
		g_bFall = true;
	}
	// 完全に落ち切ったとき
	if (g_bFall == true)
	{// ロゴを回転させる
		g_TLCnt++;

		if ((0 <= g_TLCnt % 100 ) && (50 > g_TLCnt % 100))
		{// 左の傾き
			if ((0 <= g_TLCnt % 100) && (25 > g_TLCnt % 100))
			{
				g_TitleLogo.rot.z += 0.002f;
			}
			else if ((25 <= g_TLCnt % 100) && (50 > g_TLCnt % 100))
			{
				g_TitleLogo.rot.z -= 0.002f;
			}
		}
		else if ((50 <= g_TLCnt % 100) && (100 > g_TLCnt % 100))
		{// 右の傾き
			if ((50 <= g_TLCnt % 100) && (75 > g_TLCnt % 100))
			{
				g_TitleLogo.rot.z -= 0.002f;
			}
			else if ((75 <= g_TLCnt % 100) && (100 > g_TLCnt % 100))
			{
				g_TitleLogo.rot.z += 0.002f;
			}
		}
	}

	// 頂点座標の設定
	pVtx[0].pos.x = g_TitleLogo.pos.x + sinf(g_TitleLogo.rot.z + (D3DX_PI * 0.00f - g_TitleLogo.fAngle)) * g_TitleLogo.fLength;
	pVtx[0].pos.y = g_TitleLogo.pos.y + cosf(g_TitleLogo.rot.z + (D3DX_PI * 0.00f - g_TitleLogo.fAngle)) * g_TitleLogo.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_TitleLogo.pos.x + sinf(g_TitleLogo.rot.z + (g_TitleLogo.fAngle + D3DX_PI * 1.00f)) * g_TitleLogo.fLength;
	pVtx[1].pos.y = g_TitleLogo.pos.y + cosf(g_TitleLogo.rot.z + (g_TitleLogo.fAngle + D3DX_PI * 1.00f)) * g_TitleLogo.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_TitleLogo.pos.x + sinf(g_TitleLogo.rot.z + (g_TitleLogo.fAngle + D3DX_PI * 0.00f)) * g_TitleLogo.fLength;
	pVtx[2].pos.y = g_TitleLogo.pos.y + cosf(g_TitleLogo.rot.z + (g_TitleLogo.fAngle + D3DX_PI * 0.00f)) * g_TitleLogo.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_TitleLogo.pos.x + sinf(g_TitleLogo.rot.z + (D3DX_PI * 1.00f - g_TitleLogo.fAngle)) * g_TitleLogo.fLength;
	pVtx[3].pos.y = g_TitleLogo.pos.y + cosf(g_TitleLogo.rot.z + (D3DX_PI * 1.00f - g_TitleLogo.fAngle)) * g_TitleLogo.fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}

//-----------------------------------------------------------------
// タイトルロゴの描画処理
//-----------------------------------------------------------------
void DrawTitlelogo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTextureTitleLogo);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------
// タイトルロゴ情報の取得
//-----------------------------------------------------------------
bool GetFall(void)
{
	return g_bFall;
}