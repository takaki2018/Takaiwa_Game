//-----------------------------------------------------------------
//
// プレイヤー (player.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "player.h"
#include "motion.h"
#include "wall.h"
#include "coin.h"
#include "enemy.h"
#include "meshfield.h"
#include "wall.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "motion.h"
#include "model_set.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define POLYGON_SIZE		(5.0f)		// ポリゴンのサイズ
#define MOVE_MODEL			(2.0f)
#define PI_QUARTER			(D3DX_PI / 4.0f)

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
Player g_player;						// プレイヤー情報
int g_nCntMotion;

//-----------------------------------------------------------------
// プレイヤーの初期化処理
//-----------------------------------------------------------------
HRESULT InitPlayer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// プレイヤー情報の初期化
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.bJump = false;
	g_player.bFall = false;
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.MotionType = MOTIONTYPE_NEUTRAL;
	g_player.MotionTypeOld = g_player.MotionType;
	g_player.nStateCounter = 0;
	g_player.nCounterMotion = 0;
	g_player.nKey = 0;

	// プレイヤー情報の読み込み
	LoadPlayerdata();

	// プレイヤーのパーツ設定
	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(g_player.aModel[nCntModel].aFileName, 
			D3DXMESH_SYSTEMMEM, 
			pDevice,
			NULL,
			&g_player.aModel[nCntModel].pBuffMat,
			NULL, 
			&g_player.aModel[nCntModel].nNumMat,
			&g_player.aModel[nCntModel].pMesh)))
		{
			return E_FAIL;
		}
	}
		
	// 変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		// 頂点数の取得
		nNumVtx = g_player.aModel[nCntModel].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[nCntModel].pMesh->GetFVF());

		// 頂点バッファのロック
		g_player.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点を比較してモデルの最小値最大値を抜き出す
			if (g_player.minVecPlayer.x > vtx.x)
			{// X座標の最小値
				g_player.minVecPlayer.x = vtx.x;
			}
			else if (g_player.maxVecPlayer.x < vtx.x)
			{// X座標の最大値
				g_player.maxVecPlayer.x = vtx.x;
			}

			if (g_player.minVecPlayer.y > vtx.y)
			{// Y座標の最小値
				g_player.minVecPlayer.y = vtx.y;
			}
			else if (g_player.maxVecPlayer.y < vtx.y)
			{// Y座標の最大値
				g_player.maxVecPlayer.y = vtx.y;
			}

			if (g_player.minVecPlayer.z > vtx.z)
			{// Z座標の最小値
				g_player.minVecPlayer.z = vtx.z;
			}
			else if (g_player.maxVecPlayer.z < vtx.z)
			{// Z座標の最大値
				g_player.maxVecPlayer.z = vtx.z;
			}
			// 頂点フォーマットのサイズ文ポインタを進める
			pVtxBuff += sizeFVF;
		}
		// 頂点バッファのアンロック
		g_player.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}
	// 影の配置
	g_player.nIdx = SetShadow(D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z), 20.0f, 20.0f);

	return S_OK;
}

//-----------------------------------------------------------------
// プレイヤーの終了処理
//-----------------------------------------------------------------
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		// マテリアル情報の開放
		if (g_player.aModel[nCnt].pBuffMat != NULL)
		{
			g_player.aModel[nCnt].pBuffMat->Release();
			g_player.aModel[nCnt].pBuffMat = NULL;
		}

		// メッシュ情報の開放
		if (g_player.aModel[nCnt].pMesh != NULL)
		{
			g_player.aModel[nCnt].pMesh->Release();
			g_player.aModel[nCnt].pMesh = NULL;
		}
	}
}

//-----------------------------------------------------------------
// プレイヤーの更新処理
//-----------------------------------------------------------------
void UpdatePlayer(void)
{
	// プレイヤーモーション
	PlayerMotion();

	// プレイヤーの状態を判別
	StateManagerPlayer();

	// 移動量の減衰(慣性)
	g_player.move.x += (0.0f - g_player.move.x) * 0.2f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.2f;

	// 重力設定
	g_player.move.y -= 0.6f;

	// 前の座標を保存
	g_player.posOld = g_player.pos;

	// 位置の更新
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;

	// コインとの当たり判定
	CollisionCoin(&g_player.pos, &g_player.posOld, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// モデルとの当たり判定
	bool bCollisionModel = CollisionModelSet(&g_player.pos, &g_player.posOld, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	if (bCollisionModel == true)
	{
		// 移動量を0.0fにする
		g_player.move.y = 0.0f;

		if (g_player.state == PLAYERSTATE_FLOATING)
		{// ジャンプ中のときジャンプしていない状態にする
			g_player.state = PLAYERSTATE_NORMAL;
		}
	}
	// メッシュフィールドとの当たり判定
	bool bLand = CollisionMeshField(&g_player.pos, &g_player.posOld);

	if (bLand == true)
	{
		// 移動量を0.0fにする
		g_player.move.y = 0.0f;

		if (g_player.state == PLAYERSTATE_FLOATING)
		{// ジャンプ中のときジャンプしていない状態にする
			g_player.state = PLAYERSTATE_NORMAL;
		}
	}

	// 影の移動
	SetPositionShadow(g_player.nIdx, D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z));

	// 落ちたらフェード
	if (g_player.pos.y < -100.0f)
	{
		g_player.bFall = true;
	}
}

//-----------------------------------------------------------------
// プレイヤーの描画処理
//-----------------------------------------------------------------
void DrawPlayer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// 変数の受け渡し
	pDevice = GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		// 変数宣言
		D3DXMATRIX mtxRotModel, mtxTransModel;		// 計算用マトリックス
		D3DXMATRIX mtxParent;						// 親のマトリックス

		// 各パーツのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		// 各パーツの向き
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		// 各パーツの位置
		D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		// 各パーツの親のマトリックスを設定
		if (g_player.aModel[nCntModel].nIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxParent = g_player.mtxWorld;
		}
		// 算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld,
			&g_player.aModel[nCntModel].mtxWorld,
			&mtxParent);

		// 各パーツのマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// モデル(パーツ)の描画
			g_player.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
		}
	}
	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//-----------------------------------------------------------------
// プレイヤー状態の判別
//-----------------------------------------------------------------
void StateManagerPlayer(void)
{
	// プレイヤーの状態を調べる
	switch (g_player.state)
	{// 状態が通常のとき
	case PLAYERSTATE_NORMAL:
		PlayerStateNormal();
		break;
		// 状態が浮遊のとき
	case PLAYERSTATE_FLOATING:
		PlayerStateFloating();
		break;
		// 状態がダメージのとき
	case PLAYERSTATE_DAMAGE:
		PlayerStateDamage();
		break;
	}
}

//-----------------------------------------------------------------
// プレイヤー状態が通常の時
//-----------------------------------------------------------------
void PlayerStateNormal(void)
{
	// プレイヤーの移動処理
	MovePlayer();

	// 敵との当たり判定
	bool bCollisionEnemy = CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// 敵に当たったらダメージ状態にする
	if (bCollisionEnemy == true)
	{
		g_player.state = PLAYERSTATE_DAMAGE;
	}
}

//-----------------------------------------------------------------
// プレイヤー状態が浮遊の時
//-----------------------------------------------------------------
void PlayerStateFloating(void)
{
	// プレイヤーの移動処理
	MovePlayer();

	// 敵との当たり判定
	bool bCollisionEnemy = CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// 敵に当たったらダメージ状態にする
	if (bCollisionEnemy == true)
	{
		g_player.state = PLAYERSTATE_DAMAGE;
	}

	// モーションをジャンプモーションにする
	g_player.MotionType = MOTIONTYPE_JUMP;
}

//-----------------------------------------------------------------
// プレイヤーがダメージ状態のとき
//-----------------------------------------------------------------
void PlayerStateDamage(void)
{
	// 状態カウンタを進める
	g_player.nStateCounter++;

	// のけぞりモーションに変える
	g_player.MotionType = MOTIONTYPE_DAMAGE;

	// 状態カウンターが20になったとき処理
	if (g_player.nStateCounter >= 20)
	{
		// モーションを戻す
		g_player.MotionType = MOTIONTYPE_NEUTRAL;

		// 状態をノーマルに戻す
		g_player.state = PLAYERSTATE_NORMAL;

		// 状態カウンターを0に初期化
		g_player.nStateCounter = 0;
	}
}

//-----------------------------------------------------------------
// プレイヤーの移動処理
//-----------------------------------------------------------------
void MovePlayer(void)
{
	// 構造体のポインタ変数
	Camera pCamera = GetCamera();

	// 現在の回転角を制限
	if (g_player.rotDest.y - g_player.rot.y < -D3DX_PI)
	{// -3.14fより小さくなったとき値に3.14fにする
		g_player.rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDest.y - g_player.rot.y > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		g_player.rotDest.y -= D3DX_PI * 2.0f;
	}
	// 向きの更新
	g_player.rot.y += (g_player.rotDest.y - g_player.rot.y) * 0.15f;

	// 現在の回転角を制限
	if (g_player.rot.y < -D3DX_PI)
	{// -3.14fより小さくなったとき値に3.14fにする
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// キーボード
	MovePlayerKeyboard();

	// ゲームパッド
	MovePlayerGamepad();
}

//-----------------------------------------------------------------
// プレイヤーの移動処理(キーボード)
//-----------------------------------------------------------------
void MovePlayerKeyboard(void)
{
	// 情報の取得
	Camera camera = GetCamera();				// カメラ情報
#if 1
	if (GetKeyboardPress(KEYINFO_UP) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// モデル斜め右上移動
			g_player.move.z -= MOVE_MODEL * sinf(camera.rot.y - PI_QUARTER);
			g_player.move.x += MOVE_MODEL * cosf(camera.rot.y - PI_QUARTER);
			g_player.rotDest.y = camera.rot.y - PI_QUARTER * 3.0f;
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// モデル斜め右上移動
			g_player.move.z += MOVE_MODEL * sinf(camera.rot.y + PI_QUARTER);
			g_player.move.x -= MOVE_MODEL * cosf(camera.rot.y + PI_QUARTER);
			g_player.rotDest.y = camera.rot.y + PI_QUARTER * 3.0f;
		}
		else
		{// モデル上移動
			g_player.move.z += MOVE_MODEL * cosf(camera.rot.y);
			g_player.move.x += MOVE_MODEL * sinf(camera.rot.y);
			g_player.rotDest.y = camera.rot.y - D3DX_PI;
		}
		// 移動しているとき移動モーションに設定
		g_player.MotionType = MOTIONTYPE_MOVE;
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// モデル斜め右上移動
			g_player.move.z -= MOVE_MODEL * sinf(camera.rot.y + PI_QUARTER);
			g_player.move.x += MOVE_MODEL * cosf(camera.rot.y + PI_QUARTER);
			g_player.rotDest.y = camera.rot.y - PI_QUARTER;
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// モデル斜め右上移動
			g_player.move.z += MOVE_MODEL * sinf(camera.rot.y - PI_QUARTER);
			g_player.move.x -= MOVE_MODEL * cosf(camera.rot.y - PI_QUARTER);
			g_player.rotDest.y = camera.rot.y + PI_QUARTER;
		}
		else
		{// モデル下移動
			g_player.move.z -= MOVE_MODEL * cosf(camera.rot.y);
			g_player.move.x -= MOVE_MODEL * sinf(camera.rot.y);
			g_player.rotDest.y = camera.rot.y;
		}
		// 移動しているとき移動モーションに設定
		g_player.MotionType = MOTIONTYPE_MOVE;
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// モデル右移動
		g_player.move.z -= MOVE_MODEL * sinf(camera.rot.y);
		g_player.move.x += MOVE_MODEL * cosf(camera.rot.y);
		g_player.rotDest.y = camera.rot.y - D3DX_PI / 2.0f;

		// 移動しているとき移動モーションに設定
		g_player.MotionType = MOTIONTYPE_MOVE;
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{// モデル左移動
		g_player.move.z += MOVE_MODEL * sinf(camera.rot.y);
		g_player.move.x -= MOVE_MODEL * cosf(camera.rot.y);
		g_player.rotDest.y = camera.rot.y + D3DX_PI / 2.0f;

		// 移動しているとき移動モーションに設定
		g_player.MotionType = MOTIONTYPE_MOVE;
	}
	if (GetKeyboardPress(KEYINFO_MODEL_RIGHT_TURN) == true)
	{// 注視点の左回転
		g_player.rotDest.y += 0.1f;						// 回転量
	}
	if (GetKeyboardPress(KEYINFO_MODEL_LEFT_TURN) == true)
	{// 注視点の右回転
		g_player.rotDest.y -= 0.1f;						// 回転量
	}
	if(GetKeyboardTrigger(KEYINFO_SHOOT) == true &&
		g_player.state != PLAYERSTATE_FLOATING)
	{
		g_player.move.y += 14.0f;
		g_player.state = PLAYERSTATE_FLOATING;
	}
	if (GetKeyboardPress(KEYINFO_UP) == false && GetKeyboardPress(KEYINFO_DOWN) == false &&
		GetKeyboardPress(KEYINFO_LEFT) == false && GetKeyboardPress(KEYINFO_RIGHT) == false &&
		g_player.bJump == false && g_player.MotionType != MOTIONTYPE_LAND)
	{
		// 何もしないときニュートラルモーションに設定
		g_player.MotionType = MOTIONTYPE_NEUTRAL;
	}
#endif
}

//-----------------------------------------------------------------
// プレイヤーの移動処理(ゲームパッド)
//-----------------------------------------------------------------
void MovePlayerGamepad(void)
{
	// 情報の取得
	Camera camera = GetCamera();				// カメラ情報

	// ゲームパッド情報の取得
	DIJOYSTATE2 Controller = GetControllerPless(PLAYER_1);

	// JOYPADのスティック入力
	// 移動処理
	if (Controller.lX != 0 || Controller.lY != 0)
	{// スティックの入力角度
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY);

		g_player.rotDest.y = camera.rot.y - fAngle;								// 向きの設定
		g_player.move.x -= MOVE_MODEL * sinf(g_player.rotDest.y);			// X軸移動量の決定
		g_player.move.z -= MOVE_MODEL * cosf(g_player.rotDest.y);			// Z軸移動量の決定

		// 現在の回転角を制限
		if (g_player.rotDest.y > D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			g_player.rotDest.y -= D3DX_PI * 2.0f;
		}
		else if (g_player.rotDest.y < -D3DX_PI)
		{// -3.14fより小さくなったとき値に3.14fにする
			g_player.rotDest.y += D3DX_PI * 2.0f;
		}
		// モーションを移動モーションにする
		g_player.MotionType = MOTIONTYPE_MOVE;
	}
	// 注視点回転
	if (GetJoypadPress(PLAYER_1, JPINFO_RIGHT_TURN) == true)
	{// 注視点の左回転
		g_player.rotDest.y += 0.1f;						// 回転量
	}
	if (GetJoypadPress(PLAYER_1, JPINFO_LEFT_TURN) == true)
	{// 注視点の右回転
		g_player.rotDest.y -= 0.1f;						// 回転量
	}
	// ジャンプ処理
	if (GetJoypadTrigger(PLAYER_1, JPINFO_JUMP) == true &&
		g_player.state != PLAYERSTATE_FLOATING)
	{
		g_player.move.y += 14.0f;
		g_player.state = PLAYERSTATE_FLOATING;
	}
}

//-----------------------------------------------------------------
// プレイヤー情報の取得
//-----------------------------------------------------------------
Player *GetPlayer(void)
{
	return &g_player;
}

//-----------------------------------------------------------------
// プレイヤ-モーションの設定
//-----------------------------------------------------------------
void PlayerMotion(void)
{
	// 前のモーションと現在のモーションを比較して異なるとき処理
	if (g_player.MotionTypeOld != g_player.MotionType)
	{
		// モーションカウンタの初期化
		g_player.nCounterMotion = 0;

		// 現在のキーを初期化
		g_player.nKey = 0;

		// 位置の補正
		g_player.aModel[0].pos.x = g_player.posParent.x;
		g_player.aModel[0].pos.y = g_player.posParent.y;
		g_player.aModel[0].pos.z = g_player.posParent.z;
	}
	// ループするかどうか
	g_player.bLoopMotion = g_player.aMotionInfo[g_player.MotionType].bLoop;

	// キー数の確定
	g_player.nNumKey = g_player.aMotionInfo[g_player.MotionType].nNumKey;

	// モーションの記憶
	g_player.MotionTypeOld = g_player.MotionType;

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		// プレイヤーのモーションの設定をする
		if (g_player.nKey == (g_player.nNumKey - 1) && g_player.bLoopMotion == true)
		{
			//g_player.aModel[nCntModel].pos.x += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fPosX - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosX) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			//g_player.aModel[nCntModel].pos.y += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fPosY - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosY) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			//g_player.aModel[nCntModel].pos.z += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fPosZ - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosZ) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			g_player.aModel[nCntModel].rot.x = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fRotX - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.y = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fRotY - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.z = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[0].aKey[nCntModel].fRotZ - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
		}
		else
		{
			//g_player.aModel[nCntModel].pos.x += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosX - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosX) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			//g_player.aModel[nCntModel].pos.y += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosY - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosY) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			//g_player.aModel[nCntModel].pos.z += (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosZ - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosZ) / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame;
			g_player.aModel[nCntModel].rot.x = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotX - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.y = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotY - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.z = g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ + (g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotZ - g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame);
		
			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == false)
			{
				break;
			}
		}
		if (g_player.nCounterMotion == g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nKey].nFrame)
		{
			g_player.nCounterMotion = 0;

			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
			{
				g_player.nKey = 0;
			}
			else
			{
				g_player.nKey++;
			}
		}
	}
	if (g_player.nCounterMotion != g_player.aMotionInfo[g_player.MotionType].aKeyInfo[g_player.nNumKey - 1].nFrame)
	{
		// モーションカウンタの更新
		g_player.nCounterMotion++;
	}
}