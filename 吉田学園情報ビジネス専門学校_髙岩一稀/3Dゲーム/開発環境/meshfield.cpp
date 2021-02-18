//-----------------------------------------------------------------
//
// メッシュフィールド (meshfield.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "meshfield.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define WIDTH_NUM			(10)										// 列
#define DEPTH_NUM			(10)										// 行
#define WIDTH				(200.0f)									// 幅
#define DEPTH				(200.0f)									// 奥行
#define WIDTH_VEC			(WIDTH_NUM + 1)								// 列の頂点数
#define DEPTH_VEC			(DEPTH_NUM + 1)								// 行の頂点数

#define MAX_MF_VERTEX		(DEPTH_NUM * 2 * (WIDTH_NUM + 2) - 2)		// 総頂点数
#define MAX_INDEX			(WIDTH_NUM * DEPTH_NUM * 2 + (4 * (DEPTH_NUM - 1)))	// 総インデックス数
#define MAX_MESHFIELD		((WIDTH_NUM + 1) * (DEPTH_NUM + 1))					// 

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;				// メッシュフィールドの頂点バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;				// メッシュフィールドのインデックスへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;					// メッシュフィールドのテクスチャのポインタ
MeshField g_aMeshField;											// メッシュフィールド情報

//-----------------------------------------------------------------
// メッシュフィールドの初期化処理
//-----------------------------------------------------------------
HRESULT InitMeshField(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();
	
	// 情報の初期化
	g_aMeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.nCntBlockX = WIDTH_NUM;
	g_aMeshField.nCntBlockZ = DEPTH_NUM;
	g_aMeshField.fWidth = WIDTH;
	g_aMeshField.fDepth = DEPTH;
	g_aMeshField.fsizeWidth = g_aMeshField.nCntBlockX * g_aMeshField.fWidth;
	g_aMeshField.fsizeDepth = g_aMeshField.nCntBlockZ * g_aMeshField.fDepth;
	g_aMeshField.bUse = false;
	
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field001.jpg", &g_pTextureMeshField);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_MESHFIELD,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < DEPTH_VEC; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC; nCntX++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aMeshField.fsizeWidth / 2.0f + (float)nCntX * g_aMeshField.fWidth,
				g_aMeshField.pos.y, 
				g_aMeshField.fsizeDepth / 2.0f - (float)nCntZ * g_aMeshField.fDepth);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

			// 次の情報
			pVtx++;
		}
	}
	
	// 頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();

	// インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * MAX_MF_VERTEX,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,						// フォーマット
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL)))
	{
		return E_FAIL;
	}

	// インデックス情報へのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < DEPTH_NUM; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC + 1; nCntX++)
		{
			if (nCntX != 0 && nCntX == WIDTH_VEC && nCntZ != DEPTH_NUM - 1)
			{// 右端から折り返す時
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntZ == DEPTH_NUM - 1 && nCntX == WIDTH_VEC)
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
	g_pIdxBuffMeshField->Unlock();

	// 床の配置
	SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		0.0f, 0.0f);

	return S_OK;
}

//-----------------------------------------------------------------
// メッシュフィールドの終了処理
//-----------------------------------------------------------------
void UninitMeshField(void)
{
	// インデックスの開放
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	// テクスチャの開放
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}
}

//-----------------------------------------------------------------
// メッシュフィールドの更新処理
//-----------------------------------------------------------------
void UpdateMeshField(void)
{

}

//-----------------------------------------------------------------
// メッシュフィールドの描画処理
//-----------------------------------------------------------------
void DrawMeshField(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// 変数の受け渡し
	pDevice = GetDevice();

	if (g_aMeshField.bUse == true)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMeshField.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField.rot.y, g_aMeshField.rot.x, g_aMeshField.rot.z);
		D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aMeshField.pos.x, g_aMeshField.pos.y, g_aMeshField.pos.z);
		D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField.mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshField);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// ポリゴンの描画
		pDevice->SetTexture(0, g_pTextureMeshField);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0, MAX_MESHFIELD,0, MAX_INDEX);
	}
	
}

//-----------------------------------------------------------------
// メッシュフィールドの設置
//-----------------------------------------------------------------
void SetMeshField(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	// 構造体のポインタ変数
	MeshField *pMeshField = &g_aMeshField;

	if (pMeshField->bUse == false)
	{
		// 位置の設定
		pMeshField->pos = pos;

		// 使用している状態にする
		pMeshField->bUse = true;
	}
}

//-----------------------------------------------------------------
// メッシュフィールドとの当たり判定
//-----------------------------------------------------------------
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld)
{
	// 変数宣言
	bool bLand = false;

	if (pPosOld->x > g_aMeshField.pos.x - g_aMeshField.fsizeWidth / 2.0f && pPosOld->x < g_aMeshField.pos.x + g_aMeshField.fsizeWidth / 2.0f &&
		pPosOld->z > g_aMeshField.pos.z - g_aMeshField.fsizeDepth / 2.0f && pPosOld->z < g_aMeshField.pos.z + g_aMeshField.fsizeDepth / 2.0f)
	{
		if (pPosOld->y >= g_aMeshField.pos.y)
		{
			if (pPos->y < g_aMeshField.pos.y)
			{
				pPos->y = g_aMeshField.pos.y;
				bLand = true;
			}
		}
	}
	return bLand;
}