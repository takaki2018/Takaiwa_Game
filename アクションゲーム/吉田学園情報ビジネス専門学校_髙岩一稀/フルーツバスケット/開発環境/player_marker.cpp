//-------------------------------------------------------------------
//
// プレイヤーマーカー (player_marker.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "player_marker.h"
#include "player.h"
#include "selectmode.h"
#include "bear.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_PLAYER_MARKER		(4)						// プレイヤーマーカーの最大値

#define PLAYER_MARKER_SIZE_X	(15.0f)					// プレイヤーマーカーのサイズ(幅)
#define PLAYER_MARKER_SIZE_Y	(15.0f)					// プレイヤーマーカーのサイズ(高さ)

#define PLAYER_MARKER_HEIGHT	(70.0f)					// プレイヤーマーカーの位置調整

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayerMarker = NULL;				// プレイヤーマーカーのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayerMarker[MAX_PLAYER_MARKER] = {};	// プレイヤーマーカーの頂点バッファのポインタ
PLAYER_MARKER g_aPlayerMarker[MAX_PLAYER_MARKER];					// プレイヤーマーカーの情報
int g_nCntPlayerMarker;												// 配置するプレイヤーマーカーの数

//-----------------------------------------------------------------
// 敵の初期化処理
//-----------------------------------------------------------------
HRESULT InitPlayerMarker(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	PLAYER *pPlayer;

	// 変数の受け渡し
	pDevice = GetDevice();
	pPlayer = GetPlayer();
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// シングルモードの時処理
	case PLAYMODE_SINGLE:
		g_nCntPlayerMarker = 1;
		break;
		// マルチモードの時処理
	case PLAYMODE_MULTI:
		g_nCntPlayerMarker = GetnCntController();
		break;
	}

	// 変数の初期化
	for (int nCnt = 0; nCnt < g_nCntPlayerMarker; nCnt++,pPlayer++)
	{
		g_aPlayerMarker[nCnt].pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - PLAYER_MARKER_HEIGHT, 0.0f);
		g_aPlayerMarker[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayerMarker[nCnt].type = PLAYER_MARKER_TYPE_ONE;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_1.png", &g_pTexturePlayerMarker[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_2.png", &g_pTexturePlayerMarker[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_3.png", &g_pTexturePlayerMarker[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_4.png", &g_pTexturePlayerMarker[3]);


	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * g_nCntPlayerMarker, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayerMarker,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayerMarker->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_nCntPlayerMarker; nCnt++)
	{
		// 頂点座標の設定
		SetVertexPlayerMarker(nCnt);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 次の情報に更新
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayerMarker->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 敵の終了処理
//-----------------------------------------------------------------
void UninitPlayerMarker(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffPlayerMarker != NULL)
	{
		g_pVtxBuffPlayerMarker->Release();
		g_pVtxBuffPlayerMarker = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_PLAYER_MARKER; nCnt++)
	{
		if (g_pTexturePlayerMarker[nCnt] != NULL)
		{
			g_pTexturePlayerMarker[nCnt]->Release();
			g_pTexturePlayerMarker[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// 敵の更新処理
//-----------------------------------------------------------------
void UpdatePlayerMarker(void)
{
	// 構造体のポインタ変数
	PLAYER_MARKER *pPlayerMarker = &g_aPlayerMarker[0];
	PLAYER *pPlayer = GetPlayer();

	for (int nCntPlayerMarker = 0; nCntPlayerMarker < g_nCntPlayerMarker; nCntPlayerMarker++, pPlayerMarker++,pPlayer++)
	{
		// 位置の更新
		pPlayerMarker->pos.x = pPlayer->pos.x;
		pPlayerMarker->pos.y = pPlayer->pos.y - PLAYER_MARKER_HEIGHT;

		// 頂点座標の更新
		SetVertexPlayerMarker(nCntPlayerMarker);
	}
}

//-----------------------------------------------------------------
// 敵の描画処理
//-----------------------------------------------------------------
void DrawPlayerMarker(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayerMarker, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// プレイヤーマーカーの描画
	for (int nCnt = 0; nCnt < g_nCntPlayerMarker; nCnt++)
	{
		pDevice->SetTexture(0, g_pTexturePlayerMarker[nCnt]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//-----------------------------------------------------------------
// 敵の頂点座標
//-----------------------------------------------------------------
void SetVertexPlayerMarker(int nIdx)
{
	// 変数宣言
	float ShaikingMove = GetShaking();

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayerMarker->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の情報を次にうつす
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPlayerMarker[nIdx].pos.x - PLAYER_MARKER_SIZE_X, g_aPlayerMarker[nIdx].pos.y + PLAYER_MARKER_SIZE_Y + ShaikingMove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayerMarker[nIdx].pos.x - PLAYER_MARKER_SIZE_X, g_aPlayerMarker[nIdx].pos.y - PLAYER_MARKER_SIZE_Y + ShaikingMove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayerMarker[nIdx].pos.x + PLAYER_MARKER_SIZE_X, g_aPlayerMarker[nIdx].pos.y + PLAYER_MARKER_SIZE_Y + ShaikingMove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayerMarker[nIdx].pos.x + PLAYER_MARKER_SIZE_X, g_aPlayerMarker[nIdx].pos.y - PLAYER_MARKER_SIZE_Y + ShaikingMove, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayerMarker->Unlock();
}