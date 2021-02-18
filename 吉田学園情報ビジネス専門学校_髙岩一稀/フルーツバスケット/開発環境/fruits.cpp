//-------------------------------------------------------------------
//
// フルーツ (fruits.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "fruits.h"
#include "player.h"
#include "block.h"
#include "bear.h"
#include "crow.h"
#include "score.h"
#include "point.h"
#include "gamepad.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_FRUITS				(256)					// フルーツの最大値

#define FRUITS_SIZE_X_GAME		(20.0f)					// フルーツのサイズ(幅)ゲーム
#define FRUITS_SIZE_Y_GAME		(20.0f)					// フルーツのサイズ(高さ)ゲーム

#define FRUITS_SIZE_X_TITLE		(80.0f)				// フルーツのサイズ(幅)タイトル
#define FRUITS_SIZE_Y_TITLE		(80.0f)				// フルーツのサイズ(高さ)タイトル

#define ADD_GRAVITY				(0.3f)					// 重力
#define ADD_ROT					(0.1f)					// 回転量
#define BOUND					(1.70f)					// バウンド値

#define SETFRUITS_INTERVAL		(60)					// フルーツを配置する間隔
#define SETFRUITS_ONE			(0)						// 一個目
#define SETFRUITS_TWO			(30)					// 二個目

#define SETFEVERFRUITS_INTERVAL	(10)					// フィーバータイムのフルーツを配置する間隔
#define SETFEVERFRUITS_ONE		(0)						// 一個目

#define MAX_GETFRUITS			(5)						// 獲得したフルーツの履歴の最大数

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFruits = NULL	;				// フルーツのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureFruits[FRUITSTYPE_MAX] = {};		// フルーツの頂点バッファのポインタ
FRUITS g_aFruits[MAX_FRUITS];									// フルーツの情報
float g_fLength;												// 対角線
float g_fAngle;													// 角度
int g_nSetFruitsCnt;											// フルーツを配置するカウント
int g_nSetFeverFruitsCnt;										// フィーバータイムのフルーツを配置するカウント
int g_nSetGoldFruitsCnt;										// 金色のフルーツを配置するカウント
int g_nCntTitleFruits;											// タイトル画面で落とす時間
bool g_bTitleFruits;											// タイトル画面でフルーツを落とすかどうか
																
//-----------------------------------------------------------------
// フルーツの初期化処理
//-----------------------------------------------------------------
HRESULT InitFruits(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	MODE mode = GetMode();

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	for (int nCnt = 0; nCnt < MAX_FRUITS; nCnt++)
	{
		g_aFruits[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFruits[nCnt].posOld = g_aFruits[nCnt].pos;
		g_aFruits[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFruits[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFruits[nCnt].nCntBound = 0;

		switch (mode)
		{// タイトルのとき
		case MODE_TITLE:
			g_aFruits[nCnt].fWidth = FRUITS_SIZE_X_TITLE;
			g_aFruits[nCnt].fHeight = FRUITS_SIZE_Y_TITLE;
			break;

			// ゲームのとき
		case MODE_GAME:
			g_aFruits[nCnt].fWidth = FRUITS_SIZE_X_GAME;
			g_aFruits[nCnt].fHeight = FRUITS_SIZE_Y_GAME;
			break;
		}
		
		g_aFruits[nCnt].nType = FRUITSTYPE_APPLE;
		g_aFruits[nCnt].bLand = false;
		g_aFruits[nCnt].bUse = false;
	}

	// 変数の初期化
	g_fLength = sqrtf(g_aFruits[0].fWidth * g_aFruits[0].fWidth + g_aFruits[0].fHeight * g_aFruits[0].fHeight);
	g_fAngle = atan2f(g_aFruits[0].fWidth, g_aFruits[0].fHeight);
	g_nSetFruitsCnt = 0;
	g_nCntTitleFruits = 100;
	g_bTitleFruits = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/apple.png", &g_pTextureFruits[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/blue_apple.png", &g_pTextureFruits[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/peach.png", &g_pTextureFruits[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/orange.png", &g_pTextureFruits[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/apple_gold.png", &g_pTextureFruits[4]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_FRUITS, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffFruits,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFruits->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_FRUITS; nCnt++)
	{
		// 頂点座標の設定
		SetVertexFruits(nCnt);

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

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 次の情報に更新
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffFruits->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// フルーツの終了処理
//-----------------------------------------------------------------
void UninitFruits(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffFruits != NULL)
	{
		g_pVtxBuffFruits->Release();
		g_pVtxBuffFruits = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < FRUITSTYPE_MAX; nCnt++)
	{
		if (g_pTextureFruits[nCnt] != NULL)
		{
			g_pTextureFruits[nCnt]->Release();
			g_pTextureFruits[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// フルーツの更新処理
//-----------------------------------------------------------------
void UpdateFruits(void)
{
	// 変数宣言
	MODE mode = GetMode();

	switch (mode)
	{// タイトル画面のとき
	case MODE_TITLE:
		TitleFruits();
		break;

		// ゲーム画面のとき
	case MODE_GAME:
		GameFruits();
		break;
	}
}

//-----------------------------------------------------------------
// フルーツの描画処理
//-----------------------------------------------------------------
void DrawFruits(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFruits, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// バウンドの描画
	for (int nCnt = 0; nCnt < MAX_FRUITS; nCnt++)
	{
		if (g_aFruits[nCnt].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureFruits[g_aFruits[nCnt].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// タイトル画面のフルーツ
//-----------------------------------------------------------------
void TitleFruits(void)
{
	// 構造体のポインタ変数
	FRUITS *pFruits = &g_aFruits[0];

	// フルーツの配置
	SetFruitsTitle();

	for (int nCntFruits = 0; nCntFruits < MAX_FRUITS; nCntFruits++, pFruits++)
	{
		if (pFruits->bUse == true)
		{
			// 位置の保存
			pFruits->posOld = pFruits->pos;

			// 移動量の更新
			pFruits->move.y += ADD_GRAVITY;

			// 位置の更新
			pFruits->pos += pFruits->move;

			// 画面外にいったときfalse
			if (g_aFruits[nCntFruits].pos.y > SCREEN_HEIGHT + g_aFruits[nCntFruits].fHeight / 2.0f)
			{
				g_aFruits[nCntFruits].bUse = false;
			}

			// 頂点座標の更新
			SetVertexFruits(nCntFruits);
		}
	}
}

//-----------------------------------------------------------------
// ゲーム画面のフルーツ
//-----------------------------------------------------------------
void GameFruits(void)
{
	// 構造体のポインタ変数
	FRUITS *pFruits = &g_aFruits[0];

	// フルーツの配置
	SetFruitsGame();

	for (int nCntFruits = 0; nCntFruits < MAX_FRUITS; nCntFruits++, pFruits++)
	{
		if (pFruits->bUse == true)
		{
			// 位置の保存
			pFruits->posOld = pFruits->pos;

			// 移動量の更新
			pFruits->move.y += ADD_GRAVITY;

			// 位置の更新
			pFruits->pos += pFruits->move;

			// 回転
			if (pFruits->posOld.x < pFruits->pos.x)
			{// 右に進んでるとき右回転する
				pFruits->rot.z -= ADD_ROT;
				if (pFruits->rot.z < D3DX_PI)
				{
					pFruits->rot.z -= D3DX_PI * 2.0f;
				}
			}
			else if (pFruits->posOld.x > pFruits->pos.x)
			{// 左に進んでるとき左回転する
				pFruits->rot.z += ADD_ROT;
				if (pFruits->rot.z > D3DX_PI)
				{
					pFruits->rot.z -= D3DX_PI * 2.0f;
				}
			}
			// フルーツの当たり判定
			HitFruits(nCntFruits);

			if (pFruits->pos.x + pFruits->fWidth <= 120.0f)
			{// 一番左に到達したとき処理
			 //pFruits->bUse = false;
				pFruits->move.x *= -1;
			}
			if (pFruits->pos.x - pFruits->fWidth >= SCREEN_WIDTH - 120.0f)
			{// 一番右に到達したとき処理
				pFruits->move.x *= -1;
			}
			// 穴に行ったとき
			if (pFruits->pos.y - pFruits->fHeight >= 580.0f)
			{
				if (pFruits->pos.x + pFruits->fWidth < 180.0f)
				{// 左側に入ったとき
					pFruits->bUse = false;
				}
				else if (pFruits->pos.x - pFruits->fWidth > 1070.0f)
				{// 右側に入ったとき
					pFruits->bUse = false;
				}
			}
			// ブロックに乗ったかどうか
			pFruits->bLand = CollisionBlock(&pFruits->pos, &pFruits->posOld, &pFruits->move,
				pFruits->fWidth, pFruits->fHeight, COLLISIONINFO_FRUITS);

			if (pFruits->bLand == true)
			{// ブロックの乗ったときバウンドする
				pFruits->move.y -= pFruits->move.y * BOUND;
			}
			// 頂点座標の更新
			SetVertexFruits(nCntFruits);
		}
	}
}

//-----------------------------------------------------------------
// タイトル画面の配置情報
//-----------------------------------------------------------------
void SetFruitsTitle(void)
{
	// 変数に情報を渡す
	float fShaikingMove = GetShaking();

	if (fShaikingMove != 0.0f)
	{// 画面が揺れているとき
		g_bTitleFruits = true;
	}

	if (g_bTitleFruits == true && g_nCntTitleFruits >= 0)
	{// フィーバータイムのフルーツの配置カウントを増加
		g_nCntTitleFruits--;

		// フルーツの配置
		SetFruits(D3DXVECTOR3((float)(rand() % 430), -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));

		// フルーツの配置
		SetFruits(D3DXVECTOR3((float)(rand() % 430 + 430), -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));

		// フルーツの配置
		SetFruits(D3DXVECTOR3((float)(rand() % 430 + 860), -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));
	}
}

//-----------------------------------------------------------------
// フルーツの配置情報
//-----------------------------------------------------------------
void SetFruitsGame(void)
{
	// 変数に情報を渡す
	CROW *pCrow = GetCrow();
	float fShaikingMove = GetShaking();

	// 変数宣言
	bool bSetGoldFruits = false;
	bool bSetFruits = false;

	if (fShaikingMove != 0.0f)
	{// 画面が揺れているとき
		bSetFruits = true;
	}
	for (int nCntCrow = 0; nCntCrow < 4; nCntCrow++)
	{// 使用しているカラスを探す
		if (pCrow->bUse == true)
		{
			bSetGoldFruits = true;
			g_nSetGoldFruitsCnt++;
		}
	}
	// フルーツの配置カウントを増加
	g_nSetFruitsCnt++;

	if (bSetFruits == true)
	{// フィーバータイムのフルーツの配置カウントを増加
		g_nSetFeverFruitsCnt++;

		if (SETFEVERFRUITS_ONE == g_nSetFeverFruitsCnt % SETFEVERFRUITS_INTERVAL)
		{// フルーツの配置
			SetFruits(D3DXVECTOR3((float)(rand() % 1000 + 100), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 + 3), 0.0f, 0.0f),
				(FRUITSTYPE)(rand() % 4));

			// フルーツの配置
			SetFruits(D3DXVECTOR3((float)(rand() % 1000 + 100), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 - 5), 0.0f, 0.0f),
				(FRUITSTYPE)(rand() % 4));
		}
	}
	if (bSetGoldFruits == true)
	{// 金色の果物の配置
		if (0 == g_nSetGoldFruitsCnt % 800)
		{
			SetFruits(D3DXVECTOR3((float)(rand() % 1000 + 100), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 + 3), 0.0f, 0.0f),
				(FRUITSTYPE)(FRUITSTYPE_GOLDAPPLE));
		}
	}

	if (SETFRUITS_ONE == g_nSetFruitsCnt % SETFRUITS_INTERVAL)
	{// フルーツの配置
		SetFruits(D3DXVECTOR3((float)(rand() % 950 + 150), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 + 3), 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));
	}
	else if (SETFRUITS_TWO == g_nSetFruitsCnt % SETFRUITS_INTERVAL)
	{// フルーツの配置
		SetFruits(D3DXVECTOR3((float)(rand() % 950 + 150), 50.0f, 0.0f), D3DXVECTOR3((float)(rand() % 3 - 5), 0.0f, 0.0f),
			(FRUITSTYPE)(rand() % 4));
	}
}

//-----------------------------------------------------------------
// フルーツの当たり判定
//-----------------------------------------------------------------
void HitFruits(int nCntFruits)
{
	// 構造体のポインタ変数
	FRUITS *pFruits = GetFruits();
	PLAYER *pPlayer = GetPlayer();

	// 変数宣言
	int nPlayer = GetPlayerCnt();

	// 何番目か
	pFruits += nCntFruits;

	// フルーツの当たり判定
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++, pPlayer++)
	{
		if ((pPlayer->pos.y + pPlayer->fHeight) >(pFruits->pos.y - pFruits->fHeight) &&
			(pPlayer->pos.y - pPlayer->fHeight) < (pFruits->pos.y + pFruits->fHeight) &&
			(pPlayer->pos.x + pPlayer->fWidth * 0.7f) > (pFruits->pos.x - pFruits->fWidth) &&
			(pPlayer->pos.x - pPlayer->fWidth * 0.7f) < (pFruits->pos.x + pFruits->fWidth))
		{
			if (pFruits->nType != FRUITSTYPE_GOLDAPPLE)
			{
				// 獲得ポイントを表示する
				SetPoint(pPlayer->pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), POINTTYPE_PLUS_ONE);

				// スコアの加算
				AddScore(nCntPlayer, 1);

				// サウンド
				PlaySound(SOUND_LABEL_SE_GET);
			}
			else if (pFruits->nType == FRUITSTYPE_GOLDAPPLE)
			{
				// 獲得ポイントを表示する
				SetPoint(pPlayer->pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), POINTTYPE_PLUS_FIVE);

				// スコアの加算
				AddScore(nCntPlayer, 5);

				// サウンド
				PlaySound(SOUND_LABEL_SE_GET_GOLD);
			}
			// 使用状態をfalseにする
			pFruits->bUse = false;
		}
		
	}
}

//-----------------------------------------------------------------
// フルーツの設定
//-----------------------------------------------------------------
void SetFruits(D3DXVECTOR3 pos, D3DXVECTOR3 move, FRUITSTYPE type)
{
	// 構造体のポインタ変数
	FRUITS *pFruits = &g_aFruits[0];

	// 弾の設定
	for (int nCntFruits = 0; nCntFruits < MAX_FRUITS; nCntFruits++, pFruits++)
	{
		if (pFruits->bUse == false)
		{// 位置設定
			pFruits->pos = pos;

			// 移動量の設定
			pFruits->move = move;

			// タイプの確定
			pFruits->nType = type;

			// trueで返す
			pFruits->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// フルーツの頂点座標
//-----------------------------------------------------------------
void SetVertexFruits(int nIdx)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFruits->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の情報を次にうつす
	pVtx += nIdx * 4;

	// 頂点座標の更新
	pVtx[0].pos.x = g_aFruits[nIdx].pos.x + sinf(g_aFruits[nIdx].rot.z + (D3DX_PI * 0.00f - g_fAngle)) * g_fLength;
	pVtx[0].pos.y = g_aFruits[nIdx].pos.y + cosf(g_aFruits[nIdx].rot.z + (D3DX_PI * 0.00f - g_fAngle)) * g_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aFruits[nIdx].pos.x + sinf(g_aFruits[nIdx].rot.z + (g_fAngle + D3DX_PI * 1.00f)) * g_fLength;
	pVtx[1].pos.y = g_aFruits[nIdx].pos.y + cosf(g_aFruits[nIdx].rot.z + (g_fAngle + D3DX_PI * 1.00f)) * g_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aFruits[nIdx].pos.x + sinf(g_aFruits[nIdx].rot.z + (g_fAngle + D3DX_PI * 0.00f)) * g_fLength;
	pVtx[2].pos.y = g_aFruits[nIdx].pos.y + cosf(g_aFruits[nIdx].rot.z + (g_fAngle + D3DX_PI * 0.00f)) * g_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aFruits[nIdx].pos.x + sinf(g_aFruits[nIdx].rot.z + (D3DX_PI * 1.00f - g_fAngle)) * g_fLength;
	pVtx[3].pos.y = g_aFruits[nIdx].pos.y + cosf(g_aFruits[nIdx].rot.z + (D3DX_PI * 1.00f - g_fAngle)) * g_fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	g_pVtxBuffFruits->Unlock();
}

//-----------------------------------------------------------------
// フルーツ情報の取得
//-----------------------------------------------------------------
FRUITS *GetFruits(void)
{
	return &g_aFruits[0];
}

//-----------------------------------------------------------------
// タイトル画面のフルーツカウントの取得
//-----------------------------------------------------------------
bool GetbTitleFruits(void)
{
	return g_bTitleFruits;
}

//-----------------------------------------------------------------
// タイトル画面のフルーツカウントの取得
//-----------------------------------------------------------------
int GetTitleFruitsCnt(void)
{
	return g_nCntTitleFruits;
}