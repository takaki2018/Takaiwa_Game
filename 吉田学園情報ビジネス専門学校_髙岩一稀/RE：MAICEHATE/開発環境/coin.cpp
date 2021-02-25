//-----------------------------------------------------------------
//
// コイン (Coin.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "Coin.h"
#include "coin_sheets.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "fade.h"
#include "score.h"
#include "sound.h"
#include "setparticle.h"

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
CoinInfo g_CoinInfo;								// モデル情報
bool g_bNoCoin;										// コインがあるかないか

//-----------------------------------------------------------------
// コインの初期化処理
//-----------------------------------------------------------------
HRESULT InitCoin(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();
	g_bNoCoin = false;

	// コインの配置情報の読み込み
	LoadSetCoin();

	for (int nCntCoin = 0; nCntCoin < g_CoinInfo.nNumModel; nCntCoin++)
	{
		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(&g_CoinInfo.coin[nCntCoin].aFileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_CoinInfo.coin[nCntCoin].pBuffMatModel,
			NULL,		
			&g_CoinInfo.coin[nCntCoin].nNumMatModel,
			&g_CoinInfo.coin[nCntCoin].pMeshModel)))
		{
			return E_FAIL;
		}

		// コインのテクスチャ
		TexCoin(nCntCoin);

		// コインの頂点
		VecCoin(nCntCoin);
	}
	// モデルの配置
	for (int nCntCoin = 0; nCntCoin < g_CoinInfo.nCntCoin; nCntCoin++)
	{
		// 使用状態をtrueにする
		g_CoinInfo.coinSet[nCntCoin].bUse = true;

		// 影の配置
		g_CoinInfo.coinSet[nCntCoin].nIdxShadow = SetShadow(D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x, g_CoinInfo.coinSet[nCntCoin].pos.y, g_CoinInfo.coinSet[nCntCoin].pos.z), SHADOW_SIZE, SHADOW_SIZE);
	}

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
		if (g_CoinInfo.coin[nCnt].pBuffMatModel != NULL)
		{
			g_CoinInfo.coin[nCnt].pBuffMatModel->Release();
			g_CoinInfo.coin[nCnt].pBuffMatModel = NULL;
		}

		// マテリアル情報の開放
		if (g_CoinInfo.coin[nCnt].pMeshModel != NULL)
		{
			g_CoinInfo.coin[nCnt].pMeshModel->Release();
			g_CoinInfo.coin[nCnt].pMeshModel = NULL;
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
	CoinInfo *pCoinInfo = &g_CoinInfo;

	// 変数宣言
	int nCntTrue = 0;

	for (int nCntCoin = 0; nCntCoin < pCoinInfo->nCntCoin; nCntCoin++)
	{
		if (pCoinInfo->coinSet[nCntCoin].bUse == true)
		{// 使われているのでカウントする
			nCntTrue++;

			// 回転させる
			pCoinInfo->coinSet[nCntCoin].rot.y += 0.05f;

			// 3.14fを超えたら6.28fを引く
			if (pCoinInfo->coinSet[nCntCoin].rot.y > D3DX_PI)
			{
				pCoinInfo->coinSet[nCntCoin].rot.y -= D3DX_PI * 2.0f;
			}
		}
	}
	if (nCntTrue == 0)
	{// コインの数が0枚の時ないとする
		g_bNoCoin = true;
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

	for (int nCntCoin = 0; nCntCoin < g_CoinInfo.nCntCoin; nCntCoin++)
	{
		if (g_CoinInfo.coinSet[nCntCoin].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_CoinInfo.coinSet[nCntCoin].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_CoinInfo.coinSet[nCntCoin].rot.y, g_CoinInfo.coinSet[nCntCoin].rot.x, g_CoinInfo.coinSet[nCntCoin].rot.z);
			D3DXMatrixMultiply(&g_CoinInfo.coinSet[nCntCoin].mtxWorld, &g_CoinInfo.coinSet[nCntCoin].mtxWorld, &mtxRot);

			// 頂点座標の補正
			D3DXVec3TransformCoord(&g_CoinInfo.coinSet[nCntCoin].vtxMaxModel, &g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].vtxMaxModel, &g_CoinInfo.coinSet[nCntCoin].mtxWorld);
			D3DXVec3TransformCoord(&g_CoinInfo.coinSet[nCntCoin].vtxMinModel, &g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].vtxMinModel, &g_CoinInfo.coinSet[nCntCoin].mtxWorld);

			// 頂点座標の補正
			VecCoinSet(nCntCoin);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_CoinInfo.coinSet[nCntCoin].pos.x, g_CoinInfo.coinSet[nCntCoin].pos.y, g_CoinInfo.coinSet[nCntCoin].pos.z);
			D3DXMatrixMultiply(&g_CoinInfo.coinSet[nCntCoin].mtxWorld, &g_CoinInfo.coinSet[nCntCoin].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_CoinInfo.coinSet[nCntCoin].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].nNumMatModel; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].apTextureModelSet[nCntMat]);

				// モデル(パーツ)の描画
				g_CoinInfo.coin[g_CoinInfo.coinSet[nCntCoin].nIdx].pMeshModel->DrawSubset(nCntMat);
			}
			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
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
	pMat = (D3DXMATERIAL*)g_CoinInfo.coin[nCntCoin].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_CoinInfo.coin[nCntCoin].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_CoinInfo.coin[nCntCoin].apTextureModelSet[nCntMat]);
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
	nNumVtx = g_CoinInfo.coin[nCntCoin].pMeshModel->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_CoinInfo.coin[nCntCoin].pMeshModel->GetFVF());

	// 頂点バッファのロック
	g_CoinInfo.coin[nCntCoin].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点を比較してモデルの最小値最大値を抜き出す
		if (g_CoinInfo.coin[nCntCoin].vtxMinModel.x > vtx.x)
		{// X座標の最小値
			g_CoinInfo.coin[nCntCoin].vtxMinModel.x = vtx.x;
		}
		else if (g_CoinInfo.coin[nCntCoin].vtxMaxModel.x < vtx.x)
		{// X座標の最大値
			g_CoinInfo.coin[nCntCoin].vtxMaxModel.x = vtx.x;
		}

		if (g_CoinInfo.coin[nCntCoin].vtxMinModel.y > vtx.y)
		{// Y座標の最小値
			g_CoinInfo.coin[nCntCoin].vtxMinModel.y = vtx.y;
		}
		else if (g_CoinInfo.coin[nCntCoin].vtxMaxModel.y < vtx.y)
		{// Y座標の最大値
			g_CoinInfo.coin[nCntCoin].vtxMaxModel.y = vtx.y;
		}

		if (g_CoinInfo.coin[nCntCoin].vtxMinModel.z > vtx.z)
		{// Z座標の最小値
			g_CoinInfo.coin[nCntCoin].vtxMinModel.z = vtx.z;
		}
		else if (g_CoinInfo.coin[nCntCoin].vtxMaxModel.z < vtx.z)
		{// Z座標の最大値
			g_CoinInfo.coin[nCntCoin].vtxMaxModel.z = vtx.z;
		}
		// 頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;
	}
	// 頂点バッファのアンロック
	g_CoinInfo.coin[nCntCoin].pMeshModel->UnlockVertexBuffer();
}

//-----------------------------------------------------------------
// モデルの頂点座標の補正
//-----------------------------------------------------------------
void VecCoinSet(int nCntCoin)
{
	// 最大値と最小値の入れ替え
	if (g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x < g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x)
	{
		// 変数宣言
		float fVecX;		// 保存用

		// 最大値と最小値を入れ替える
		fVecX = g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x;
		g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x = g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x;
		g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x = fVecX;
	}
	if (g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z < g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z)
	{
		// 変数宣言
		float fVecZ;		// 保存用

		// 最大値と最小値を入れ替える
		fVecZ = g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z;
		g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z = g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z;
		g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z = fVecZ;
	}
}

//-----------------------------------------------------------------
// オブジェクトの当たり判定
//-----------------------------------------------------------------
bool CollisionCoin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// 構造体のポインタ変数
	CoinInfo *pCoinInfo = &g_CoinInfo;

	// 変数宣言
	bool bGetCoin = false;		// コインを獲得したかどうか

	for (int nCntCoin = 0; nCntCoin < g_CoinInfo.nCntCoin; nCntCoin++)
	{
		if (pCoinInfo->coinSet[nCntCoin].bUse == true)
		{
			// コインの各頂点座標
			g_CoinInfo.coinSet[nCntCoin].aPos[0] = D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x + g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x, 0.0f, g_CoinInfo.coinSet[nCntCoin].pos.z + g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z);
			g_CoinInfo.coinSet[nCntCoin].aPos[1] = D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x + g_CoinInfo.coinSet[nCntCoin].vtxMinModel.x, 0.0f, g_CoinInfo.coinSet[nCntCoin].pos.z + g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z);
			g_CoinInfo.coinSet[nCntCoin].aPos[2] = D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x + g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x, 0.0f, g_CoinInfo.coinSet[nCntCoin].pos.z + g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.z);
			g_CoinInfo.coinSet[nCntCoin].aPos[3] = D3DXVECTOR3(g_CoinInfo.coinSet[nCntCoin].pos.x + g_CoinInfo.coinSet[nCntCoin].vtxMaxModel.x, 0.0f, g_CoinInfo.coinSet[nCntCoin].pos.z + g_CoinInfo.coinSet[nCntCoin].vtxMinModel.z);

			// 各頂点に向けてのベクトル
			g_CoinInfo.coinSet[nCntCoin].aVecA[0] = g_CoinInfo.coinSet[nCntCoin].aPos[1] - g_CoinInfo.coinSet[nCntCoin].aPos[0];
			g_CoinInfo.coinSet[nCntCoin].aVecA[1] = g_CoinInfo.coinSet[nCntCoin].aPos[2] - g_CoinInfo.coinSet[nCntCoin].aPos[1];
			g_CoinInfo.coinSet[nCntCoin].aVecA[2] = g_CoinInfo.coinSet[nCntCoin].aPos[3] - g_CoinInfo.coinSet[nCntCoin].aPos[2];
			g_CoinInfo.coinSet[nCntCoin].aVecA[3] = g_CoinInfo.coinSet[nCntCoin].aPos[0] - g_CoinInfo.coinSet[nCntCoin].aPos[3];

			// コインの各頂点から対象までのベクトル
			pCoinInfo->coinSet[nCntCoin].aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pCoinInfo->coinSet[nCntCoin].aPos[0].x, 0.0f, pPos->z - pCoinInfo->coinSet[nCntCoin].aPos[0].z);
			pCoinInfo->coinSet[nCntCoin].aVecB[1] = D3DXVECTOR3(pPos->x - pCoinInfo->coinSet[nCntCoin].aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pCoinInfo->coinSet[nCntCoin].aPos[1].z);
			pCoinInfo->coinSet[nCntCoin].aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pCoinInfo->coinSet[nCntCoin].aPos[2].x, 0.0f, pPos->z - pCoinInfo->coinSet[nCntCoin].aPos[2].z);
			pCoinInfo->coinSet[nCntCoin].aVecB[3] = D3DXVECTOR3(pPos->x - pCoinInfo->coinSet[nCntCoin].aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pCoinInfo->coinSet[nCntCoin].aPos[3].z);

			// 外積を用いた当たり判定
			pCoinInfo->coinSet[nCntCoin].fVec[0] = (pCoinInfo->coinSet[nCntCoin].aVecA[0].z * pCoinInfo->coinSet[nCntCoin].aVecB[0].x) - (pCoinInfo->coinSet[nCntCoin].aVecA[0].x * pCoinInfo->coinSet[nCntCoin].aVecB[0].z);
			pCoinInfo->coinSet[nCntCoin].fVec[1] = (pCoinInfo->coinSet[nCntCoin].aVecA[1].z * pCoinInfo->coinSet[nCntCoin].aVecB[1].x) - (pCoinInfo->coinSet[nCntCoin].aVecA[1].x * pCoinInfo->coinSet[nCntCoin].aVecB[1].z);
			pCoinInfo->coinSet[nCntCoin].fVec[2] = (pCoinInfo->coinSet[nCntCoin].aVecA[2].z * pCoinInfo->coinSet[nCntCoin].aVecB[2].x) - (pCoinInfo->coinSet[nCntCoin].aVecA[2].x * pCoinInfo->coinSet[nCntCoin].aVecB[2].z);
			pCoinInfo->coinSet[nCntCoin].fVec[3] = (pCoinInfo->coinSet[nCntCoin].aVecA[3].z * pCoinInfo->coinSet[nCntCoin].aVecB[3].x) - (pCoinInfo->coinSet[nCntCoin].aVecA[3].x * pCoinInfo->coinSet[nCntCoin].aVecB[3].z);

			if (pPos->y <= pCoinInfo->coinSet[nCntCoin].pos.y + pCoinInfo->coinSet[nCntCoin].vtxMaxModel.y - pvtxMin->y &&
				pPos->y >= pCoinInfo->coinSet[nCntCoin].pos.y + pCoinInfo->coinSet[nCntCoin].vtxMinModel.y - pvtxMax->y)
			{
				if (pCoinInfo->coinSet[nCntCoin].fVec[0] > 0.0f && pCoinInfo->coinSet[nCntCoin].fVec[1] > 0.0f &&
					pCoinInfo->coinSet[nCntCoin].fVec[2] > 0.0f && pCoinInfo->coinSet[nCntCoin].fVec[3] > 0.0f)
				{
					// 使用状態をfalseにする
					pCoinInfo->coinSet[nCntCoin].bUse = false;

					// スコアを加算する
					AddScore(1000);

					// コイン獲得状況の加算
					AddCoinSheets(1);

					// テスト
					CoinParticle(pCoinInfo->coinSet[nCntCoin].pos);

					// 音を出す
					PlaySound(SOUND_LABEL_SE_GETCOIN);

					// 影を消す
					DeleteShadow(pCoinInfo->coinSet[nCntCoin].nIdxShadow);
				}
			}
		}
	}

	return bGetCoin;
}

//-----------------------------------------------------------------
// コイン配置情報の読み込み
//-----------------------------------------------------------------
void LoadSetCoin(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/coin.txt", "r");

	// 変数の初期化
	g_CoinInfo.nCntCoin = 0;

	// 変数宣言
	bool bGetScript = false;
	bool bModelSet = false;

	char aLine[128];
	char aText[128];
	int nCntModel = 0;
	int nCntModelSet = -1;
	int nCntMotion = -1;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	// SCRIPTを探す
	while (fgets(aLine, 128, pFile) != NULL)
	{
		fscanf(pFile, "%s", &aText[0]);

		// SCRIPTを見つけたらそれ以降を読み込む
		if (strcmp(&aText[0], "SCRIPT") == 0)
		{
			bGetScript = true;
		}

		if (bGetScript == true)
		{
			// モデル情報の読み込み
			// NUM_MODELを見つけたらモデルの総数を格納
			if (strcmp(&aText[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s %d", &aText[0], &g_CoinInfo.nNumModel);
			}
			// MODEL_FILENAMEを見つけたらXファイル名を格納
			if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < g_CoinInfo.nNumModel)
			{
				fscanf(pFile, "%s %s", &aText[0], &g_CoinInfo.coin[nCntModel].aFileName[0]);

				// モデルの総数だけ格納するのでカウントを増やす
				nCntModel++;
			}
			// 各パーツに情報を読み込む許可があるとき処理
			if (strcmp(&aText[0], "MODELSET") == 0)
			{
				// 変数を加算
				nCntModelSet++;

				// 配置するモデル数を加算
				g_CoinInfo.nCntCoin++;

				bModelSet = true;
			}
			if (bModelSet == true)
			{
				// TYPEを見つけたら番号を格納
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &g_CoinInfo.coinSet[nCntModelSet].nIdx);
				}
				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&g_CoinInfo.coinSet[nCntModelSet].pos.x,
						&g_CoinInfo.coinSet[nCntModelSet].pos.y,
						&g_CoinInfo.coinSet[nCntModelSet].pos.z);
				}
				// ROTを見つけたら角度情報を格納
				if (strcmp(&aText[0], "ROT") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&rot.x,
						&rot.y,
						&rot.z);

					rot.x = rot.x / 180.0f;
					rot.y = rot.y / 180.0f;
					rot.z = rot.z / 180.0f;

					g_CoinInfo.coinSet[nCntModelSet].rot.x = rot.x * D3DX_PI;
					g_CoinInfo.coinSet[nCntModelSet].rot.y = rot.y * D3DX_PI;
					g_CoinInfo.coinSet[nCntModelSet].rot.z = rot.z * D3DX_PI;
				}
				// END_MODELSETKEYを見つけたらモデル配置をする
				if (strcmp(&aText[0], "END_MODELSET") == 0)
				{
					bModelSet = false;
				}
			}
			// END_SCRIPTを見つけたら読み込みを終える
			if (strcmp(&aText[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	// ファイルを閉じる
	fclose(pFile);
}

//-----------------------------------------------------------------
// コインがあるかないか
//-----------------------------------------------------------------
bool GetbNoCoin(void)
{
	return g_bNoCoin;
}