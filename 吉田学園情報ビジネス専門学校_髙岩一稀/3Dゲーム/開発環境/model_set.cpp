//-----------------------------------------------------------------
//
// モデルセット (model_set.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "model_set.h"
#include "shadow.h"

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
Model g_Model;												// モデル情報

//-----------------------------------------------------------------
// モデルセットの初期化処理
//-----------------------------------------------------------------
HRESULT InitModelSet(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	// テキストファイルの読み込み
	LoadModelSet();

	for (int nCnt = 0; nCnt < g_Model.nNumModel; nCnt++)
	{
		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(&g_Model.modelInfo[nCnt].aFileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Model.modelInfo[nCnt].pBuffMatModel,
			NULL,
			&g_Model.modelInfo[nCnt].nNumMatModel,
			&g_Model.modelInfo[nCnt].pMeshModel)))
		{
			return E_FAIL;
		}

		// モデルのテクスチャ
		TexModel(nCnt);

		// モデルの頂点
		VecModel(nCnt);
	}
	// モデルの配置
	for (int nCntModel = 0; nCntModel < g_Model.nCntModel; nCntModel++)
	{
		// 使用状態をtrueにする
		g_Model.modelSet[nCntModel].bUse = true;
	}

	return S_OK;
}

//-----------------------------------------------------------------
// モデルセットの終了処理
//-----------------------------------------------------------------
void UninitModelSet(void)
{
	for (int nCnt = 0; nCnt < g_Model.nNumModel; nCnt++)
	{
		// メッシュ情報の開放
		if (g_Model.modelInfo[nCnt].pBuffMatModel != NULL)
		{
			g_Model.modelInfo[nCnt].pBuffMatModel->Release();
			g_Model.modelInfo[nCnt].pBuffMatModel = NULL;
		}

		// マテリアル情報の開放
		if (g_Model.modelInfo[nCnt].pMeshModel != NULL)
		{
			g_Model.modelInfo[nCnt].pMeshModel->Release();
			g_Model.modelInfo[nCnt].pMeshModel = NULL;
		}
	}
}

//-----------------------------------------------------------------
// モデルセットの更新処理
//-----------------------------------------------------------------
void UpdateModelSet(void)
{

}

//-----------------------------------------------------------------
// モデルセットの描画処理
//-----------------------------------------------------------------
void DrawModelSet(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// 変数の受け渡し
	pDevice = GetDevice();

	for (int nModelSet = 0; nModelSet < MAX_MODEL; nModelSet++)
	{
		if (g_Model.modelSet[nModelSet].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Model.modelSet[nModelSet].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model.modelSet[nModelSet].rot.y, g_Model.modelSet[nModelSet].rot.x, g_Model.modelSet[nModelSet].rot.z);
			D3DXMatrixMultiply(&g_Model.modelSet[nModelSet].mtxWorld, &g_Model.modelSet[nModelSet].mtxWorld, &mtxRot);

			// 頂点座標の補正
			D3DXVec3TransformCoord(&g_Model.modelSet[nModelSet].vtxMaxModel, &g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].vtxMaxModel, &g_Model.modelSet[nModelSet].mtxWorld);
			D3DXVec3TransformCoord(&g_Model.modelSet[nModelSet].vtxMinModel, &g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].vtxMinModel, &g_Model.modelSet[nModelSet].mtxWorld);

			// 最大値最小値の補正
			VecModelSet(nModelSet);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Model.modelSet[nModelSet].pos.x, g_Model.modelSet[nModelSet].pos.y, g_Model.modelSet[nModelSet].pos.z);
			D3DXMatrixMultiply(&g_Model.modelSet[nModelSet].mtxWorld, &g_Model.modelSet[nModelSet].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Model.modelSet[nModelSet].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].nNumMatModel; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].apTextureModelSet[nCntMat]);

				// モデル(パーツ)の描画
				g_Model.modelInfo[g_Model.modelSet[nModelSet].nIdx].pMeshModel->DrawSubset(nCntMat);
			}
			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------
// モデルのテクスチャ
//-----------------------------------------------------------------
void TexModel(int nModelSet)
{
	// 変数宣言
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ

	// 変数の受け渡し
	pDevice = GetDevice();

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_Model.modelInfo[nModelSet].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Model.modelInfo[nModelSet].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_Model.modelInfo[nModelSet].apTextureModelSet[nCntMat]);
		}
	}
}

//-----------------------------------------------------------------
// モデルの頂点座標
//-----------------------------------------------------------------
void VecModel(int nModelSet)
{
	// 変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数の取得
	nNumVtx = g_Model.modelInfo[nModelSet].pMeshModel->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_Model.modelInfo[nModelSet].pMeshModel->GetFVF());

	// 頂点バッファのロック
	g_Model.modelInfo[nModelSet].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点を比較してモデルの最小値最大値を抜き出す
		if (g_Model.modelInfo[nModelSet].vtxMinModel.x > vtx.x)
		{// X座標の最小値
			g_Model.modelInfo[nModelSet].vtxMinModel.x = vtx.x;
		}
		else if (g_Model.modelInfo[nModelSet].vtxMaxModel.x < vtx.x)
		{// X座標の最大値
			g_Model.modelInfo[nModelSet].vtxMaxModel.x = vtx.x;
		}

		if (g_Model.modelInfo[nModelSet].vtxMinModel.y > vtx.y)
		{// Y座標の最小値
			g_Model.modelInfo[nModelSet].vtxMinModel.y = vtx.y;
		}
		else if (g_Model.modelInfo[nModelSet].vtxMaxModel.y < vtx.y)
		{// Y座標の最大値
			g_Model.modelInfo[nModelSet].vtxMaxModel.y = vtx.y;
		}

		if (g_Model.modelInfo[nModelSet].vtxMinModel.z > vtx.z)
		{// Z座標の最小値
			g_Model.modelInfo[nModelSet].vtxMinModel.z = vtx.z;
		}
		else if (g_Model.modelInfo[nModelSet].vtxMaxModel.z < vtx.z)
		{// Z座標の最大値
			g_Model.modelInfo[nModelSet].vtxMaxModel.z = vtx.z;
		}
		// 頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;
	}
	// 頂点バッファのアンロック
	g_Model.modelInfo[nModelSet].pMeshModel->UnlockVertexBuffer();
}

//-----------------------------------------------------------------
// モデルの頂点座標の補正
//-----------------------------------------------------------------
void VecModelSet(int nNumModelSet)
{
	// 最大値と最小値の入れ替え
	if (g_Model.modelSet[nNumModelSet].vtxMaxModel.x < g_Model.modelSet[nNumModelSet].vtxMinModel.x)
	{
		// 変数宣言
		float fVecX;		// 保存用

		// 最大値と最小値を入れ替える
		fVecX = g_Model.modelSet[nNumModelSet].vtxMaxModel.x;
		g_Model.modelSet[nNumModelSet].vtxMaxModel.x = g_Model.modelSet[nNumModelSet].vtxMinModel.x;
		g_Model.modelSet[nNumModelSet].vtxMinModel.x = fVecX;
	}
	if (g_Model.modelSet[nNumModelSet].vtxMaxModel.z < g_Model.modelSet[nNumModelSet].vtxMinModel.z)
	{
		// 変数宣言
		float fVecZ;		// 保存用

		// 最大値と最小値を入れ替える
		fVecZ = g_Model.modelSet[nNumModelSet].vtxMaxModel.z;
		g_Model.modelSet[nNumModelSet].vtxMaxModel.z = g_Model.modelSet[nNumModelSet].vtxMinModel.z;
		g_Model.modelSet[nNumModelSet].vtxMinModel.z = fVecZ;
	}
}

//-----------------------------------------------------------------
// 外積を用いたモデルの当たり判定
//-----------------------------------------------------------------
void CrossProductModelSet(int nModelSet)
{
	// 変数の初期化
	g_Model.modelSet[nModelSet].aPos[0] = D3DXVECTOR3(g_Model.modelSet[nModelSet].pos.x + g_Model.modelSet[nModelSet].vtxMinModel.x, 0.0f, g_Model.modelSet[nModelSet].pos.z + g_Model.modelSet[nModelSet].vtxMinModel.z);
	g_Model.modelSet[nModelSet].aPos[1] = D3DXVECTOR3(g_Model.modelSet[nModelSet].pos.x + g_Model.modelSet[nModelSet].vtxMinModel.x, 0.0f, g_Model.modelSet[nModelSet].pos.z + g_Model.modelSet[nModelSet].vtxMaxModel.z);
	g_Model.modelSet[nModelSet].aPos[2] = D3DXVECTOR3(g_Model.modelSet[nModelSet].pos.x + g_Model.modelSet[nModelSet].vtxMaxModel.x, 0.0f, g_Model.modelSet[nModelSet].pos.z + g_Model.modelSet[nModelSet].vtxMaxModel.z);
	g_Model.modelSet[nModelSet].aPos[3] = D3DXVECTOR3(g_Model.modelSet[nModelSet].pos.x + g_Model.modelSet[nModelSet].vtxMaxModel.x, 0.0f, g_Model.modelSet[nModelSet].pos.z + g_Model.modelSet[nModelSet].vtxMinModel.z);

	// 変数の初期化
	g_Model.modelSet[nModelSet].aVecA[0] = g_Model.modelSet[nModelSet].aPos[1] - g_Model.modelSet[nModelSet].aPos[0];
	g_Model.modelSet[nModelSet].aVecA[1] = g_Model.modelSet[nModelSet].aPos[2] - g_Model.modelSet[nModelSet].aPos[1];
	g_Model.modelSet[nModelSet].aVecA[2] = g_Model.modelSet[nModelSet].aPos[3] - g_Model.modelSet[nModelSet].aPos[2];
	g_Model.modelSet[nModelSet].aVecA[3] = g_Model.modelSet[nModelSet].aPos[0] - g_Model.modelSet[nModelSet].aPos[3];
}

//-----------------------------------------------------------------
// モデルの当たり判定
//-----------------------------------------------------------------
bool CollisionModelSet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// 構造体のポインタ変数
	Model *pModel = &g_Model;

	// 変数宣言
	bool bCollisionModel = false;		// コインを獲得したかどうか

	for (int nModelSet = 0; nModelSet < MAX_MODEL; nModelSet++)
	{
		if (pModel->modelSet[nModelSet].bUse == true)
		{
			// 外積を用いた当たり判定
			CrossProductModelSet(nModelSet);

			// 外積を用いた当たり判定
			pModel->modelSet[nModelSet].aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pModel->modelSet[nModelSet].aPos[0].x, 0.0f, pPos->z - pModel->modelSet[nModelSet].aPos[0].z);
			pModel->modelSet[nModelSet].aVecB[1] = D3DXVECTOR3(pPos->x - pModel->modelSet[nModelSet].aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pModel->modelSet[nModelSet].aPos[1].z);
			pModel->modelSet[nModelSet].aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pModel->modelSet[nModelSet].aPos[2].x, 0.0f, pPos->z - pModel->modelSet[nModelSet].aPos[2].z);
			pModel->modelSet[nModelSet].aVecB[3] = D3DXVECTOR3(pPos->x - pModel->modelSet[nModelSet].aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pModel->modelSet[nModelSet].aPos[3].z);

			// 外積の値
			pModel->modelSet[nModelSet].fVec[0] = (pModel->modelSet[nModelSet].aVecA[0].z * pModel->modelSet[nModelSet].aVecB[0].x) - (pModel->modelSet[nModelSet].aVecA[0].x * pModel->modelSet[nModelSet].aVecB[0].z);
			pModel->modelSet[nModelSet].fVec[1] = (pModel->modelSet[nModelSet].aVecA[1].z * pModel->modelSet[nModelSet].aVecB[1].x) - (pModel->modelSet[nModelSet].aVecA[1].x * pModel->modelSet[nModelSet].aVecB[1].z);
			pModel->modelSet[nModelSet].fVec[2] = (pModel->modelSet[nModelSet].aVecA[2].z * pModel->modelSet[nModelSet].aVecB[2].x) - (pModel->modelSet[nModelSet].aVecA[2].x * pModel->modelSet[nModelSet].aVecB[2].z);
			pModel->modelSet[nModelSet].fVec[3] = (pModel->modelSet[nModelSet].aVecA[3].z * pModel->modelSet[nModelSet].aVecB[3].x) - (pModel->modelSet[nModelSet].aVecA[3].x * pModel->modelSet[nModelSet].aVecB[3].z);

			if (pModel->modelSet[nModelSet].fVec[0] > 0.0f && pModel->modelSet[nModelSet].fVec[1] > 0.0f &&
				pModel->modelSet[nModelSet].fVec[2] > 0.0f && pModel->modelSet[nModelSet].fVec[3] > 0.0f)
			{
				if ((pPosOld->y >= pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMaxModel.y) && 
					(pPos->y < pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMaxModel.y))
				{// モデルの上に立つ
					pPos->y = pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMaxModel.y;

					bCollisionModel = true;
				}
				else if ((pPosOld->y + pvtxMax->y <= pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMinModel.y) &&
					(pPos->y + pvtxMax->y > pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMinModel.y))
				{
					pPos->y = pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMinModel.y - pvtxMax->y;
				}
				else if (pPos->y <= pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMaxModel.y - pvtxMin->y &&
					pPos->y >= pModel->modelSet[nModelSet].pos.y + pModel->modelSet[nModelSet].vtxMinModel.y - pvtxMax->y)
				{
					if (pPos->x > pPosOld->x && (pPosOld->x < pModel->modelSet[nModelSet].pos.x + pModel->modelSet[nModelSet].vtxMinModel.x))
					{// プレイヤーが左から当たったとき
						pPos->x = pModel->modelSet[nModelSet].pos.x + pModel->modelSet[nModelSet].vtxMinModel.x - pvtxMax->x;
					}
					else if (pPos->x < pPosOld->x && (pPosOld->x > pModel->modelSet[nModelSet].pos.x + pModel->modelSet[nModelSet].vtxMaxModel.x))
					{// プレイヤーが右から当たったとき
						pPos->x = pModel->modelSet[nModelSet].pos.x + pModel->modelSet[nModelSet].vtxMaxModel.x - pvtxMin->x;
					}
					else if (pPos->z > pPosOld->z && pPosOld->z < pModel->modelSet[nModelSet].pos.z + pModel->modelSet[nModelSet].vtxMinModel.z)
					{// プレイヤーが手前から当たったとき
						pPos->z = pModel->modelSet[nModelSet].pos.z + pModel->modelSet[nModelSet].vtxMinModel.z - pvtxMax->z;
					}
					else if (pPos->z < pPosOld->z && pPosOld->z > pModel->modelSet[nModelSet].pos.z + pModel->modelSet[nModelSet].vtxMaxModel.z)
					{// プレイヤーが奥から当たったとき
						pPos->z = pModel->modelSet[nModelSet].pos.z + pModel->modelSet[nModelSet].vtxMaxModel.z - pvtxMin->z;
					}

				}
			}
		}
	}

	return bCollisionModel;
}

//-----------------------------------------------------------------
// モデル配置情報の読み込み
//-----------------------------------------------------------------
void LoadModelSet(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/model.txt", "r");
	
	// 変数の初期化
	g_Model.nCntModel = 0;

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
				fscanf(pFile, "%s %d", &aText[0], &g_Model.nNumModel);
			}
			// MODEL_FILENAMEを見つけたらXファイル名を格納
			if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < g_Model.nNumModel)
			{
				fscanf(pFile, "%s %s", &aText[0], &g_Model.modelInfo[nCntModel].aFileName[0]);

				// モデルの総数だけ格納するのでカウントを増やす
				nCntModel++;
			}
			// 各パーツに情報を読み込む許可があるとき処理
			if (strcmp(&aText[0], "MODELSET") == 0)
			{
				// 変数を加算
				nCntModelSet++;

				// 配置するモデル数を加算
				g_Model.nCntModel++;

				bModelSet = true;
			}
			if (bModelSet == true)
			{
				// TYPEを見つけたら番号を格納
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &g_Model.modelSet[nCntModelSet].nIdx);
				}
				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&g_Model.modelSet[nCntModelSet].pos.x,
						&g_Model.modelSet[nCntModelSet].pos.y,
						&g_Model.modelSet[nCntModelSet].pos.z);
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

					g_Model.modelSet[nCntModelSet].rot.x = rot.x * D3DX_PI;
					g_Model.modelSet[nCntModelSet].rot.y = rot.y * D3DX_PI;
					g_Model.modelSet[nCntModelSet].rot.z = rot.z * D3DX_PI;
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