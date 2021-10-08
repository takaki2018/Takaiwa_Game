//-----------------------------------------------------------------
//
// カメラ (camera.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define LENGTH					(50.0f)		// 奥行
#define MOVE_ROT_LookAtPoint	(0.05f)			// 注視点回転量
#define MOVE_ROT_Point			(0.05f)			// 視点回転量
#define MOVE_LookAtPoint		(0.5f)			// 注視点移動量
#define MOVE_Point				(3.0f)			// 視点移動量

//-----------------------------------------------------------------
// CCameraのコンストラクタ
//-----------------------------------------------------------------
CCamera::CCamera()
{

}

//-----------------------------------------------------------------
// CCameraのデストラクタ
//-----------------------------------------------------------------
CCamera::~CCamera()
{

}

//-----------------------------------------------------------------
// カメラの初期化処理
//-----------------------------------------------------------------
void CCamera::Init(void)
{
	// 変数の初期化
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -LENGTH);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = SCREEN_WIDTH;
	m_viewport.Height = SCREEN_HEIGHT;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
}

//-----------------------------------------------------------------
// カメラの終了処理
//-----------------------------------------------------------------
void CCamera::Uninit(void)
{

}

//-----------------------------------------------------------------
// カメラの更新処理
//-----------------------------------------------------------------
void CCamera::Update(void)
{
#ifdef _DEBUG
	// 注視点操作
	LookAtPointRotate();

	// 視点操作
	PointRotate();

	// 注視点移動
	LookAtPointMove();

	// 視点移動
	//PointMove();

	// デバック表示
	CDebugProc::Print("\n--- カメラ情報 ---\n");
	CDebugProc::Print("カメラの視点  :%f %f %f\n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("カメラの注視点:%f %f %f\n", m_posR.x, m_posR.y, m_posR.z);
#endif
}

//-----------------------------------------------------------------
// カメラの設置
//-----------------------------------------------------------------
void CCamera::SetCamera(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// 変数の受け渡し
	pDevice = CManager::GetRenderer()->GetDevice();

	// ビューポートの設定
	pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)m_viewport.Width / (float)m_viewport.Height,
		10.0f,
		3000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//-----------------------------------------------------------------
// カメラの注視点回転処理
//-----------------------------------------------------------------
void CCamera::LookAtPointRotate(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_LookAtPoint_LEFT) == true)
	{// 視点の左回転
		m_rot.y -= MOVE_ROT_LookAtPoint;						// 回転量

		if (m_rot.y < -D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			m_rot.y += D3DX_PI * 2.0f;
		}

		// 注視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;	
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;	
	}
	if (pKey->GetPress(KEY_LookAtPoint_RIGHT) == true)
	{// 視点の右回転
		m_rot.y += MOVE_ROT_LookAtPoint;						// 回転量

		if (m_rot.y > D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// 注視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;	
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;	
	}
}

//-----------------------------------------------------------------
// カメラの視点回転処理
//-----------------------------------------------------------------
void CCamera::PointRotate(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_Point_LEFT) == true)
	{// 視点の左回転
		m_rot.y -= MOVE_ROT_Point;						// 回転量

		if (m_rot.y < -D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			m_rot.y += D3DX_PI * 2.0f;
		}

		// 視点の更新
		m_posV.x = m_posR.x - sinf(m_rot.y) * LENGTH;	
		m_posV.z = m_posR.z - cosf(m_rot.y) * LENGTH;	
	}
	if (pKey->GetPress(KEY_Point_RIGHT) == true)
	{// 視点の右回転
		m_rot.y += MOVE_ROT_Point;						// 回転量

		if (m_rot.y > D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			m_rot.y -= D3DX_PI * 2.0f;
		}
		// 視点の更新
		m_posV.x = m_posR.x - sinf(m_rot.y) * LENGTH;	
		m_posV.z = m_posR.z - cosf(m_rot.y) * LENGTH;	
	}
}

//-----------------------------------------------------------------
// カメラの注視点移動処理
//-----------------------------------------------------------------
void CCamera::LookAtPointMove(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_MOVE_LookAtPoint_UP) == true)
	{
		// 注視点の上移動
		m_posR.y += MOVE_Point;
	}
	if (pKey->GetPress(KEY_MOVE_LookAtPoint_DOWN) == true)
	{
		// 注視点の下移動
		m_posR.y -= MOVE_Point;
	}
}

//-----------------------------------------------------------------
// カメラの視点移動処理
//-----------------------------------------------------------------
void CCamera::PointMove(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_MOVE_UP) == true)
	{
		// 視点の前移動
		m_posV.x += MOVE_Point * sinf(m_rot.y);	
		m_posV.z += MOVE_Point * cosf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_DOWN) == true)
	{
		// 視点の後ろ移動
		m_posV.x -= MOVE_Point * sinf(m_rot.y);
		m_posV.z -= MOVE_Point * cosf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_RIGHT) == true)
	{
		// 視点の右移動
		m_posV.x += MOVE_Point * cosf(m_rot.y);
		m_posV.z -= MOVE_Point * sinf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_LEFT) == true)
	{
		// 視点の左移動
		m_posV.x -= MOVE_Point * cosf(m_rot.y);
		m_posV.z += MOVE_Point * sinf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Point_UP) == true)
	{
		// 視点の上移動
		m_posV.y += MOVE_Point;
	}
	if (pKey->GetPress(KEY_MOVE_Point_DOWN) == true)
	{
		// 視点の下移動
		m_posV.y -= MOVE_Point;
	}
}