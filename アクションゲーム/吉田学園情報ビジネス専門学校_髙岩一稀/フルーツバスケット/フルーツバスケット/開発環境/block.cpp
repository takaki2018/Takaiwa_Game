//-------------------------------------------------------------------
//
// ブロック (block.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "block.h"
#include "player.h"
#include "bear.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_BLOCKTYPE		(2)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;			// ブロックのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBlock[MAX_BLOCKTYPE] = {};	// ブロックの頂点バッファのポインタ
BLOCK g_aBlock[MAX_BLOCK];								// ブロックの情報

//-----------------------------------------------------------------
// ブロックの初期化処理
//-----------------------------------------------------------------
HRESULT InitBlock(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block001.jpg", &g_pTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block002.png", &g_pTextureBlock[1]);

	// 弾情報の初期化
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].nTimer = 0;
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].fTexU = 1.0f;
		g_aBlock[nCntBlock].fTexV = 1.0f;
		g_aBlock[nCntBlock].state = BLOCKSTATE_NORMAL;
		g_aBlock[nCntBlock].type = BLOCKTYPE_COLLISION;
		g_aBlock[nCntBlock].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_BLOCK, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntBullet = 0; nCntBullet < MAX_BLOCK; nCntBullet++)
	{
		// 頂点座標の設定
		SetVertexBlock(nCntBullet);

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

		// 弾の情報を次にうつす
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();	

	return S_OK;
}

//-----------------------------------------------------------------
// ブロックの終了処理
//-----------------------------------------------------------------
void UninitBlock(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_BLOCKTYPE; nCntTexture++)
	{
		if (g_pTextureBlock[nCntTexture] != NULL)
		{
			g_pTextureBlock[nCntTexture]->Release();
			g_pTextureBlock[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ブロックの更新処理
//-----------------------------------------------------------------
void UpdateBlock(void)
{
	// 構造体のポインタ変数
	BLOCK *pBlock = &g_aBlock[0];

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			// 位置の更新
			pBlock->pos += pBlock->move;

			// 時間の更新
			pBlock->nTimer++;

			if (pBlock->nTimer == 200)
			{// カウントが100になったとき処理
				pBlock->move *= -1;
				pBlock->nTimer = 0;
			}

			// 頂点座標の更新
			SetVertexBlock(nCntBlock);
		}
	}
}

//-----------------------------------------------------------------
// ブロックの描画処理
//-----------------------------------------------------------------
void DrawBlock(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 弾の描画
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{// 弾が使われているとき処理
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].type]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// ブロックの設定
//-----------------------------------------------------------------
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, float fTexU, float fTexV,BLOCKSTATE state, BLOCKTYPE type)
{
	// 構造体のポインタ変数
	BLOCK *pBlock;

	// 変数の受け渡し
	pBlock = &g_aBlock[0];

	// 弾の設定
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == false)
		{// 位置設定
			pBlock->pos = pos;

			// 移動量の設定
			pBlock->move = move;

			// 幅の設定
			pBlock->fWidth = fWidth;

			// 高さの設定
			pBlock->fHeight = fHeight;

			// テクスチャ座標Uの設定
			pBlock->fTexU = fTexU;

			// テクスチャ座標Vの設定
			pBlock->fTexV = fTexV;

			// ブロックの状態の設定
			pBlock->state = state;

			// ブロックの状態の設定
			pBlock->type = type;

			// trueで返す
			pBlock->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// ブロック情報の取得
//-----------------------------------------------------------------
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//-----------------------------------------------------------------
// ブロックの頂点座標
//-----------------------------------------------------------------
void SetVertexBlock(int nIdx)
{
	// 変数宣言
	float ShaikingMove = GetShaking();

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の情報を次にうつす
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x , g_aBlock[nIdx].pos.y + g_aBlock[nIdx].fHeight + ShaikingMove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x , g_aBlock[nIdx].pos.y + ShaikingMove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x + g_aBlock[nIdx].fWidth, g_aBlock[nIdx].pos.y + g_aBlock[nIdx].fHeight + ShaikingMove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x + g_aBlock[nIdx].fWidth, g_aBlock[nIdx].pos.y + ShaikingMove, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_aBlock[nIdx].fTexV);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_aBlock[nIdx].fTexU, g_aBlock[nIdx].fTexV);
	pVtx[3].tex = D3DXVECTOR2(g_aBlock[nIdx].fTexU, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//-----------------------------------------------------------------
// 配置するブロックの情報
//-----------------------------------------------------------------
void SetBlockInfo(MODESTATE modestate)
{
	switch (modestate)
	{// タイトルのブロック設置
	case MODESTATE_TITLE:
		TitleBlock();
		break;

		// ゲームの森ステージのブロック設置
	case MODESTATE_GAME_FOREST:
		GameForestBlock();
		break;

		// リザルトのブロック設置
	case MODESTATE_RESULT:
		ResultBlock();
		break;
	}
}

//-----------------------------------------------------------------
// タイトル画面のブロック配置
//-----------------------------------------------------------------
void TitleBlock(void)
{
	// 床
	SetBlock(D3DXVECTOR3(0, 690.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, BLOCK_SIZE,
		NORMAL_BLOCK, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_COLLISION);
}

//-----------------------------------------------------------------
// ゲーム森ステージのブロック配置
//-----------------------------------------------------------------
void GameForestBlock(void)
{
	// 床
	SetBlock(D3DXVECTOR3(0, 690.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, BLOCK_SIZE,
		NORMAL_BLOCK, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_COLLISION);

	// 穴
	SetBlock(D3DXVECTOR3(969.0f, 600.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 310.0f, 120.0f,
		1.0f, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_NOT_COLLISION);

	// 穴
	SetBlock(D3DXVECTOR3(-10.0f, 600.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 310.0f, 120.0f,
		-1.0f, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_NOT_COLLISION);

	// 浮いてる床
	// 左
	SetBlock(D3DXVECTOR3(0, 580.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * FLOAT_BLOCK, BLOCK_SIZE,
		FLOAT_BLOCK, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);

	// 右
	SetBlock(D3DXVECTOR3(970.0f, 580.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * FLOAT_BLOCK, BLOCK_SIZE,
		FLOAT_BLOCK, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);

	// 中央
	SetBlock(D3DXVECTOR3(415.0f, 450.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * 15.0f, BLOCK_SIZE,
		15.0f, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);

	// 左上
	SetBlock(D3DXVECTOR3(70.0f, 330.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * 5.0f, BLOCK_SIZE,
		5.0f, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);

	// 右上
	SetBlock(D3DXVECTOR3(1050.0f, 330.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCK_SIZE * 5.0f, BLOCK_SIZE,
		5.0f, 1.0f, BLOCKSTATE_FLOAT, BLOCKTYPE_COLLISION);
}

//-----------------------------------------------------------------
// リザルト画面のブロック配置
//-----------------------------------------------------------------
void ResultBlock(void)
{
	// 床
	SetBlock(D3DXVECTOR3(0, 690.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, BLOCK_SIZE,
		NORMAL_BLOCK, 1.0f, BLOCKSTATE_NORMAL, BLOCKTYPE_COLLISION);
}

//-----------------------------------------------------------------
// ブロックの当たり判定
//-----------------------------------------------------------------
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, COLLISIONINFO info)
{
	// 構造体のポインタ変数
	BLOCK *pBlock = &g_aBlock[0];
	bool bLand = false;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		if (pBlock->bUse == true)
		{// ブロックが使われているとき
			if (pBlock->type == BLOCKTYPE_COLLISION)
			{// 当たり判定があるブロックの時処理
				if ((pPos->x - fWidth) < (pBlock->pos.x + pBlock->fWidth) &&
					(pPos->x + fWidth) > pBlock->pos.x)
				{// プレイヤーがブロックの中にいるとき
					if (pPosOld->y + fHeight <= pBlock->pos.y)
					{// プレイヤーがブロックの上にいるとき
						if (pPos->y + fHeight > (pBlock->pos.y))
						{// めり込んだら処理
							LandBlock(nCntBlock, pPos, pMove, fHeight);
							bLand = true;
						}
					}
					else if ((pPosOld->y - fHeight) >= (pBlock->pos.y + pBlock->fHeight))
					{// プレイヤーがブロックの下にいるとき
						if ((pPos->y - fHeight) < (pBlock->pos.y + pBlock->fHeight))
						{// めり込んだらブロックの下の座標にする
							pPos->y = (pBlock->pos.y + pBlock->fHeight + fHeight);
							pMove->y = 1.0f;
						}
					}
				}
				if ((pPos->y + fHeight) > pBlock->pos.y && (pPos->y - fHeight) < (pBlock->pos.y + pBlock->fHeight))
				{// プレイヤーがブロックのY幅にいるとき
					if ((pPosOld->x + fWidth) <= pBlock->pos.x)
					{// プレイヤーがブロックの左側にいるとき
						if ((pPos->x + fWidth) > pBlock->pos.x)
						{// めり込んだらブロックの左の座標にする
							pPos->x = pBlock->pos.x - fWidth + pBlock->move.x;
							CollisionBlockSide(pMove, info);
						}
					}
					else if ((pPosOld->x - fWidth) >= (pBlock->pos.x + pBlock->fWidth))
					{// プレイヤーがブロックの右側にいるとき
						if ((pPos->x - fWidth) < (pBlock->pos.x + pBlock->fWidth))
						{// めり込んだらブロックの右の座標にする
							pPos->x = pBlock->pos.x + pBlock->fWidth + fWidth + pBlock->move.x;
							CollisionBlockSide(pMove, info);
						}
					}
				}
			}
		}
	}
	// 乗っているか返す
	return bLand;
}

//-----------------------------------------------------------------
// 何のブロックに乗っているか
//-----------------------------------------------------------------
void LandBlock(int nCntBlock, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, float fHeight)
{
	// 構造体のポインタ変数
	BLOCK *pBlock = &g_aBlock[0];
	
	// 何番目か
	pBlock += nCntBlock;

	if (pBlock->state == BLOCKSTATE_NORMAL || pBlock->state == BLOCKSTATE_FLOAT)
	{// ブロックの状態が床か浮いている床のとき
		pPos->y = pBlock->pos.y - fHeight;
	}
	else if (pBlock->state == BLOCKSTATE_RIFT)
	{// ブロックの状態が上下に動く床のとき
		if (pBlock->move.y >= 0)
		{// 上から下の時
			pPos->y = pBlock->pos.y - fHeight;
		}
		else if (pBlock->move.y < 0)
		{// 下から上の時
			pPos->y = pBlock->pos.y + pBlock->move.y - fHeight;
		}
	}
}

//-----------------------------------------------------------------
// ブロックの横に当たったとき
//-----------------------------------------------------------------
void CollisionBlockSide(D3DXVECTOR3 *pMove, COLLISIONINFO info)
{
	switch (info)
	{// プレイヤーのとき移動量を0にする
	case COLLISIONINFO_PLAYER:
		pMove->x = 0.0f;
		break;
		// フルーツのとき反射する
	case COLLISIONINFO_FRUITS:
		pMove->x *= -1;
		break;
		// 敵のとき反射する
	case COLLISIONINFO_ENEMY:
		pMove->x *= -1;
		break;
	}
}