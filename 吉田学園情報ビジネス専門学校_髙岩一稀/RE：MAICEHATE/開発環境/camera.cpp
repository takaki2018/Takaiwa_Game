//==============================================================================================================
//
// カメラ (camera.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "camera.h"
#include "player.h"
#include "input.h"
#include "gamepad.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MOVE_CAMERA			(3.0f)
#define LENGTH				(350.0f)
#define ROT					(0.03f)

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
Camera g_camera[CAMERATYPE_MAX];			// カメラ情報

//--------------------------------------------------------------------------------------------------------------
// カメラの初期化処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
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
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].viewport.X = 950;
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

//--------------------------------------------------------------------------------------------------------------
// カメラの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitCamera(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// カメラの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdateCamera(void)
{
	// 追従
	KeyboradFollow();

	// 見下ろしカメラ
	LookDownPlayer();
}

//--------------------------------------------------------------------------------------------------------------
// カメラの設置
// 引数		：nIdx	- 何番目のカメラか
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------------
// カメラのキーボード操作(追従)
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void KeyboradFollow(void)
{
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
	else if (GetKeyboardPress(KEYINFO_ROT_RIGHT) == true)
	{// 視点の右回転
		g_camera[CAMERATYPE_PLAYER].rot.y += ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].rot.y > D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			g_camera[CAMERATYPE_PLAYER].rot.y -= D3DX_PI * 2.0f;
		}
	}
	if (GetKeyboardPress(KEYINFO_POS_DOWN) == true)
	{// 視点の下移動
		g_camera[CAMERATYPE_PLAYER].moveV.y -= 5.0f;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].moveV.y <= 50.0f)
		{// 視点の位置が50.0fより小さくなったとき処理
			g_camera[CAMERATYPE_PLAYER].moveV.y = 50.0f;
		}
	}
	else if (GetKeyboardPress(KEYINFO_POS_UP) == true)
	{// 視点の上移動
		g_camera[CAMERATYPE_PLAYER].moveV.y += 5.0f;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].moveV.y >= 300.0f)
		{// 視点の位置が500.0fより大きくなったとき処理
			g_camera[CAMERATYPE_PLAYER].moveV.y = 300.0f;
		}
	}

	// ゲームパッド情報の取得
	bool *pRstckPress = GetRStickPress(PLAYER_1); 

	if (pRstckPress[RSTICKPRESS_LEFT] == true)
	{// 視点の右回転
		g_camera[CAMERATYPE_PLAYER].rot.y -= ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].rot.y < -D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			g_camera[CAMERATYPE_PLAYER].rot.y += D3DX_PI * 2.0f;
		}
	}
	else if (pRstckPress[RSTICKPRESS_RIGHT] == true)
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
	else if (pRstckPress[RSTICKPRESS_DOWN] == true)
	{// 視点の上移動
		g_camera[CAMERATYPE_PLAYER].moveV.y += 5.0f;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER].moveV.y >= 300.0f)
		{// 視点の位置が500.0fより大きくなったとき処理
			g_camera[CAMERATYPE_PLAYER].moveV.y = 300.0f;
		}
	}

	// 目的の注視点
	g_camera[CAMERATYPE_PLAYER].posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 10.0f + g_camera[CAMERATYPE_PLAYER].moveR.x;		// 目的の注視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 10.0f + g_camera[CAMERATYPE_PLAYER].moveR.y;		// 目的の注視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posRDest.y = pPlayer->pos.y + g_camera[CAMERATYPE_PLAYER].moveR.y;

	// 目的の視点
	g_camera[CAMERATYPE_PLAYER].posVDest.x = pPlayer->pos.x - sinf(g_camera[CAMERATYPE_PLAYER].rot.y) * LENGTH + g_camera[CAMERATYPE_PLAYER].moveV.x;	// 目的の視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER].posVDest.z = pPlayer->pos.z - cosf(g_camera[CAMERATYPE_PLAYER].rot.y) * LENGTH + g_camera[CAMERATYPE_PLAYER].moveV.z;	// 目的の視点のZ軸座標の決定
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

//--------------------------------------------------------------------------------------------------------------
// プレイヤーを見下ろすカメラ
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void LookDownPlayer(void)
{
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

	if (pRstckPress[RSTICKPRESS_LEFT] == true)
	{// 視点の右回転
		g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y -= ROT;						// 回転量

		if (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y < -D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			g_camera[CAMERATYPE_PLAYER_LOOKDOWN].rot.y += D3DX_PI * 2.0f;
		}
	}
	if (pRstckPress[RSTICKPRESS_RIGHT] == true)
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
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.y = pPlayer->pos.y + g_camera[CAMERATYPE_PLAYER_LOOKDOWN].moveV.y;

	// 注視点の更新
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.x += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.x - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.x) * 1.0f;	// 注視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.z += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.z - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.z) * 1.0f;	// 注視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.y += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posRDest.y - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posR.y) * 1.0f;	// 注視点のY軸座標の決定

	// 視点の更新
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.x += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.x - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.x) * 1.0f;	// 視点のX軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.z += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.z - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.z) * 1.0f;	// 視点のZ軸座標の決定
	g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.y += (g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posVDest.y - g_camera[CAMERATYPE_PLAYER_LOOKDOWN].posV.y) * 1.0f;
}

//--------------------------------------------------------------------------------------------------------------
// ゲーム画面のカメラ初期化
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void InitGameCamera(void)
{
	InitCamera();
}


//--------------------------------------------------------------------------------------------------------------
// プレイヤー追従カメラ情報の取得
// 引数		：void		- 何もなし
// 返り値	：Camera	- 追従カメラ情報を渡す
//--------------------------------------------------------------------------------------------------------------
Camera GetPlayerCamera(void)
{
	return g_camera[CAMERATYPE_PLAYER];
}

//--------------------------------------------------------------------------------------------------------------
// 見下ろしカメラ情報の取得
// 引数		：void		- 何もなし
// 返り値	：Camera	- 見下ろしカメラ情報を渡す
//--------------------------------------------------------------------------------------------------------------
Camera GetLookDownCamera(void)
{
	return g_camera[CAMERATYPE_PLAYER_LOOKDOWN];
}