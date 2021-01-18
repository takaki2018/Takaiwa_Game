//-----------------------------------------------------------------
//
// カラス (crow.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "rat.h"
#include "player.h"
#include "score.h"
#include "point.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_RAT				(16)										// カラスの最大数
#define TEXTURE_X			(4)											// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y			(2)											// アニメーションパターンNo.の縦の最大数

#define PLAY_ANIM			((1.0f / TEXTURE_X) * g_Rat.nPatternAnim)	// テクスチャの動作
#define UV_U				(1.0f / TEXTURE_X + PLAY_ANIM)				// Uの値
#define UV_V				(1.0f / TEXTURE_Y)							// Vの値

#define RAT_WIDTH			(50.0f)										// ネズミの幅
#define RAT_HEIGHT			(30.0f)										// ネズミの高さ

#define ANIMATION_CNT		(10)										// アニメーションを更新するカウント
#define MOVE_RAT			(5.0f)										// ネズミの移動量

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRat = NULL;	// ネズミの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRat = NULL;		// ネズミのテクスチャのポインタ
RAT g_Rat;										// ネズミの情報
int g_nRandSetRatCnt;							// ランダムで配置するカウント
int g_nSetRatCnt;								// ネズミの設置カウント

//-----------------------------------------------------------------
// ネズミの初期化処理
//-----------------------------------------------------------------
HRESULT InitRat(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();
	g_nRandSetRatCnt = rand() % 300 + 1000;
	g_nSetRatCnt = 0;

	// 情報の初期化処理
	g_Rat.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Rat.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Rat.fWidth = RAT_WIDTH / 2.0f;
	g_Rat.fHeight = RAT_HEIGHT / 2.0f;
	g_Rat.nCntAnim = 0;
	g_Rat.nPatternAnim = 0;
	g_Rat.bUse = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rat.png", &g_pTextureRat);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_RAT,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRat,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRat->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetVertexRat(0);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// 情報の更新
	pVtx += 4;
	
	// 頂点バッファをアンロックする
	g_pVtxBuffRat->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// ネズミの終了処理
//-----------------------------------------------------------------
void UninitRat(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffRat != NULL)
	{
		g_pVtxBuffRat->Release();
		g_pVtxBuffRat = NULL;
	}

	// テクスチャの開放
	if (g_pTextureRat != NULL)
	{
		g_pTextureRat->Release();
		g_pTextureRat = NULL;
	}
}

//-----------------------------------------------------------------
// ネズミの更新処理
//-----------------------------------------------------------------
void UpdateRat(void)
{
	// 構造体のポインタ変数
	RAT *pRat = &g_Rat;

	// 設置カウントのカウント
	g_nSetRatCnt++;

	// 割り切れるとき設置
	if (g_nRandSetRatCnt == g_nSetRatCnt)
	{// カラスの設置
		SetRat(D3DXVECTOR3(SCREEN_WIDTH, 680.0f, 0.0f),
			D3DXVECTOR3(-MOVE_RAT, 0.0f, 0.0f));

		// 変数の初期化
		g_nRandSetRatCnt = rand() % 300 + 1000;
		g_nSetRatCnt = 0;
	}
	// ネズミを使用してるか
	if (pRat->bUse == true)
	{
		// 位置の更新
		pRat->pos += pRat->move;

		// ネズミの当たり判定
		HitRat();

		// 頂点座標の更新
		SetVertexRat(0);

		// 画面外に行ったとき
		if (pRat->pos.x < 0.0f)
		{
			pRat->bUse = false;
		}
	}
}

//-----------------------------------------------------------------
// ネズミの描画処理
//-----------------------------------------------------------------
void DrawRat(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRat, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	if (g_Rat.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureRat);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-----------------------------------------------------------------
// ネズミの当たり判定
//-----------------------------------------------------------------
void HitRat(void)
{
	// 構造体のポインタ変数
	PLAYER *pPlayer = GetPlayer();

	// 変数宣言
	int nPlayer = GetnCntController();

	// クマとの当たり判定
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->state != PLAYERSTATE_FLASHING)
		{
			if ((pPlayer->pos.y + pPlayer->fHeight) >(g_Rat.pos.y - g_Rat.fHeight) &&
				(pPlayer->pos.y - pPlayer->fHeight) < (g_Rat.pos.y + g_Rat.fHeight) &&
				(pPlayer->pos.x + pPlayer->fWidth * 0.7f) > (g_Rat.pos.x - g_Rat.fWidth) &&
				(pPlayer->pos.x - pPlayer->fWidth * 0.7f) < (g_Rat.pos.x + g_Rat.fWidth))
			{// 変数宣言
				float fAngle = atan2f(pPlayer->pos.x - g_Rat.pos.x, pPlayer->pos.y - g_Rat.pos.y);

				// 獲得ポイントを表示する
				SetPoint(pPlayer->pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), POINTTYPE_DECREASE_ONE);

				// スコアの加算
				AddScore(nCntPlayer, -1);

				// プレイヤーの当たり判定
				HitPlayer(nCntPlayer, fAngle);
			}
		}
	}
}

//-----------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------
void SetVertexRat(int nCnt)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRat->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCnt * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Rat.pos.x - g_Rat.fWidth, g_Rat.pos.y + g_Rat.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Rat.pos.x - g_Rat.fWidth, g_Rat.pos.y - g_Rat.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Rat.pos.x + g_Rat.fWidth, g_Rat.pos.y + g_Rat.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Rat.pos.x + g_Rat.fWidth, g_Rat.pos.y - g_Rat.fHeight, 0.0f);

	// アニメーションカウンターのカウント
	g_Rat.nCntAnim++;

	if (0 == g_Rat.nCntAnim % ANIMATION_CNT)
	{// 割り切れるとき処理
		g_Rat.nPatternAnim = (g_Rat.nPatternAnim + 1) % TEXTURE_X;
	}
	// アニメーションの更新
	pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
	pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
	pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffRat->Unlock();
}

//-----------------------------------------------------------------
// ネズミの設置
//-----------------------------------------------------------------
void SetRat(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	// 構造体のポインタ変数
	RAT *pRat = &g_Rat;

	// ネズミの配置
	if (pRat->bUse == false)
	{
		// 位置の設定
		pRat->pos = pos;

		// 移動量の設定
		pRat->move = move;

		// 使用状態をtrueにする
		pRat->bUse = true;
	}
}