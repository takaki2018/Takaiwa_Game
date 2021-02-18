//-----------------------------------------------------------------
//
// カメラ (camera.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "camera.h"
#include "model.h"
#include "player.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MOVE_CAMERA			(3.0f)
#define LENGTH				(300.0f)
#define ROT					(0.03f)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
Camera g_camera[CAMERATYPE_MAX];			// カメラ情報

//-----------------------------------------------------------------
// カメラの初期化処理
//-----------------------------------------------------------------
void InitCamera(void)
{
	// 変数の初期化
	// プレイヤー見下ろしカメラ
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest = g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest = g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].moveV = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.X = 900;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.Y = 400;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.Width = 300;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.Height = 300;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.MinZ = 0.0f;
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.MaxZ = 1.0f;

	// プレイヤー追従カメラ
	g_camera[CAMERATYPE_PLAYER].posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].posVDest = g_camera[CAMERATYPE_PLAYER].posV;
	g_camera[CAMERATYPE_PLAYER].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].posRDest = g_camera[CAMERATYPE_PLAYER].posR;
	g_camera[CAMERATYPE_PLAYER].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].moveV = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera[CAMERATYPE_PLAYER].viewport.X = 0;
	g_camera[CAMERATYPE_PLAYER].viewport.Y = 0;
	g_camera[CAMERATYPE_PLAYER].viewport.Width = SCREEN_WIDTH;
	g_camera[CAMERATYPE_PLAYER].viewport.Height = SCREEN_HEIGHT;
	g_camera[CAMERATYPE_PLAYER].viewport.MinZ = 0.0f;
	g_camera[CAMERATYPE_PLAYER].viewport.MaxZ = 1.0f;
}

//-----------------------------------------------------------------
// カメラの終了処理
//-----------------------------------------------------------------
void UninitCamera(void)
{

}

//-----------------------------------------------------------------
// カメラの更新処理
//-----------------------------------------------------------------
void UpdateCamera(void)
{
	// モデル情報の取得
	TestModel *pModel = GetModel();

	// 追従
	KeyboradFollow();

	// 見下ろしカメラ
	LookDownPlayer();
}

//-----------------------------------------------------------------
// カメラの設置
//-----------------------------------------------------------------
void SetCamera(int nIdx)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// 変数の受け渡し
	pDevice = GetDevice();

	// ビューポートの設定
	pDevice->SetViewport(&g_camera[nIdx].viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera[nIdx].mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera[nIdx].mtxProjection,
		D3DXToRadian(45.0f),
		(float)g_camera[nIdx].viewport.Width / (float)g_camera[nIdx].viewport.Height,
		10.0f,
		3000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nIdx].mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera[nIdx].mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera[nIdx].mtxView,
		&g_camera[nIdx].posV,
		&g_camera[nIdx].posR,
		&g_camera[nIdx].vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nIdx].mtxView);
}

//-----------------------------------------------------------------
// カメラのキーボード操作(追従)
//-----------------------------------------------------------------
void KeyboradFollow(void)
{
	// モデル情報の取得
	TestModel *pModel = GetModel();

	// プレイヤー情報の取得
	Player *pPlayer = GetPlayer();

	if (GetKeyboardPress(KEYINFO_ROT_LEFT) == true)
	{// 視点の左回転
		g_camera[CAMERATYPE_PLAYER].rot.y -= ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].rot.y < -D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			g_camera[CAMERATYPE_PLAYER].rot.y += D3DX_PI * 2.0f;
		}
	}
	if (GetKeyboardPress(KEYINFO_ROT_RIGHT) == true)
	{// 視点の右回転
		g_camera[CAMERATYPE_PLAYER].rot.y += ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].rot.y > D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			g_camera[CAMERATYPE_PLAYER].rot.y -= D3DX_PI * 2.0f;
		}
	}
#if _DEBUG
	if (GetKeyboardPress(KEYINFO_ROT_UP) == true)
	{// 視点の上移動
		g_camera[CAMERATYPE_PLAYER].moveV.y += 2.0f;						// 回転量
	}
	if (GetKeyboardPress(KEYINFO_ROT_DOWN) == true)
	{// 視点の下移動
		g_camera[CAMERATYPE_PLAYER].moveV.y -= 2.0f;						// 回転量
	}
	if (GetKeyboardPress(KEYINFO_ROT_CAMERA_UP) == true)
	{// 注視点の上移動
		g_camera[CAMERATYPE_PLAYER].moveR.y += 2.0f;						// 回転量
	}
	if (GetKeyboardPress(KEYINFO_ROT_CAMERA_DOWN) == true)
	{// 注視点の下移動
		g_camera[CAMERATYPE_PLAYER].moveR.y -= 2.0f;						// 回転量
	}
#endif
	// ゲームパッド情報の取得
	bool *pRstckPress = GetRStickPress(PLAYER_1); 

	if (pRstckPress[RSTICKPRESS_RIGHT] == true)
	{// 視点の右回転
		g_camera[CAMERATYPE_PLAYER].rot.y -= ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].rot.y < -D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			g_camera[CAMERATYPE_PLAYER].rot.y += D3DX_PI * 2.0f;
		}
	}
	if (pRstckPress[RSTICKPRESS_LEFT] == true)
	{// 視点の左回転
		g_camera[CAMERATYPE_PLAYER].rot.y += ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].rot.y > D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			g_camera[CAMERATYPE_PLAYER].rot.y -= D3DX_PI * 2.0f;
		}
	}
	if (pRstckPress[RSTICKPRESS_UP] == true)
	{// 視点の下移動
		g_camera[CAMERATYPE_PLAYER].moveV.y -= 5.0f;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].moveV.y <= 50.0f)
		{// 視点の位置が50.0fより小さくなったとき処理
			g_camera[CAMERATYPE_PLAYER].moveV.y = 50.0f;
		}
	}
	if (pRstckPress[RSTICKPRESS_DOWN] == true)
	{// 視点の上移動
		g_camera[CAMERATYPE_PLAYER].moveV.y += 5.0f;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].moveV.y >= 500.0f)
		{// 視点の位置が500.0fより大きくなったとき処理
			g_camera[CAMERATYPE_PLAYER].moveV.y = 500.0f;
		}
	}

	// 目的の注視点
	g_camera[CAMERATYPE_PLAYER].posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 10.0f;		// 目的の注視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 10.0f;		// 目的の注視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posRDest.y = pPlayer->pos.y + g_camera[CAMERATYPE_PLAYER].moveR.y;

	// 目的の視点
	g_camera[CAMERATYPE_PLAYER].posVDest.x = pPlayer->pos.x - sinf(g_camera[CAMERATYPE_PLAYER].rot.y) * LENGTH;	// 目的の視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posVDest.z = pPlayer->pos.z - cosf(g_camera[CAMERATYPE_PLAYER].rot.y) * LENGTH;	// 目的の視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posVDest.y = pPlayer->pos.y + g_camera[CAMERATYPE_PLAYER].moveV.y;

	// 注視点の更新
	g_camera[CAMERATYPE_PLAYER].posR.x += (g_camera[CAMERATYPE_PLAYER].posRDest.x - g_camera[CAMERATYPE_PLAYER].posR.x) * 0.4f;	// 注視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posR.z += (g_camera[CAMERATYPE_PLAYER].posRDest.z - g_camera[CAMERATYPE_PLAYER].posR.z) * 0.4f;	// 注視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posR.y += (g_camera[CAMERATYPE_PLAYER].posRDest.y - g_camera[CAMERATYPE_PLAYER].posR.y) * 0.4f;	// 注視点のY軸座標の決定

	// 視点の更新
	g_camera[CAMERATYPE_PLAYER].posV.x += (g_camera[CAMERATYPE_PLAYER].posVDest.x - g_camera[CAMERATYPE_PLAYER].posV.x) * 1.0f;	// 視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posV.z += (g_camera[CAMERATYPE_PLAYER].posVDest.z - g_camera[CAMERATYPE_PLAYER].posV.z) * 1.0f;	// 視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posV.y += (g_camera[CAMERATYPE_PLAYER].posVDest.y - g_camera[CAMERATYPE_PLAYER].posV.y) * 1.0f;
}

//-----------------------------------------------------------------
// プレイヤーを見下ろすカメラ
//-----------------------------------------------------------------
void LookDownPlayer(void)
{
	// モデル情報の取得
	TestModel *pModel = GetModel();

	// プレイヤー情報の取得
	Player *pPlayer = GetPlayer();

	if (GetKeyboardPress(KEYINFO_ROT_LEFT) == true)
	{// 視点の左回転
		g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y -= ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y < -D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y += D3DX_PI * 2.0f;
		}
	}
	if (GetKeyboardPress(KEYINFO_ROT_RIGHT) == true)
	{// 視点の右回転
		g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y += ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y > D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y -= D3DX_PI * 2.0f;
		}
	}

	// ゲームパッド情報の取得
	bool *pRstckPress = GetRStickPress(PLAYER_1);

	if (pRstckPress[RSTICKPRESS_RIGHT] == true)
	{// 視点の右回転
		g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y -= ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y < -D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y += D3DX_PI * 2.0f;
		}
	}
	if (pRstckPress[RSTICKPRESS_LEFT] == true)
	{// 視点の左回転
		g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y += ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y > D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y -= D3DX_PI * 2.0f;
		}
	}

	// 目的の注視点
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.x = pPlayer->pos.x;		// 目的の注視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.z = pPlayer->pos.z;		// 目的の注視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.y = g_camera[CAMERATYPE_PLAYER_LOOKDOWN].moveR.y;

	// 目的の視点
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.x = pPlayer->pos.x - sinf(g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y);	// 目的の視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.z = pPlayer->pos.z - cosf(g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y);	// 目的の視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.y = g_camera[CAMERATYPE_PLAYER_LOOKDOWN].moveV.y;

	// 注視点の更新
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.x += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.x - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.x) * 1.0f;	// 注視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.z += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.z - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.z) * 1.0f;	// 注視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.y += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.y - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.y) * 1.0f;	// 注視点のY軸座標の決定

	// 視点の更新
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.x += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.x - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.x) * 1.0f;	// 視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.z += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.z - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.z) * 1.0f;	// 視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.y += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.y - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.y) * 1.0f;
}

//-----------------------------------------------------------------
// カメラ情報の取得
//-----------------------------------------------------------------
Camera GetCamera(void)
{
	return g_camera[CAMERATYPE_PLAYER];
}