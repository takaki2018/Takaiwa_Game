//-----------------------------------------------------------------
//
// モデル (model.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "model.h"
#include "wall.h"
#include "object.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_MODEL			(1)
#define POLYGON_SIZE		(5.0f)		// ポリゴンのサイズ
#define MOVE_MODEL			(0.8f)
#define PI_QUARTER			(D3DX_PI / 4.0f)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
TestModel g_model[MAX_MODEL];												// モデル情報
D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;						// 各頂点座標の最小値、最大値
LPDIRECT3DTEXTURE9 g_apTextureModel[8] = {};					// テクスチャへのポインタ

//-----------------------------------------------------------------
// モデルの初期化処理
//-----------------------------------------------------------------
HRESULT InitModel(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();
	g_vtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_vtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		// 情報の初期化
		g_model[nCnt].pMesh = NULL;
		g_model[nCnt].pBuffMat = NULL;
		g_model[nCnt].nNumMat = NULL;
		g_model[nCnt].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_model[nCnt].posOld = g_model[nCnt].pos;
		g_model[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[nCnt].rotDest = g_model[nCnt].rot;
		g_model[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[nCnt].nIdx = -1;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/test.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_model[nCnt].pBuffMat,
			NULL,
			&g_model[nCnt].nNumMat,
			&g_model[nCnt].pMesh)))
		{
			return E_FAIL;
		}
	}
	// 変数宣言
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_model[0].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_model[0].nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureModel[nCntMat]);
		}
	}

	// 変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数の取得
	nNumVtx = g_model[0].pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_model[0].pMesh->GetFVF());

	// 頂点バッファのロック
	g_model[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY,(void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点を比較してモデルの最小値最大値を抜き出す
		if (g_vtxMinModel.x > vtx.x)
		{// X座標の最小値
			g_vtxMinModel.x = vtx.x;
		}
		else if (g_vtxMaxModel.x < vtx.x)
		{// X座標の最大値
			g_vtxMaxModel.x = vtx.x;
		}

		if (g_vtxMinModel.y > vtx.y)
		{// Y座標の最小値
			g_vtxMinModel.y = vtx.y;
		}
		else if (g_vtxMaxModel.y < vtx.y)
		{// Y座標の最大値
			g_vtxMaxModel.y = vtx.y;
		}

		if (g_vtxMinModel.z > vtx.z)
		{// Z座標の最小値
			g_vtxMinModel.z = vtx.z;
		}
		else if (g_vtxMaxModel.z < vtx.z)
		{// Z座標の最大値
			g_vtxMaxModel.z = vtx.z;
		}
		// 頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;
	}
	// 頂点バッファのアンロック
	g_model[0].pMesh->UnlockVertexBuffer();

	// 影の配置
	g_model[0].nIdx = SetShadow(D3DXVECTOR3(g_model[0].pos.x,0.0f, g_model[0].pos.z), SHADOW_SIZE, SHADOW_SIZE);

	return S_OK;
}

//-----------------------------------------------------------------
// モデルの終了処理
//-----------------------------------------------------------------
void UninitModel(void)
{
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		// メッシュ情報の開放
		if (g_model[nCnt].pBuffMat != NULL)
		{
			g_model[nCnt].pBuffMat->Release();
			g_model[nCnt].pBuffMat = NULL;
		}

		// マテリアル情報の開放
		if (g_model[nCnt].pMesh != NULL)
		{
			g_model[nCnt].pMesh->Release();
			g_model[nCnt].pMesh = NULL;
		}
	}
}

//-----------------------------------------------------------------
// モデルの更新処理
//-----------------------------------------------------------------
void UpdateModel(void)
{
	// 情報の取得
	Camera camera = GetCamera();		// カメラ情報
	Wall *pWall = GetWall();			// 壁情報

	// モデルの位置を保存
	g_model[0].posOld = g_model[0].pos;

	// 目的の回転角を制限
	if (g_model[0].rotDest.y - g_model[0].rot.y < -D3DX_PI)
	{// -3.14fより小さくなったとき値を3.14fにする
		g_model[0].rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_model[0].rotDest.y - g_model[0].rot.y > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		g_model[0].rotDest.y -= D3DX_PI * 2.0f;
	}
	// 向きの更新
	g_model[0].rot.y += (g_model[0].rotDest.y - g_model[0].rot.y) * 0.08f;

	// 現在の回転角を制限
	if (g_model[0].rot.y < -D3DX_PI)
	{// -3.14fより小さくなったとき値に3.14fにする
		g_model[0].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_model[0].rot.y > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		g_model[0].rot.y -= D3DX_PI * 2.0f;
	}

#if 1
	if (GetKeyboardPress(KEYINFO_UP) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// モデル斜め右上移動
			g_model[0].move.z -= MOVE_MODEL * sinf(camera.rot.y - PI_QUARTER);
			g_model[0].move.x += MOVE_MODEL * cosf(camera.rot.y - PI_QUARTER);
			g_model[0].rotDest.y = camera.rot.y - PI_QUARTER * 3.0f;
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// モデル斜め右上移動
			g_model[0].move.z += MOVE_MODEL * sinf(camera.rot.y + PI_QUARTER);
			g_model[0].move.x -= MOVE_MODEL * cosf(camera.rot.y + PI_QUARTER);
			g_model[0].rotDest.y = camera.rot.y + PI_QUARTER * 3.0f;
		}
		else
		{// モデル上移動
			g_model[0].move.z += MOVE_MODEL * cosf(camera.rot.y);
			g_model[0].move.x += MOVE_MODEL * sinf(camera.rot.y);
			g_model[0].rotDest.y = camera.rot.y - D3DX_PI;
		}
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// モデル斜め右上移動
			g_model[0].move.z -= MOVE_MODEL * sinf(camera.rot.y + PI_QUARTER);
			g_model[0].move.x += MOVE_MODEL * cosf(camera.rot.y + PI_QUARTER);
			g_model[0].rotDest.y = camera.rot.y - PI_QUARTER;
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// モデル斜め右上移動
			g_model[0].move.z += MOVE_MODEL * sinf(camera.rot.y - PI_QUARTER);
			g_model[0].move.x -= MOVE_MODEL * cosf(camera.rot.y - PI_QUARTER);
			g_model[0].rotDest.y = camera.rot.y + PI_QUARTER;
		}
		else
		{// モデル下移動
			g_model[0].move.z -= MOVE_MODEL * cosf(camera.rot.y);
			g_model[0].move.x -= MOVE_MODEL * sinf(camera.rot.y);
			g_model[0].rotDest.y = camera.rot.y;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// モデル右移動
		g_model[0].move.z -= MOVE_MODEL * sinf(camera.rot.y);
		g_model[0].move.x += MOVE_MODEL * cosf(camera.rot.y);
		g_model[0].rotDest.y = camera.rot.y - D3DX_PI / 2.0f;
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{// モデル左移動
		g_model[0].move.z += MOVE_MODEL * sinf(camera.rot.y);
		g_model[0].move.x -= MOVE_MODEL * cosf(camera.rot.y);
		g_model[0].rotDest.y = camera.rot.y + D3DX_PI / 2.0f;
	}
	if (GetKeyboardPress(KEYINFO_MODEL_RIGHT_TURN) == true)
	{// 注視点の左回転
		g_model[0].rotDest.y += 0.1f;						// 回転量
	}
	if (GetKeyboardPress(KEYINFO_MODEL_LEFT_TURN) == true)
	{// 注視点の右回転
		g_model[0].rotDest.y -= 0.1f;						// 回転量
	}
#else
	// ゲームパッド情報の取得
	DIJOYSTATE2 Controller = GetControllerPless(PLAYER_1);

	// JOYPADのスティック入力
	if (Controller.lX != 0 || Controller.lY != 0)
	{// スティックの入力角度
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY * -1);

		g_model[0].rot.y = camera.rot.y + (D3DX_PI + fAngle);					// 向きの設定
		g_model[0].move.x += MOVE_MODEL * sinf(camera.rot.y + fAngle);			// X軸移動量の決定
		g_model[0].move.z += MOVE_MODEL * cosf(camera.rot.y + fAngle);			// Z軸移動量の決定
	}
#endif
	// 移動量の減衰(慣性)
	g_model[0].move.x += (0.0f - g_model[0].move.x) * 0.1f;
	g_model[0].move.z += (0.0f - g_model[0].move.z) * 0.1f;

	// 位置の更新
	g_model[0].pos.x += g_model[0].move.x;
	g_model[0].pos.z += g_model[0].move.z;

	// オブジェクトとの当たり判定
	//CollisionObject(&g_model[0].pos, &g_model[0].posOld, &g_vtxMinModel,&g_vtxMaxModel);

	// 影の配置
	SetPositionShadow(g_model[0].nIdx, D3DXVECTOR3(g_model[0].pos.x, 0.0f, g_model[0].pos.z));

	// リセット
	if (GetKeyboardTrigger(KEYINFO_OK) == true)
	{
		g_model[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_model[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[0].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//-----------------------------------------------------------------
// モデルの描画処理
//-----------------------------------------------------------------
void DrawModel(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// 変数の受け渡し
	pDevice = GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_model[0].mtxWorld);

	// スケールを反映
	//g_model[0].mtxWorld._11 = 0.8f;
	//g_model[0].mtxWorld._22 = 0.8f;
	//g_model[0].mtxWorld._33 = 0.8f;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_model[0].rot.y, g_model[0].rot.x, g_model[0].rot.z);
	D3DXMatrixMultiply(&g_model[0].mtxWorld, &g_model[0].mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_model[0].pos.x, g_model[0].pos.y, g_model[0].pos.z);
	D3DXMatrixMultiply(&g_model[0].mtxWorld, &g_model[0].mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_model[0].mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_model[0].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_model[0].nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

		// モデル(パーツ)の描画
		g_model[0].pMesh->DrawSubset(nCntMat);
	}
	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
#if 0
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_model[1].mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_model[0].rot.y, g_model[0].rot.x, g_model[0].rot.z);
	D3DXMatrixMultiply(&g_model[1].mtxWorld, &g_model[1].mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_model[0].pos.x, g_model[0].pos.y + 15.0f, g_model[0].pos.z);
	D3DXMatrixMultiply(&g_model[1].mtxWorld, &g_model[1].mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_model[1].mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_model[0].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_model[1].nNumMatModel; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// モデル(パーツ)の描画
		g_model[1].pMeshModel->DrawSubset(nCntMat);
	}
	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
#endif
}

//-----------------------------------------------------------------
// モデルの位置情報の取得
//-----------------------------------------------------------------
TestModel *GetModel(void)
{
	return &g_model[0];
}