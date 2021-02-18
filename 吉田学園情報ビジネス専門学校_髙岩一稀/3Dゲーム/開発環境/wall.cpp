//-----------------------------------------------------------------
//
// 壁 (wall.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "wall.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define WIDTH_NUM				(10)										// 列
#define HEIGHT_NUM				(2)											// 行
#define WIDTH					(100.0f)									// 幅
#define HEIGHT					(100.0f)									// 奥行
#define WIDTH_VEC				(WIDTH_NUM + 1)								// 列の頂点数
#define HEIGHT_VEC				(HEIGHT_NUM + 1)								// 行の頂点数

#define MAX_WALL_MESH_VERTEX	(HEIGHT_NUM * 2 * (WIDTH_NUM + 2) - 2)
#define MAX_INDEX				(WIDTH_NUM * HEIGHT_NUM * 2 + (4 * (HEIGHT_NUM - 1)))
#define MAX_WALL_MESH			((WIDTH_NUM + 1) * (HEIGHT_NUM + 1))										// 用意するポリゴンデータの数


//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;				// 壁の頂点バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall = NULL;				// 壁のインデックスへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;					// 壁のテクスチャのポインタ
Wall g_aWall[MAX_WALL];										// 壁の情報

//-----------------------------------------------------------------
// 壁の初期化処理
//-----------------------------------------------------------------
HRESULT InitWall(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].nCntWallX = WIDTH_NUM;
		g_aWall[nCntWall].nCntWallZ = HEIGHT_NUM;
		g_aWall[nCntWall].fWidth = WIDTH;
		g_aWall[nCntWall].fDepth = HEIGHT;
		g_aWall[nCntWall].fsizeWidth = g_aWall[nCntWall].nCntWallX * g_aWall[nCntWall].fWidth;
		g_aWall[nCntWall].fsizeDepth = g_aWall[nCntWall].nCntWallZ * g_aWall[nCntWall].fDepth;
		g_aWall[nCntWall].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field000.jpg", &g_pTextureWall);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_WALL_MESH * MAX_WALL,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		for (int nCntZ = 0; nCntZ < HEIGHT_VEC; nCntZ++)
		{
			for (int nCntX = 0; nCntX < WIDTH_VEC; nCntX++)
			{
				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3((-g_aWall[nCntWall].fsizeWidth / 2.0f + (float)nCntX * g_aWall[nCntWall].fWidth) + g_aWall[nCntWall].pos.x,
					(g_aWall[nCntWall].fsizeDepth - (float)nCntZ * g_aWall[nCntWall].fDepth) + g_aWall[nCntWall].pos.y,
					g_aWall[nCntWall].pos.z);

				// 法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

				// 次の情報
				pVtx++;
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();

	// インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * MAX_WALL_MESH_VERTEX,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,						// フォーマット
		D3DPOOL_MANAGED,
		&g_pIdxBuffWall,
		NULL)))
	{
		return E_FAIL;
	}

	// インデックス情報へのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffWall->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < HEIGHT_NUM; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC + 1; nCntX++)
		{
			if (nCntX != 0 && nCntX == WIDTH_VEC && nCntZ != HEIGHT_NUM - 1)
			{// 右端から折り返す時
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntZ == HEIGHT_NUM - 1 && nCntX == WIDTH_VEC)
			{// 終了時に無視する
				break;
			}
			else
			{// 通常配置
				pIdx[0] = WIDTH_VEC + (WIDTH_VEC * nCntZ) + nCntX;
				pIdx[1] = pIdx[0] - WIDTH_VEC;
			}
			// 次の情報に移動
			pIdx += 2;
		}
	}
	// インデックスバッファをアンロックする
	g_pIdxBuffWall->Unlock();

	// 壁の配置
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 奥の壁
	SetWall(D3DXVECTOR3(500.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, 0.0f));				// 奥の壁
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));				// 奥の壁
	SetWall(D3DXVECTOR3(-500.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2.0f, 0.0f));				// 奥の壁

	return S_OK;
}

//-----------------------------------------------------------------
// 壁の終了処理
//-----------------------------------------------------------------
void UninitWall(void)
{
	// インデックスの開放
	if (g_pIdxBuffWall != NULL)
	{
		g_pIdxBuffWall->Release();
		g_pIdxBuffWall = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}

	// テクスチャの開放
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
}

//-----------------------------------------------------------------
// 壁の更新処理
//-----------------------------------------------------------------
void UpdateWall(void)
{

}

//-----------------------------------------------------------------
// 壁の描画処理
//-----------------------------------------------------------------
void DrawWall(void)
{
	// 変数宣言
	Wall *pWall = &g_aWall[0];
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxScaling,mtxRot, mtxTrans;	// 計算用マトリックス

	// 変数の受け渡し
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pWall->mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pWall->rot.y, pWall->rot.x, pWall->rot.z);
			D3DXMatrixMultiply(&pWall->mtxWorld, &pWall->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pWall->pos.x, pWall->pos.y, pWall->pos.z);
			D3DXMatrixMultiply(&pWall->mtxWorld, &pWall->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pWall->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffWall);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureWall);
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_WALL_MESH, 0, MAX_INDEX);
		}
	}
}

//-----------------------------------------------------------------
// 壁の配置
//-----------------------------------------------------------------
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 構造体のポインタ変数
	Wall *pWall = &g_aWall[0];

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == false)
		{
			// 位置の設定
			pWall->pos = pos;

			// 角度の設定
			pWall->rot = rot;

			// 使用状態をtrueにする
			pWall->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// 壁との当たり判定
//-----------------------------------------------------------------
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// 変数宣言
	Wall *pWall = &g_aWall[0];
	bool bLand = false;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
#if 0
			// ベクトル計算
			pWall->aVecA = *pPos - pWall->pos;
			pWall->aVecB = (pWall->aVecA * 1.0f) * 2.0f;

			// 外積を用いた当たり判定
			pWall->fVec = (pWall->aVecA.x * pWall->aVecB.x) + (pWall->aVecA.z * pWall->aVecB.z);

			if (pWall->fVec > 0.0f)
			{
				if ((pPosOld->x < pWall->pos.x - pWall->fsizeWidth / 2.0f && pPosOld->x > pWall->pos.x + pWall->fsizeWidth / 2.0f) ||
					(pPosOld->z < pWall->pos.z - pWall->fsizeDepth / 2.0f && pPosOld->z > pWall->pos.z + pWall->fsizeDepth / 2.0f))
				{
					if (pPos->x > pPosOld->x && (pPosOld->x < pWall->pos.x))
					{// プレイヤーが左から当たったとき
						pPos->x = pWall->pos.x - pvtxMax->x;
					}
					else if (pPos->x < pPosOld->x && (pPosOld->x > pWall->pos.x))
					{// プレイヤーが右から当たったとき
						pPos->x = pWall->pos.x - pvtxMin->x;
					}
					else if (pPos->z > pPosOld->z && pPosOld->z < pWall->pos.z)
					{// プレイヤーが手前から当たったとき
						pPos->z = pWall->pos.z - pvtxMax->z;
					}
					else if (pPos->z < pPosOld->z && pPosOld->z > pWall->pos.z)
					{// プレイヤーが奥から当たったとき
						pPos->z = pWall->pos.z - pvtxMin->z;
					}
				}
			}
#else
			if (pPosOld->x < pWall->pos.x - pWall->fsizeWidth / 2.0f && pPosOld->x > pWall->pos.x + pWall->fsizeWidth / 2.0f &&
				pPosOld->z < pWall->pos.z - pWall->fsizeDepth / 2.0f && pPosOld->z > pWall->pos.z + pWall->fsizeDepth / 2.0f)
			{
				pPos->y = 200.0f;
			}
#endif
		}
	}
}

//-----------------------------------------------------------------
// 壁情報の取得
//-----------------------------------------------------------------
Wall *GetWall(void)
{
	return &g_aWall[0];
}