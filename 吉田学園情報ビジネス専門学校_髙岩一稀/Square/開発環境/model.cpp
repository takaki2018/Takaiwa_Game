//=============================================================================
//
// モデル処理 [model.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "model.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "debugproc.h"

//=============================================================================
// CModelのコンストラクタ
//=============================================================================
CModel::CModel(int nPriority) : CSceneX(nPriority)
{
	// メンバ変数の初期化
	m_moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCnt = 0;
	m_bAppear = false;
}

//=============================================================================
// CModelのデストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, char *pModelFileName)
{
	// ポインタ変数の生成
	CModel *pModel;
	pModel = new CModel(CScene::PRIORITY_MODEL);

	// NULLチェック
	if (pModel != NULL)
	{
		// モデルファイルネームの設定
		pModel->BindXFileName(pModelFileName);

		// 初期化処理
		pModel->Init(pos, size);

		// オブジェクトタイプの設定
		pModel->SetObjType(OBJTYPE_MODEL);
	}

	return pModel;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CSceneXの初期化処理
	CSceneX::Init(pos, size);

	// メンバ変数の初期化
	m_moveRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nCnt = 0;
	m_bAppear = false;

	// マテリアル情報の取得
	D3DXMATERIAL *pMat = GetMat();
	int nNumMat = GetNumMat();

	for (int nCnt = 0; nCnt < nNumMat; nCnt++)
	{
		// α値の設定
		pMat[nCnt].MatD3D.Diffuse.a = 0.0f;

		// マテリアルの設定
		SetMat(pMat,nCnt);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	// CSceneXの終了処理
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{
	// CSceneXの更新処理
	CSceneX::Update();

	// 角度情報の取得
	D3DXVECTOR3 rot = GetRotate();

	// 勝手に回る
	rot.x += m_moveRot.x + 0.01f;
	rot.y += m_moveRot.y + 0.01f;
	rot.z -= m_moveRot.z + 0.005f;

	// 慣性
	m_moveRot.x += (0.0f - m_moveRot.x) * 0.03f;
	m_moveRot.y += (0.0f - m_moveRot.y) * 0.03f;
	m_moveRot.z += (0.0f - m_moveRot.z) * 0.03f;

	// 角度の設定
	SetRotate(rot);

	if (m_bAppear == false)
	{
		// マテリアル情報の取得
		D3DXMATERIAL *pMat = GetMat();
		int nNumMat = GetNumMat();

		for (int nCnt = 0; nCnt < nNumMat; nCnt++)
		{
			// α値の設定
			pMat[nCnt].MatD3D.Diffuse.a += 0.0005f;

			// マテリアルの設定
			SetMat(pMat, nCnt);

			// α値が1.0fを超えたら見えたことを伝える
			if (pMat[nCnt].MatD3D.Diffuse.a >= 1.0f)
			{
				m_bAppear = true;
			}
		}
	}

#ifdef _DEBUG
	CDebugProc::Print("\n--- モデルの角度 ---\n");
	CDebugProc::Print("X軸 : %f\n",rot.x);
	CDebugProc::Print("Y軸 : %f\n", rot.y);
	CDebugProc::Print("Z軸 : %f\n", rot.z);
	CDebugProc::Print("\n--- モデルの角度移動量 ---\n");
	CDebugProc::Print("X軸 : %f\n", m_moveRot.x);
	CDebugProc::Print("Y軸 : %f\n", m_moveRot.y);
	CDebugProc::Print("Z軸 : %f\n", m_moveRot.z);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	// CSceneXの描画処理
	CSceneX::Draw();
}

//=============================================================================
// 角度移動量加算処理
//=============================================================================
void CModel::AddMoveRotate(D3DXVECTOR3 rot)
{
	// 角度の加算
	m_moveRot += rot;

	// 移動量の制限
	// X軸
	if (m_moveRot.x >= 0.2f)
	{
		m_moveRot.x = 0.2f;
	}

	// Y軸
	if (m_moveRot.y >= 0.2f)
	{
		m_moveRot.y = 0.2f;
	}

	// Z軸
	if (m_moveRot.z >= 0.2f)
	{
		m_moveRot.z = 0.2f;
	}
}