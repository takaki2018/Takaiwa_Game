//-----------------------------------------------------------------
//
// コイン (Coin.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "Coin.h"
#include "wall.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "score.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_COIN			(8)
#define POLYGON_SIZE		(5.0f)		// ポリゴンのサイズ
#define PI_QUARTER			(D3DX_PI / 4.0f)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
Coin g_aCoin[MAX_COIN];										// モデル情報
LPDIRECT3DTEXTURE9 g_apTextureCoin[MAX_COIN] = {};			// テクスチャへのポインタ

//-----------------------------------------------------------------
// コインの初期化処理
//-----------------------------------------------------------------
HRESULT InitCoin(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_COIN; nCnt++)
	{
		// 情報の初期化
		g_aCoin[nCnt].pMeshModel = NULL;
		g_aCoin[nCnt].pBuffMatModel = NULL;
		g_aCoin[nCnt].nNumMatModel = NULL;
		g_aCoin[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].rotDest = g_aCoin[nCnt].rot;
		g_aCoin[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].nIdx = -1;
		g_aCoin[nCnt].bUse = false;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/coin.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aCoin[nCnt].pBuffMatModel,
			NULL,
			&g_aCoin[nCnt].nNumMatModel,
			&g_aCoin[nCnt].pMeshModel)))
		{
			return E_FAIL;
		}

		// コインのテクスチャ
		TexCoin(nCnt);

		// コインの頂点
		VecCoin(nCnt);
	}

	// コインの設置
	SetCoin(D3DXVECTOR3(600.0f, 0.0f, 0.0f));
	SetCoin(D3DXVECTOR3(-200.0f, 0.0f, 400.0f));
	SetCoin(D3DXVECTOR3(0.0f, 0.0f, 600.0f));
	SetCoin(D3DXVECTOR3(700.0f, 0.0f, -200.0f));
	SetCoin(D3DXVECTOR3(100.0f, 0.0f, 100.0f));
	SetCoin(D3DXVECTOR3(-500.0f, 0.0f, -400.0f));
	SetCoin(D3DXVECTOR3(200.0f, 0.0f, -100.0f));
	SetCoin(D3DXVECTOR3(0.0f, 0.0f, -500.0f));
	
	return S_OK;
}

//-----------------------------------------------------------------
// コインの終了処理
//-----------------------------------------------------------------
void UninitCoin(void)
{
	for (int nCnt = 0; nCnt < MAX_COIN; nCnt++)
	{
		// メッシュ情報の開放
		if (g_aCoin[nCnt].pBuffMatModel != NULL)
		{
			g_aCoin[nCnt].pBuffMatModel->Release();
			g_aCoin[nCnt].pBuffMatModel = NULL;
		}

		// マテリアル情報の開放
		if (g_aCoin[nCnt].pMeshModel != NULL)
		{
			g_aCoin[nCnt].pMeshModel->Release();
			g_aCoin[nCnt].pMeshModel = NULL;
		}
	}
}

//-----------------------------------------------------------------
// コインの更新処理
//-----------------------------------------------------------------
void UpdateCoin(void)
{
	// 構造体のポインタ変数
	Player *pPlayer = GetPlayer();
	Coin *pCoin = &g_aCoin[0];

	// 変数宣言
	int nCntTrue = 0;

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++,pCoin++)
	{
		if (pCoin->bUse == true)
		{// 使われているのでカウントする
			nCntTrue++;

			// 回転させる
			pCoin->rot.y += 0.05f;

			// 3.14fを超えたら6.28fを引く
			if (pCoin->rot.y > D3DX_PI)
			{
				pCoin->rot.y -= D3DX_PI * 2.0f;
			}
		}
	}
	if (nCntTrue == 0)
	{// コインの数が0枚の時フェードアウト
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------
//コインの描画処理
//-----------------------------------------------------------------
void DrawCoin(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// 変数の受け渡し
	pDevice = GetDevice();

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aCoin[nCntCoin].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCoin[nCntCoin].rot.y, g_aCoin[nCntCoin].rot.x, g_aCoin[nCntCoin].rot.z);
			D3DXMatrixMultiply(&g_aCoin[nCntCoin].mtxWorld, &g_aCoin[nCntCoin].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aCoin[nCntCoin].pos.x, g_aCoin[nCntCoin].pos.y, g_aCoin[nCntCoin].pos.z);
			D3DXMatrixMultiply(&g_aCoin[nCntCoin].mtxWorld, &g_aCoin[nCntCoin].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aCoin[nCntCoin].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aCoin[nCntCoin].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aCoin[nCntCoin].nNumMatModel; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureCoin[nCntMat]);

				// モデル(パーツ)の描画
				g_aCoin[nCntCoin].pMeshModel->DrawSubset(nCntMat);
			}
			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------
// コインの設置
//-----------------------------------------------------------------
void SetCoin(D3DXVECTOR3 pos)
{
	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == false)
		{
			// 座標の設定
			g_aCoin[nCntCoin].pos = pos;

			// 外積を用いた当たり判定
			CrossProductCoin(nCntCoin);

			// 使用状態をtrueにする
			g_aCoin[nCntCoin].bUse = true;

			// 影の配置
			g_aCoin[nCntCoin].nIdx = SetShadow(D3DXVECTOR3(g_aCoin[nCntCoin].pos.x, 0.0f, g_aCoin[nCntCoin].pos.z), SHADOW_SIZE, SHADOW_SIZE);

			break;
		}
	}
}

//-----------------------------------------------------------------
// コインのテクスチャ
//-----------------------------------------------------------------
void TexCoin(int nCntCoin)
{
	// 変数宣言
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ

	// 変数の受け渡し
	pDevice = GetDevice();

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aCoin[nCntCoin].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aCoin[nCntCoin].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureCoin[nCntMat]);
		}
	}
}

//-----------------------------------------------------------------
// コインの頂点座標
//-----------------------------------------------------------------
void VecCoin(int nCntCoin)
{
	// 変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数の取得
	nNumVtx = g_aCoin[nCntCoin].pMeshModel->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_aCoin[nCntCoin].pMeshModel->GetFVF());

	// 頂点バッファのロック
	g_aCoin[nCntCoin].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点を比較してモデルの最小値最大値を抜き出す
		if (g_aCoin[nCntCoin].vtxMinModel.x > vtx.x)
		{// X座標の最小値
			g_aCoin[nCntCoin].vtxMinModel.x = vtx.x;
		}
		else if (g_aCoin[nCntCoin].vtxMaxModel.x < vtx.x)
		{// X座標の最大値
			g_aCoin[nCntCoin].vtxMaxModel.x = vtx.x;
		}

		if (g_aCoin[nCntCoin].vtxMinModel.y > vtx.y)
		{// Y座標の最小値
			g_aCoin[nCntCoin].vtxMinModel.y = vtx.y;
		}
		else if (g_aCoin[nCntCoin].vtxMaxModel.y < vtx.y)
		{// Y座標の最大値
			g_aCoin[nCntCoin].vtxMaxModel.y = vtx.y;
		}

		if (g_aCoin[nCntCoin].vtxMinModel.z > vtx.z)
		{// Z座標の最小値
			g_aCoin[nCntCoin].vtxMinModel.z = vtx.z;
		}
		else if (g_aCoin[nCntCoin].vtxMaxModel.z < vtx.z)
		{// Z座標の最大値
			g_aCoin[nCntCoin].vtxMaxModel.z = vtx.z;
		}
		// 頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;
	}
	// 頂点バッファのアンロック
	g_aCoin[0].pMeshModel->UnlockVertexBuffer();
}

//-----------------------------------------------------------------
// 外積を用いたコインの当たり判定
//-----------------------------------------------------------------
void CrossProductCoin(int nCntCoin)
{
	// 変数の初期化
	g_aCoin[nCntCoin].aPos[0] = D3DXVECTOR3(g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].vtxMinModel.x, 0.0f,g_aCoin[nCntCoin].pos.z + g_aCoin[nCntCoin].vtxMinModel.z);
	g_aCoin[nCntCoin].aPos[1] = D3DXVECTOR3(g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].vtxMinModel.x, 0.0f,g_aCoin[nCntCoin].pos.z + g_aCoin[nCntCoin].vtxMaxModel.z);
	g_aCoin[nCntCoin].aPos[2] = D3DXVECTOR3(g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].vtxMaxModel.x, 0.0f,g_aCoin[nCntCoin].pos.z + g_aCoin[nCntCoin].vtxMaxModel.z);
	g_aCoin[nCntCoin].aPos[3] = D3DXVECTOR3(g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].vtxMaxModel.x, 0.0f,g_aCoin[nCntCoin].pos.z + g_aCoin[nCntCoin].vtxMinModel.z);

	// 変数の初期化
	g_aCoin[nCntCoin].aVecA[0] = g_aCoin[nCntCoin].aPos[1] - g_aCoin[nCntCoin].aPos[0];
	g_aCoin[nCntCoin].aVecA[1] = g_aCoin[nCntCoin].aPos[2] - g_aCoin[nCntCoin].aPos[1];
	g_aCoin[nCntCoin].aVecA[2] = g_aCoin[nCntCoin].aPos[3] - g_aCoin[nCntCoin].aPos[2];
	g_aCoin[nCntCoin].aVecA[3] = g_aCoin[nCntCoin].aPos[0] - g_aCoin[nCntCoin].aPos[3];
}

//-----------------------------------------------------------------
// オブジェクトの当たり判定
//-----------------------------------------------------------------
bool CollisionCoin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// 構造体のポインタ変数
	Coin *pCoin = &g_aCoin[0];

	// 変数宣言
	bool bGetCoin = false;		// コインを獲得したかどうか

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++, pCoin++)
	{
		if (pCoin->bUse == true)
		{
			// 外積を用いた当たり判定
			pCoin->aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pCoin->aPos[0].x, 0.0f, pPos->z - pCoin->aPos[0].z);
			pCoin->aVecB[1] = D3DXVECTOR3(pPos->x - pCoin->aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pCoin->aPos[1].z);
			pCoin->aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pCoin->aPos[2].x, 0.0f, pPos->z - pCoin->aPos[2].z);
			pCoin->aVecB[3] = D3DXVECTOR3(pPos->x - pCoin->aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pCoin->aPos[3].z);

			pCoin->fVec[0] = (pCoin->aVecA[0].z * pCoin->aVecB[0].x) - (pCoin->aVecA[0].x * pCoin->aVecB[0].z);
			pCoin->fVec[1] = (pCoin->aVecA[1].z * pCoin->aVecB[1].x) - (pCoin->aVecA[1].x * pCoin->aVecB[1].z);
			pCoin->fVec[2] = (pCoin->aVecA[2].z * pCoin->aVecB[2].x) - (pCoin->aVecA[2].x * pCoin->aVecB[2].z);
			pCoin->fVec[3] = (pCoin->aVecA[3].z * pCoin->aVecB[3].x) - (pCoin->aVecA[3].x * pCoin->aVecB[3].z);

			if (pPos->y <= pCoin->pos.y + pCoin->vtxMaxModel.y - pvtxMin->y &&
				pPos->y >= pCoin->pos.y + pCoin->vtxMinModel.y - pvtxMax->y)
			{
				if (pCoin->fVec[0] > 0.0f && pCoin->fVec[1] > 0.0f &&
					pCoin->fVec[2] > 0.0f && pCoin->fVec[3] > 0.0f)
				{
					// 使用状態をfalseにする
					pCoin->bUse = false;

					// スコアを加算する
					AddScore(1000);

					// 影を消す
					DeleteShadow(pCoin->nIdx);
				}
			}
		}
	}

	return bGetCoin;
}