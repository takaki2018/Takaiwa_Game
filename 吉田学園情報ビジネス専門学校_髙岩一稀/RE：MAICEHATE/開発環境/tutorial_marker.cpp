//==============================================================================================================
//
// チュートリアルマーカー (tutorial_marker.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial_marker.h"
#include "tutorial_image.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MAX_MARKER		(4)		// マーカーの最大値

#define MARKER_X		(565.0f)					// マーカーの中心座標(X)
#define MARKER_Y		(660.0f)					// マーカーの中心座標(Y)
#define MARKER_SIZE 	(20.0f)						// マーカーのサイズ
#define MARKER_SPACE	(50.0f)						// マーカーを配置する間隔

//--------------------------------------------------------------------------------------------------------------
// 構造体の定義
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXCOLOR col;		// 色
	float fSize;		// 大きさ
}TutorialMarker;

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialMarker = NULL;		// チュートリアルマーカーの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTutorialMarker[MAX_MARKER] = {};	// チュートリアルマーカーのテクスチャのポインタ
TutorialMarker g_TutorialMarker[MAX_MARKER];					// チュートリアルマーカー情報
int g_nPointMarker;												// 何個目のマーカーを選んでいるか

//--------------------------------------------------------------------------------------------------------------
// チュートリアルマーカーの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialMarker(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// マーカー情報の初期化
	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		g_TutorialMarker[nCntMarker].pos = D3DXVECTOR3(MARKER_X + MARKER_SPACE * nCntMarker, MARKER_Y,0.0f);
		g_TutorialMarker[nCntMarker].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		g_TutorialMarker[nCntMarker].fSize = MARKER_SIZE / 2.0f;
	}
	g_nPointMarker = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/marker.png", &g_pTextureTutorialMarker[0]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_MARKER, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialMarker,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialMarker->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_TutorialMarker[nCntMarker].pos.x - g_TutorialMarker[nCntMarker].fSize, g_TutorialMarker[nCntMarker].pos.y + g_TutorialMarker[nCntMarker].fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TutorialMarker[nCntMarker].pos.x - g_TutorialMarker[nCntMarker].fSize, g_TutorialMarker[nCntMarker].pos.y - g_TutorialMarker[nCntMarker].fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TutorialMarker[nCntMarker].pos.x + g_TutorialMarker[nCntMarker].fSize, g_TutorialMarker[nCntMarker].pos.y + g_TutorialMarker[nCntMarker].fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TutorialMarker[nCntMarker].pos.x + g_TutorialMarker[nCntMarker].fSize, g_TutorialMarker[nCntMarker].pos.y - g_TutorialMarker[nCntMarker].fSize, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 色の設定
		SetColTutorialMarker(nCntMarker);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTutorialMarker->Unlock();

	// サウンド
	PlaySound(SOUND_LABEL_OE);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアルマーカーの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitTutorialMarker(void)
{
	// サウンドの一時停止
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffTutorialMarker != NULL)
	{
		g_pVtxBuffTutorialMarker->Release();
		g_pVtxBuffTutorialMarker = NULL;
	}

	// テクスチャの開放
	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		if (g_pTextureTutorialMarker[nCntMarker] != NULL)
		{
			g_pTextureTutorialMarker[nCntMarker]->Release();
			g_pTextureTutorialMarker[nCntMarker] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアルマーカーの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorialMarker(void)
{
	// 画像番号の取得
	int nPointImage = GetPointImage();

	for (int nCntMarker = 0; nCntMarker < MAX_MARKER; nCntMarker++)
	{
		if (nCntMarker == nPointImage)
		{
			// 画像番号と一致しているとき色を一番明るくする
			g_TutorialMarker[nCntMarker].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			// それ以外は暗くする
			g_TutorialMarker[nCntMarker].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		// 色の更新
		SetColTutorialMarker(nCntMarker);
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアルマーカーの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawTutorialMarker(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialMarker, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_MARKER; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureTutorialMarker[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアルマーカーの色設定
// 引数		：nCntMarker	- 何番目のマーカーか取得
// 返り値	：void			- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetColTutorialMarker(int nCntMarker)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialMarker->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目のマーカーか
	pVtx += 4 * nCntMarker;

	// 頂点カラーの設定
	pVtx[0].col = g_TutorialMarker[nCntMarker].col;
	pVtx[1].col = g_TutorialMarker[nCntMarker].col;
	pVtx[2].col = g_TutorialMarker[nCntMarker].col;
	pVtx[3].col = g_TutorialMarker[nCntMarker].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorialMarker->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// マーカーの位置を取得
// 引数		：void	- 何もなし
// 返り値	：int	- 何番目のマーカーが選択されてるか返す
//--------------------------------------------------------------------------------------------------------------
int GetPointMarker(void)
{
	return g_nPointMarker;
}