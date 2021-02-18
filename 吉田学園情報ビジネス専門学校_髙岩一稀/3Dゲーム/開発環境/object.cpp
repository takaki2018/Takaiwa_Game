//-----------------------------------------------------------------
//
// オブジェクト (object.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "object.h"
#include "wall.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_OBJECT			(1)
#define POLYGON_SIZE		(5.0f)		// ポリゴンのサイズ
#define MOVE_MODEL			(0.8f)
#define PI_QUARTER			(D3DX_PI / 4.0f)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
Object g_aCoin[MAX_OBJECT];											// モデル情報
D3DXVECTOR3 g_vtxMinObject, g_vtxMaxObject;						// 各頂点座標の最小値、最大値
LPDIRECT3DTEXTURE9 g_apTextureObject[8] = {};					// テクスチャへのポインタ

//-----------------------------------------------------------------
// オブジェクトの初期化処理
//-----------------------------------------------------------------
HRESULT InitObject(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();
	g_vtxMinObject = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_vtxMaxObject = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		// 情報の初期化
		g_aCoin[nCnt].pMeshModel = NULL;
		g_aCoin[nCnt].pBuffMatModel = NULL;
		g_aCoin[nCnt].nNumMatModel = NULL;
		g_aCoin[nCnt].pos = D3DXVECTOR3(50.0f, 10.0f, 0.0f);
		g_aCoin[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].rotDest = g_aCoin[nCnt].rot;
		g_aCoin[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCnt].nIdx = -1;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/test.x",
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
	}
	// 変数宣言
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aCoin[0].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aCoin[0].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureObject[nCntMat]);
		}
	}

	// 変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数の取得
	nNumVtx = g_aCoin[0].pMeshModel->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_aCoin[0].pMeshModel->GetFVF());

	// 頂点バッファのロック
	g_aCoin[0].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点を比較してモデルの最小値最大値を抜き出す
		if (g_vtxMinObject.x > vtx.x)
		{// X座標の最小値
			g_vtxMinObject.x = vtx.x;
		}
		else if (g_vtxMaxObject.x < vtx.x)
		{// X座標の最大値
			g_vtxMaxObject.x = vtx.x;
		}

		if (g_vtxMinObject.y > vtx.y)
		{// Y座標の最小値
			g_vtxMinObject.y = vtx.y;
		}
		else if (g_vtxMaxObject.y < vtx.y)
		{// Y座標の最大値
			g_vtxMaxObject.y = vtx.y;
		}

		if (g_vtxMinObject.z > vtx.z)
		{// Z座標の最小値
			g_vtxMinObject.z = vtx.z;
		}
		else if (g_vtxMaxObject.z < vtx.z)
		{// Z座標の最大値
			g_vtxMaxObject.z = vtx.z;
		}
		// 頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;
	}
	// 頂点バッファのアンロック
	g_aCoin[0].pMeshModel->UnlockVertexBuffer();

	// 影の配置
	g_aCoin[0].nIdx = SetShadow(D3DXVECTOR3(g_aCoin[0].pos.x, 0.0f, g_aCoin[0].pos.z), SHADOW_SIZE, SHADOW_SIZE);

	return S_OK;
}

//-----------------------------------------------------------------
// オブジェクトの終了処理
//-----------------------------------------------------------------
void UninitObject(void)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
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
// オブジェクトの更新処理
//-----------------------------------------------------------------
void UpdateObject(void)
{

}

//-----------------------------------------------------------------
// オブジェクトの描画処理
//-----------------------------------------------------------------
void DrawObject(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

									// 変数の受け渡し
	pDevice = GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aCoin[0].mtxWorld);

	// スケールを反映
	//g_model[0].mtxWorld._11 = 0.8f;
	//g_model[0].mtxWorld._22 = 0.8f;
	//g_model[0].mtxWorld._33 = 0.8f;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCoin[0].rot.y, g_aCoin[0].rot.x, g_aCoin[0].rot.z);
	D3DXMatrixMultiply(&g_aCoin[0].mtxWorld, &g_aCoin[0].mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aCoin[0].pos.x, g_aCoin[0].pos.y, g_aCoin[0].pos.z);
	D3DXMatrixMultiply(&g_aCoin[0].mtxWorld, &g_aCoin[0].mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aCoin[0].mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aCoin[0].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aCoin[0].nNumMatModel; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureObject[nCntMat]);

		// モデル(パーツ)の描画
		g_aCoin[0].pMeshModel->DrawSubset(nCntMat);
	}
	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//-----------------------------------------------------------------
// オブジェクトの当たり判定
//-----------------------------------------------------------------
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// 構造体のポインタ変数
	Object *pObject = &g_aCoin[0];

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++,pObject++)
	{
		if ((pPos->x - pvtxMin->x) < (pObject->pos.x + g_vtxMaxObject.x) &&
			(pPos->x + pvtxMax->x) > (pObject->pos.x - g_vtxMinObject.x))
		{// プレイヤーがブロックの中にいるとき
			if (pPosOld->z + pvtxMax->z <= (pObject->pos.z - g_vtxMinObject.z))
			{// プレイヤーがブロックの上にいるとき
				if (pPos->z + pvtxMax->z >(pObject->pos.z - g_vtxMinObject.z))
				{// めり込んだら処理
					pPos->z = pObject->pos.z - g_vtxMinObject.z - pvtxMax->z;
				}
			}
			else if ((pPosOld->z - pvtxMin->z) >= (pObject->pos.z + g_vtxMaxObject.z))
			{// プレイヤーがブロックの下にいるとき
				if ((pPos->z - pvtxMin->z) < (pObject->pos.z + g_vtxMaxObject.z))
				{// めり込んだらブロックの下の座標にする
					pPos->y = (pObject->pos.z + g_vtxMinObject.z + pvtxMin->z);
				}
			}
		}
		if ((pPos->z + pvtxMax->z) > pObject->pos.z - g_vtxMinObject.z && 
			(pPos->z - pvtxMin->z) < (pObject->pos.z + g_vtxMaxObject.z))
		{// プレイヤーがブロックのY幅にいるとき
			if ((pPosOld->x + pvtxMax->x) <= pObject->pos.x - g_vtxMinObject.x)
			{// プレイヤーがブロックの左側にいるとき
				if ((pPos->x + pvtxMax->x) > pObject->pos.x - g_vtxMinObject.x)
				{// めり込んだらブロックの左の座標にする
					pPos->x = pObject->pos.x - g_vtxMinObject.x - pvtxMax->x;
				}
			}
			else if ((pPosOld->x - pvtxMin->x) >= (pObject->pos.x + g_vtxMaxObject.x))
			{// プレイヤーがブロックの右側にいるとき
				if ((pPos->x - pvtxMin->x) < (pObject->pos.x + g_vtxMaxObject.x))
				{// めり込んだらブロックの右の座標にする
					pPos->x = pObject->pos.x - g_vtxMinObject.x + pvtxMin->x;
				}
			}
		}
	}
}