//=============================================================================
//
// モデル (model.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// CSceneXのコンストラクタ
//=============================================================================
CSceneX::CSceneX(int nPriority) : CScene(nPriority)
{

}

//=============================================================================
// CSceneXのデストラクタ
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// シーンXの初期化処理
//=============================================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = CManager::GetRenderer()->GetDevice();
	m_vtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	m_vtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	// 情報の初期化
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = NULL;
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(m_pModelFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh)))
	{
		return E_FAIL;
	}

	// 変数宣言
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
		}
		else
		{
			m_pTexture[nCntMat] = NULL;
		}
	}

	// 変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点を比較してモデルの最小値最大値を抜き出す
		if (m_vtxMinModel.x > vtx.x)
		{// X座標の最小値
			m_vtxMinModel.x = vtx.x;
		}
		else if (m_vtxMaxModel.x < vtx.x)
		{// X座標の最大値
			m_vtxMaxModel.x = vtx.x;
		}

		if (m_vtxMinModel.y > vtx.y)
		{// Y座標の最小値
			m_vtxMinModel.y = vtx.y;
		}
		else if (m_vtxMaxModel.y < vtx.y)
		{// Y座標の最大値
			m_vtxMaxModel.y = vtx.y;
		}

		if (m_vtxMinModel.z > vtx.z)
		{// Z座標の最小値
			m_vtxMinModel.z = vtx.z;
		}
		else if (m_vtxMaxModel.z < vtx.z)
		{// Z座標の最大値
			m_vtxMaxModel.z = vtx.z;
		}
		// 頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;
	}
	// 頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// シーンXの終了処理
//=============================================================================
void CSceneX::Uninit(void)
{
	// メッシュ情報の開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// マテリアル情報の開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	// 破棄　
	Release();
}

//=============================================================================
// シーンXの更新処理
//=============================================================================
void CSceneX::Update(void)
{

}

//=============================================================================
// シーンXの描画処理
//=============================================================================
void CSceneX::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// 変数の受け渡し
	pDevice = CManager::GetRenderer()->GetDevice();

	// Zバッファを適用
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// 標準に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// シーンXのマテリアル設定
//=============================================================================
void CSceneX::SetMat(D3DXMATERIAL *pMat,int nCntMat)
{
	// 変数の受け渡し
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;			

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルの設定
	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// シーンXのマテリアル情報の取得
//=============================================================================
D3DXMATERIAL *CSceneX::GetMat(void)
{
	// 変数の受け渡し
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;	

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	return pMat;
}

//=============================================================================
// シーンXの回転値設定
//=============================================================================
void CSceneX::SetRotate(D3DXVECTOR3 rot)
{
	// 角度の制限
	// x軸
	if (rot.x >= D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x <= -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}

	// y軸
	if (rot.y >= D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y <= -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	// z軸
	if (rot.z >= D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z <= -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	// 角度の設定
	m_rot = rot;
}