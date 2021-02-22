//-----------------------------------------------------------------
//
// 敵 (enemy.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "enemy.h"
#include "wall.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "score.h"
#include "model_set.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_ENEMY			(8)
#define PI_QUARTER			(D3DX_PI / 4.0f)

#define MOVE_ENEMY			(1.0f)							// 敵の移動量
#define RENGE				(300.0f)						// 敵がプレイヤーを見つける範囲

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
Enemy g_aEnemy[MAX_ENEMY];										// モデル情報
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_ENEMY] = {};			// テクスチャへのポインタ

//-----------------------------------------------------------------
// 敵の初期化処理
//-----------------------------------------------------------------
HRESULT InitEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// 情報の初期化
		g_aEnemy[nCnt].pMeshModel = NULL;
		g_aEnemy[nCnt].pBuffMatModel = NULL;
		g_aEnemy[nCnt].nNumMatModel = NULL;
		g_aEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rotDest = g_aEnemy[nCnt].rot;
		g_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].nIdx = -1;
		g_aEnemy[nCnt].bUse = false;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/rat.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEnemy[nCnt].pBuffMatModel,
			NULL,
			&g_aEnemy[nCnt].nNumMatModel,
			&g_aEnemy[nCnt].pMeshModel)))
		{
			return E_FAIL;
		}
		// 敵の頂点
		VecEnemy(nCnt);
	}

	// 敵の設置
	SetEnemy(D3DXVECTOR3(100.0f, 0.0f, 200.0f));
	SetEnemy(D3DXVECTOR3(600.0f, 0.0f, -200.0f));
	SetEnemy(D3DXVECTOR3(-200.0f, 0.0f, 600.0f));

	return S_OK;
}

//-----------------------------------------------------------------
// 敵の終了処理
//-----------------------------------------------------------------
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// メッシュ情報の開放
		if (g_aEnemy[nCnt].pBuffMatModel != NULL)
		{
			g_aEnemy[nCnt].pBuffMatModel->Release();
			g_aEnemy[nCnt].pBuffMatModel = NULL;
		}

		// マテリアル情報の開放
		if (g_aEnemy[nCnt].pMeshModel != NULL)
		{
			g_aEnemy[nCnt].pMeshModel->Release();
			g_aEnemy[nCnt].pMeshModel = NULL;
		}
	}
}

//-----------------------------------------------------------------
// 敵の更新処理
//-----------------------------------------------------------------
void UpdateEnemy(void)
{
	// 変数宣言
	int nCntTrue = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// 座標の保存
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			// 敵の移動処理
			MoveEnemy(nCntEnemy);

			// モデルの当たり判定
			CollisionModelSet(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move,&g_aEnemy[nCntEnemy].vtxMinModel, &g_aEnemy[nCntEnemy].vtxMaxModel,g_aEnemy[nCntEnemy].nIdx);
		}
	}
	if (nCntTrue == 0)
	{// コインの数が0枚の時フェードアウト
	 //SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------
// 敵の描画処理
//-----------------------------------------------------------------
void DrawEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// 変数の受け渡し
	pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMatModel; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEnemy[nCntMat]);

				// モデル(パーツ)の描画
				g_aEnemy[nCntEnemy].pMeshModel->DrawSubset(nCntMat);
			}
			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------
// 敵の移動処理
//-----------------------------------------------------------------
void MoveEnemy(int nCntEnemy)
{
	// 構造体のポインタ変数
	Player *pPlayer = GetPlayer();

	// 変数宣言
	D3DXVECTOR3 EnemyVec;		// プレイヤーの方向に向かせるために使用

	// 目的の回転角を制限
	if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{// -3.14fより小さくなったとき値を3.14fにする
		g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2.0f;
	}
	// 向きの更新
	g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y) * 0.08f;

	// 現在の回転角を制限
	if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{// -3.14fより小さくなったとき値に3.14fにする
		g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
	}

	// 敵がプレイヤーを見るようにする
	// 敵とプレイヤーの方向ベクトル
	EnemyVec = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

	// 角度を求める
	float fAngle = atan2f(EnemyVec.x, EnemyVec.z);

	// 自機と中心点の対角線の長さ
	float fLength = sqrtf((EnemyVec.x * EnemyVec.x) + (EnemyVec.z * EnemyVec.z));

	if (fLength < RENGE)
	{
		// プレイヤーを追尾する
		g_aEnemy[nCntEnemy].move.x = MOVE_ENEMY * sinf(fAngle);
		g_aEnemy[nCntEnemy].move.z = MOVE_ENEMY * cosf(fAngle);

		// 向きの更新
		g_aEnemy[nCntEnemy].rotDest.y = fAngle;

		// 位置の更新
		g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
		g_aEnemy[nCntEnemy].pos.z += g_aEnemy[nCntEnemy].move.z;
	}

	// 外積を用いた当たり判定に使うベクトルの計算
	CrossProductEnemy(nCntEnemy);

	// 影の移動
	//SetPositionShadow(g_aEnemy[nCntEnemy].nIdx, g_aEnemy[nCntEnemy].pos);
}

//-----------------------------------------------------------------
// 敵の設置
//-----------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			// 座標の設定
			g_aEnemy[nCntEnemy].pos = pos;

			// 敵のテクスチャ
			TexEnemy(nCntEnemy);

			// 使用状態をtrueにする
			g_aEnemy[nCntEnemy].bUse = true;

			// 影の配置
			//g_aEnemy[nCntEnemy].nIdx = SetShadow(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0.0f, g_aEnemy[nCntEnemy].pos.z), SHADOW_SIZE, SHADOW_SIZE);

			break;
		}
	}
}

//-----------------------------------------------------------------
// 敵のテクスチャ
//-----------------------------------------------------------------
void TexEnemy(int nCntCoin)
{
	// 変数宣言
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ

	// 変数の受け渡し
	pDevice = GetDevice();

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aEnemy[nCntCoin].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntCoin].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureEnemy[nCntMat]);
		}
	}
}

//-----------------------------------------------------------------
// 敵の頂点座標
//-----------------------------------------------------------------
void VecEnemy(int nCntCoin)
{
	// 変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

							// 頂点数の取得
	nNumVtx = g_aEnemy[nCntCoin].pMeshModel->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntCoin].pMeshModel->GetFVF());

	// 頂点バッファのロック
	g_aEnemy[nCntCoin].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点を比較してモデルの最小値最大値を抜き出す
		if (g_aEnemy[nCntCoin].vtxMinModel.x > vtx.x)
		{// X座標の最小値
			g_aEnemy[nCntCoin].vtxMinModel.x = vtx.x;
		}
		else if (g_aEnemy[nCntCoin].vtxMaxModel.x < vtx.x)
		{// X座標の最大値
			g_aEnemy[nCntCoin].vtxMaxModel.x = vtx.x;
		}

		if (g_aEnemy[nCntCoin].vtxMinModel.y > vtx.y)
		{// Y座標の最小値
			g_aEnemy[nCntCoin].vtxMinModel.y = vtx.y;
		}
		else if (g_aEnemy[nCntCoin].vtxMaxModel.y < vtx.y)
		{// Y座標の最大値
			g_aEnemy[nCntCoin].vtxMaxModel.y = vtx.y;
		}

		if (g_aEnemy[nCntCoin].vtxMinModel.z > vtx.z)
		{// Z座標の最小値
			g_aEnemy[nCntCoin].vtxMinModel.z = vtx.z;
		}
		else if (g_aEnemy[nCntCoin].vtxMaxModel.z < vtx.z)
		{// Z座標の最大値
			g_aEnemy[nCntCoin].vtxMaxModel.z = vtx.z;
		}
		// 頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;
	}
	// 頂点バッファのアンロック
	g_aEnemy[0].pMeshModel->UnlockVertexBuffer();
}

//-----------------------------------------------------------------
// 外積に使うベクトルの計算
//-----------------------------------------------------------------
void CrossProductEnemy(int nCntCoin)
{
	// 変数の初期化
	g_aEnemy[nCntCoin].aPos[0] = D3DXVECTOR3(g_aEnemy[nCntCoin].pos.x + g_aEnemy[nCntCoin].vtxMinModel.x, 0.0f, g_aEnemy[nCntCoin].pos.z + g_aEnemy[nCntCoin].vtxMinModel.z);
	g_aEnemy[nCntCoin].aPos[1] = D3DXVECTOR3(g_aEnemy[nCntCoin].pos.x + g_aEnemy[nCntCoin].vtxMinModel.x, 0.0f, g_aEnemy[nCntCoin].pos.z + g_aEnemy[nCntCoin].vtxMaxModel.z);
	g_aEnemy[nCntCoin].aPos[2] = D3DXVECTOR3(g_aEnemy[nCntCoin].pos.x + g_aEnemy[nCntCoin].vtxMaxModel.x, 0.0f, g_aEnemy[nCntCoin].pos.z + g_aEnemy[nCntCoin].vtxMaxModel.z);
	g_aEnemy[nCntCoin].aPos[3] = D3DXVECTOR3(g_aEnemy[nCntCoin].pos.x + g_aEnemy[nCntCoin].vtxMaxModel.x, 0.0f, g_aEnemy[nCntCoin].pos.z + g_aEnemy[nCntCoin].vtxMinModel.z);

	// 変数の初期化
	g_aEnemy[nCntCoin].aVecA[0] = g_aEnemy[nCntCoin].aPos[1] - g_aEnemy[nCntCoin].aPos[0];
	g_aEnemy[nCntCoin].aVecA[1] = g_aEnemy[nCntCoin].aPos[2] - g_aEnemy[nCntCoin].aPos[1];
	g_aEnemy[nCntCoin].aVecA[2] = g_aEnemy[nCntCoin].aPos[3] - g_aEnemy[nCntCoin].aPos[2];
	g_aEnemy[nCntCoin].aVecA[3] = g_aEnemy[nCntCoin].aPos[0] - g_aEnemy[nCntCoin].aPos[3];
}

//-----------------------------------------------------------------
// 敵の当たり判定
//-----------------------------------------------------------------
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// 構造体のポインタ変数
	Enemy *pEnemy = &g_aEnemy[0];

	// 変数宣言
	bool bCollision = false;		// 当たったかどうか

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// ベクトル計算
			pEnemy->aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pEnemy->aPos[0].x, 0.0f, pPos->z - pEnemy->aPos[0].z);
			pEnemy->aVecB[1] = D3DXVECTOR3(pPos->x - pEnemy->aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pEnemy->aPos[1].z);
			pEnemy->aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pEnemy->aPos[2].x, 0.0f, pPos->z - pEnemy->aPos[2].z);
			pEnemy->aVecB[3] = D3DXVECTOR3(pPos->x - pEnemy->aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pEnemy->aPos[3].z);

			// 外積を用いた当たり判定
			pEnemy->fVec[0] = (pEnemy->aVecA[0].z * pEnemy->aVecB[0].x) - (pEnemy->aVecA[0].x * pEnemy->aVecB[0].z);
			pEnemy->fVec[1] = (pEnemy->aVecA[1].z * pEnemy->aVecB[1].x) - (pEnemy->aVecA[1].x * pEnemy->aVecB[1].z);
			pEnemy->fVec[2] = (pEnemy->aVecA[2].z * pEnemy->aVecB[2].x) - (pEnemy->aVecA[2].x * pEnemy->aVecB[2].z);
			pEnemy->fVec[3] = (pEnemy->aVecA[3].z * pEnemy->aVecB[3].x) - (pEnemy->aVecA[3].x * pEnemy->aVecB[3].z);

			if (pEnemy->fVec[0] > 0.0f && pEnemy->fVec[1] > 0.0f &&
				pEnemy->fVec[2] > 0.0f && pEnemy->fVec[3] > 0.0f)
			{
				if ((pPosOld->y >= pEnemy->pos.y + pEnemy->vtxMaxModel.y) && (pPos->y < pEnemy->pos.y + pEnemy->vtxMaxModel.y))
				{// 敵を踏んだら消す
					pPos->y = pEnemy->pos.y + pEnemy->vtxMaxModel.y;
					pMove->y *= -0.5f;
					
					// 敵を消去する
					DeleteEnemy(nCntEnemy);
				}
				else if (pPos->y <= pEnemy->pos.y + pEnemy->vtxMaxModel.y&&
					pPos->y >= pEnemy->pos.y + pEnemy->vtxMinModel.y - pvtxMax->y)
				{
					if (pPos->x > pPosOld->x && (pPosOld->x < pEnemy->pos.x + pEnemy->vtxMinModel.x))
					{// プレイヤーが左から当たったとき
						pPos->x = pEnemy->pos.x + pEnemy->vtxMinModel.x - pvtxMax->x;
					}
					else if (pPos->x < pPosOld->x && (pPosOld->x > pEnemy->pos.x + pEnemy->vtxMaxModel.x))
					{// プレイヤーが右から当たったとき
						pPos->x = pEnemy->pos.x + pEnemy->vtxMaxModel.x - pvtxMin->x;
					}
					else if (pPos->z > pPosOld->z && pPosOld->z < pEnemy->pos.z + pEnemy->vtxMinModel.z)
					{// プレイヤーが手前から当たったとき
						pPos->z = pEnemy->pos.z + pEnemy->vtxMinModel.z - pvtxMax->z;
					}
					else if (pPos->z < pPosOld->z && pPosOld->z > pEnemy->pos.z + pEnemy->vtxMaxModel.z)
					{// プレイヤーが奥から当たったとき
						pPos->z = pEnemy->pos.z + pEnemy->vtxMaxModel.z - pvtxMin->z;
					}
					pMove->x = pEnemy->move.x * 10.0f;
					pMove->z = pEnemy->move.z * 10.0f;

					bCollision = true;
				}
			}
		}
	}

	return bCollision;
}

//-----------------------------------------------------------------
// 敵を消去する処理
//-----------------------------------------------------------------
void DeleteEnemy(int nCntEnemy)
{
	// 影を消す
	//DeleteShadow(g_aEnemy[nCntEnemy].nIdx);

	// 使用状態をfalseにする
	g_aEnemy[nCntEnemy].bUse = false;
}