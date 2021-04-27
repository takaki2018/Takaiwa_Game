//==============================================================================================================
//
// プレイヤー (player.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "player.h"
#include "motion.h"
#include "coin.h"
#include "enemy.h"
#include "meshfield.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "motion.h"
#include "model_set.h"
#include "count_down.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define MOVE_MODEL			(2.0f)
#define JUMP_PLAYER			(11.0f)
#define PI_QUARTER			(D3DX_PI / 4.0f)
#define GRAVITY				(0.5f)		// 重力

#define LENGTH				(8.0f)

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
Player g_player;						// プレイヤー情報
int g_nCntMotion;

//--------------------------------------------------------------------------------------------------------------
// プレイヤーの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitPlayer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// プレイヤー情報の読み込み
	LoadPlayerData();

	// プレイヤー情報の初期化
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.bJump = false;
	g_player.bFall = false;
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.MotionType = MOTIONTYPE_NEUTRAL;
	g_player.MotionTypeOld = g_player.MotionType;
	g_player.nStateCounter = 0;
	g_player.nCounterMotion = 1;
	g_player.nKey = 0;
	g_player.bLoopMotion = g_player.aMotionInfo[g_player.MotionType].bLoop;
	g_player.nNumKey = g_player.aMotionInfo[g_player.MotionType].nNumKey;

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
	g_player.nIdx = SetShadow(D3DXVECTOR3(g_player.pos.x, g_player.pos.y, g_player.pos.z), 20.0f, 20.0f);
	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// プレイヤーの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------------
// プレイヤーの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdatePlayer(void)
{
	// プレイヤーモーション
	PlayerMotion();

	// プレイヤーの状態を判別
	StateManagerPlayer();

	// 前の座標を保存
	g_player.posOld = g_player.pos;

	// 移動量の減衰(慣性)
	g_player.move.x += (0.0f - g_player.move.x) * 0.3f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.3f;

	// 重力設定
	g_player.move.y -= GRAVITY;

	// 位置の更新
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;

	// コインとの当たり判定
	CollisionCoin(&g_player.pos, &g_player.posOld, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// メッシュフィールドとの当たり判定
	bool bLand = CollisionMeshField(&g_player.pos, &g_player.posOld, g_player.nIdx);

	// モデルとの当たり判定
	bool bLandModel = CollisionModelSet(&g_player.pos, &g_player.posOld, &g_player.move,&g_player.minVecPlayer, &g_player.maxVecPlayer,g_player.nIdx);

	if (bLand == true || bLandModel == true)
	{
		if (g_player.state == PLAYERSTATE_FLOATING)
		{// ジャンプ中のときジャンプしていない状態にする
			g_player.state = PLAYERSTATE_NORMAL;
		}

		// 移動量を0.0fにする
		g_player.move.y = 0.0f;
	}
	else if (bLand == false && bLandModel == false)
	{
		// 何も乗っていないとき浮いている状態にする
		g_player.state = PLAYERSTATE_FLOATING;
	}

	// 落ちたらフェード
	if (g_player.pos.y < -100.0f)
	{
		g_player.bFall = true;
	}
}

//--------------------------------------------------------------------------------------------------------------
// プレイヤーの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------------
// プレイヤー状態の判別
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------------
// プレイヤー状態が通常の時
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void PlayerStateNormal(void)
{
	// ゲームパッド情報の取得
	DIJOYSTATE2 Controller = GetControllerPless(PLAYER_1);

	// プレイヤーの移動処理
	MovePlayer();

	// 敵との当たり判定
	bool bCollisionEnemy = CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// 敵に当たったらダメージ状態にする
	if (bCollisionEnemy == true)
	{
		g_player.state = PLAYERSTATE_DAMAGE;

		// 音を出す
		PlaySound(SOUND_LABEL_SE_DAMAGE);
	}

	if (GetKeyboardPress(KEYINFO_UP) == false && GetKeyboardPress(KEYINFO_DOWN) == false &&
		GetKeyboardPress(KEYINFO_LEFT) == false && GetKeyboardPress(KEYINFO_RIGHT) == false &&
		g_player.bJump == false && g_player.MotionType != MOTIONTYPE_LAND &&
		Controller.lX == 0 && Controller.lY == 0)
	{
		// 何もしないときニュートラルモーションに設定
		SetMotion(MOTIONTYPE_NEUTRAL, true, 10);
	}
}

//--------------------------------------------------------------------------------------------------------------
// プレイヤー状態が浮遊の時
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void PlayerStateFloating(void)
{
	// プレイヤーの移動処理
	MovePlayer();

	// モーションをジャンプモーションにする
	SetMotion(MOTIONTYPE_JUMP, true, 10);

	// 敵との当たり判定
	bool bCollisionEnemy = CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.minVecPlayer, &g_player.maxVecPlayer);

	// 敵に当たったらダメージ状態にする
	if (bCollisionEnemy == true)
	{
		g_player.state = PLAYERSTATE_DAMAGE;

		// 音を出す
		PlaySound(SOUND_LABEL_SE_DAMAGE);
	}
}

//--------------------------------------------------------------------------------------------------------------
// プレイヤーがダメージ状態のとき
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void PlayerStateDamage(void)
{
	// 状態カウンタを進める
	g_player.nStateCounter++;

	// のけぞりモーションに変える
	SetMotion(MOTIONTYPE_DAMAGE, true, 5);

	// 状態カウンターが20になったとき処理
	if (g_player.nStateCounter >= 20)
	{
		// モーションを戻す
		SetMotion(MOTIONTYPE_NEUTRAL, true, 10);

		// 状態をノーマルに戻す
		g_player.state = PLAYERSTATE_NORMAL;

		// 状態カウンターを0に初期化
		g_player.nStateCounter = 0;
	}
}

//--------------------------------------------------------------------------------------------------------------
// プレイヤーの移動処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void MovePlayer(void)
{
	// カウントダウンが終わったかどうか取得
	bool bFinishCountDown = GetFinishCountDown();

	// 構造体のポインタ変数
	Camera pCamera = GetPlayerCamera();

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

	if (bFinishCountDown == true)
	{
		// キーボード
		MovePlayerKeyboard();

		// ゲームパッド
		MovePlayerGamepad();
	}
}

//--------------------------------------------------------------------------------------------------------------
// プレイヤーの移動処理(キーボード)
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void MovePlayerKeyboard(void)
{
	// 情報の取得
	Camera camera = GetPlayerCamera();				// カメラ情報
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
		SetMotion(MOTIONTYPE_MOVE, true, 10);
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
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// モデル右移動
		g_player.move.z -= MOVE_MODEL * sinf(camera.rot.y);
		g_player.move.x += MOVE_MODEL * cosf(camera.rot.y);
		g_player.rotDest.y = camera.rot.y - D3DX_PI / 2.0f;

		// 移動しているとき移動モーションに設定
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{// モデル左移動
		g_player.move.z += MOVE_MODEL * sinf(camera.rot.y);
		g_player.move.x -= MOVE_MODEL * cosf(camera.rot.y);
		g_player.rotDest.y = camera.rot.y + D3DX_PI / 2.0f;

		// 移動しているとき移動モーションに設定
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	if(GetKeyboardTrigger(KEYINFO_SHOOT) == true &&
		g_player.state != PLAYERSTATE_FLOATING)
	{
		g_player.move.y += JUMP_PLAYER;
		g_player.state = PLAYERSTATE_FLOATING;

		// 音を出す
		PlaySound(SOUND_LABEL_SE_JUMP);
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------
// プレイヤーの移動処理(ゲームパッド)
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void MovePlayerGamepad(void)
{
	// 情報の取得
	Camera camera = GetPlayerCamera();				// カメラ情報

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
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}
	// ジャンプ処理
	if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true &&
		g_player.state != PLAYERSTATE_FLOATING)
	{
		g_player.move.y += JUMP_PLAYER;
		g_player.state = PLAYERSTATE_FLOATING;

		// 音を出す
		PlaySound(SOUND_LABEL_SE_JUMP);
	}
}

//--------------------------------------------------------------------------------------------------------------
// プレイヤー情報の取得
// 引数		：void		- 何もなし
// 返り値	：Player	- Player型構造体のポインタを返す
//--------------------------------------------------------------------------------------------------------------
Player *GetPlayer(void)
{
	return &g_player;
}

//--------------------------------------------------------------------------------------------------------------
// プレイヤ-モーションの設定
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void PlayerMotion(void)
{
	// 変数宣言
	Player *pPlayer = GetPlayer();
	KEY *pKey;				// キー情報
	KEY *pKeyNext;			// 次のキー情報
	KEY *pKeyBlend;			// ブレンドキー情報
	KEY *pKeyNextBlend;		// 次のブレンドキー情報
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == false && pPlayer->bBlendMotion == false)
		{
			break;
		}

		// キーの確定
		pKey = &pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[pPlayer->nKey].aKey[nCntModel];
		pKeyNext = &pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[(pPlayer->nKey + 1) % pPlayer->nNumKey].aKey[nCntModel];

		// 差分の確定
		float fDiffMoitonPosX = pKeyNext->fPosX - pKey->fPosX;
		float fDiffMoitonPosY = pKeyNext->fPosY - pKey->fPosY;
		float fDiffMoitonPosZ = pKeyNext->fPosZ - pKey->fPosZ;
		float fDiffMoitonRotX = pKeyNext->fRotX - pKey->fRotX;
		float fDiffMoitonRotY = pKeyNext->fRotY - pKey->fRotY;
		float fDiffMoitonRotZ = pKeyNext->fRotZ - pKey->fRotZ;

		// 相対値の確定
		float fRateMotion = (float)pPlayer->nCounterMotion / (float)pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[pPlayer->nKey].nFrame;

		if (g_player.bBlendMotion == true)
		{
			// キーの確定
			pKeyBlend = &pPlayer->aMotionInfo[pPlayer->MotionTypeBlend].aKeyInfo[pPlayer->nKeyBlend].aKey[nCntModel];
			pKeyNextBlend = &pPlayer->aMotionInfo[pPlayer->MotionTypeBlend].aKeyInfo[(pPlayer->nKeyBlend + 1) % pPlayer->nNumKeyBlend].aKey[nCntModel];

			// 差分の確定
			float fDiffMoitonBlendPosX = pKeyNextBlend->fPosX - pKeyBlend->fPosX;
			float fDiffMoitonBlendPosY = pKeyNextBlend->fPosY - pKeyBlend->fPosY;
			float fDiffMoitonBlendPosZ = pKeyNextBlend->fPosZ - pKeyBlend->fPosZ;
			float fDiffMoitonBlendRotX = pKeyNextBlend->fRotX - pKeyBlend->fRotX;
			float fDiffMoitonBlendRotY = pKeyNextBlend->fRotY - pKeyBlend->fRotY;
			float fDiffMoitonBlendRotZ = pKeyNextBlend->fRotZ - pKeyBlend->fRotZ;

			// 相対値の確定
			float fRateMotionBlend = (float)pPlayer->nCounterMotionBlend / (float)pPlayer->aMotionInfo[pPlayer->MotionTypeBlend].aKeyInfo[pPlayer->nKeyBlend].nFrame;

			float fRateBlend = (float)pPlayer->nCounterBlend / (float)pPlayer->nFrameBlend;

			// 現在のモーションの位置と向きの確定
			float fPosXCurrent = pKey->fPosX + (fDiffMoitonPosX * fRateMotion);
			float fPosYCurrent = pKey->fPosY + (fDiffMoitonPosY * fRateMotion);
			float fPosZCurrent = pKey->fPosZ + (fDiffMoitonPosZ * fRateMotion);
			float fRotXCurrent = pKey->fRotX + (fDiffMoitonRotX * fRateMotion);
			float fRotYCurrent = pKey->fRotY + (fDiffMoitonRotY * fRateMotion);
			float fRotZCurrent = pKey->fRotZ + (fDiffMoitonRotZ * fRateMotion);

			// 角度の制限
			D3DXVECTOR3(fRotXCurrent, fRotYCurrent, fRotZCurrent) = AngleCorrection(fRotXCurrent, fRotYCurrent, fRotZCurrent);

			// ブレンドモーションの位置と向きの確定
			float fPosXBlend = pKeyBlend->fPosX + (fDiffMoitonBlendPosX * fRateMotionBlend);
			float fPosYBlend = pKeyBlend->fPosY + (fDiffMoitonBlendPosY * fRateMotionBlend);
			float fPosZBlend = pKeyBlend->fPosZ + (fDiffMoitonBlendPosZ * fRateMotionBlend);
			float fRotXBlend = pKeyBlend->fRotX + (fDiffMoitonBlendRotX * fRateMotionBlend);
			float fRotYBlend = pKeyBlend->fRotY + (fDiffMoitonBlendRotY * fRateMotionBlend);
			float fRotZBlend = pKeyBlend->fRotZ + (fDiffMoitonBlendRotZ * fRateMotionBlend);

			// 角度の制限
			D3DXVECTOR3(fRotXBlend, fRotYBlend, fRotZBlend) = AngleCorrection(fRotXBlend, fRotYBlend, fRotZBlend);

			// 現在のモーションとブレンドモーションの差分
			float fDiffBlendPosX = fPosXBlend - fPosXCurrent;
			float fDiffBlendPosY = fPosYBlend - fPosYCurrent;
			float fDiffBlendPosZ = fPosZBlend - fPosZCurrent;
			float fDiffBlendRotX = fRotXBlend - fRotXCurrent;
			float fDiffBlendRotY = fRotYBlend - fRotYCurrent;
			float fDiffBlendRotZ = fRotZBlend - fRotZCurrent;

			// 角度の制限
			D3DXVECTOR3(fDiffBlendRotX, fDiffBlendRotY, fDiffBlendRotZ) = AngleCorrection(fDiffBlendRotX, fDiffBlendRotY, fDiffBlendRotZ);

			// ブレンドモーションの位置と向きの確定
			fPosX = fPosXCurrent + (fDiffBlendPosX * fRateBlend);
			fPosY = fPosYCurrent + (fDiffBlendPosY * fRateBlend);
			fPosZ = fPosZCurrent + (fDiffBlendPosZ * fRateBlend);
			fRotX = fRotXCurrent + (fDiffBlendRotX * fRateBlend);
			fRotY = fRotYCurrent + (fDiffBlendRotY * fRateBlend);
			fRotZ = fRotZCurrent + (fDiffBlendRotZ * fRateBlend);
		}
		else
		{
			// モーションの位置と向きの確定
			fPosX = pKey->fPosX + (fDiffMoitonPosX * fRateMotion);
			fPosY = pKey->fPosY + (fDiffMoitonPosY * fRateMotion);
			fPosZ = pKey->fPosZ + (fDiffMoitonPosZ * fRateMotion);
			fRotX = pKey->fRotX + (fDiffMoitonRotX * fRateMotion);
			fRotY = pKey->fRotY + (fDiffMoitonRotY * fRateMotion);
			fRotZ = pKey->fRotZ + (fDiffMoitonRotZ * fRateMotion);
		}
		// 角度の制限
		D3DXVECTOR3(fRotX, fRotY, fRotZ) = AngleCorrection(fRotX, fRotY, fRotZ);

		// 位置と向きの設定
		pPlayer->aModel[nCntModel].pos.x = fPosX;
		pPlayer->aModel[nCntModel].pos.y = fPosY;
		pPlayer->aModel[nCntModel].pos.z = fPosZ;
		pPlayer->aModel[nCntModel].rot.x = fRotX;
		pPlayer->aModel[nCntModel].rot.y = fRotY;
		pPlayer->aModel[nCntModel].rot.z = fRotZ;
	}

	if (g_player.bBlendMotion == true)
	{
		if (pPlayer->nCounterMotionBlend > pPlayer->aMotionInfo[pPlayer->MotionTypeBlend].aKeyInfo[pPlayer->nKeyBlend].nFrame)
		{
			// ブレンドモーションカウンタの初期化
			pPlayer->nCounterMotionBlend = 0;

			// キーの更新
			pPlayer->nKeyBlend++;

			// キーの確定
			pPlayer->nKeyBlend = pPlayer->nKeyBlend % pPlayer->nNumKeyBlend;
		}
		else
		{
			if (pPlayer->nCounterBlend >= pPlayer->nFrameBlend)
			{
				// モーションの確定
				pPlayer->MotionType = pPlayer->MotionTypeBlend;

				// モーションカウンタを初期化
				pPlayer->nCounterMotion = pPlayer->nCounterMotionBlend;

				// 現在のキーを初期化
				pPlayer->nKey = pPlayer->nKeyBlend;

				// ループするかどうか
				pPlayer->bLoopMotion = pPlayer->bLoopMotionBlend;

				// キー数の確定
				pPlayer->nNumKey = pPlayer->nNumKeyBlend;

				// モーションブレンドを終える
				pPlayer->bBlendMotion = false;
			}
			else
			{
				// ブレンドモーションカウンタの更新
				pPlayer->nCounterMotionBlend++;

				// ブレンドカウンタの更新
				pPlayer->nCounterBlend++;
			}
		}
	}
	else
	{
		// モーションカウンタが各キーの最終フレームに到達したとき
  		if (pPlayer->nCounterMotion >= pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[pPlayer->nKey].nFrame)
		{
			if (g_player.bLoopMotion == false && pPlayer->nKey == pPlayer->nNumKey - 1)
			{
				// モーションカウンタの更新
  				pPlayer->nCounterMotion = pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[pPlayer->nKey].nFrame;
			}
			else
			{
				// モーションカウンタの初期化
				pPlayer->nCounterMotion = 0;

				// キーの更新
				pPlayer->nKey++;

				// キーの確定
				pPlayer->nKey = pPlayer->nKey % pPlayer->nNumKey;
			}
		}
		else
		{
			// モーションカウンタの更新
			pPlayer->nCounterMotion++;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// モーションの設定
// 引数		：motiontype	- モーションタイプ
//			：bBlend		- ブレンドするかどうか
//			：nFrameBlend	- ブレンドフレーム数
// 返り値	：void			- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetMotion(MOTIONTYPE motiontype, bool bBlend, int nFrameBlend)
{
	// 変数宣言
	Player *pPlayer = GetPlayer();

	// モーションタイプによって条件を変える
	switch (motiontype)
	{
	case MOTIONTYPE_NEUTRAL:
		if (g_player.state == PLAYERSTATE_FLOATING)
		{
			motiontype = MOTIONTYPE_JUMP;
		}
		break;

	case MOTIONTYPE_MOVE:
		if (g_player.state == PLAYERSTATE_FLOATING)
		{
			motiontype = MOTIONTYPE_JUMP;
		}
		break;

	case MOTIONTYPE_JUMP:
		break;

	case MOTIONTYPE_LAND:
		break;

	case MOTIONTYPE_DAMAGE:
		break;

	default:
		break;
	}
	//if (g_player.bFinishMotion == true)
	{
		if (bBlend == true && g_player.MotionTypeBlend != motiontype)
		{
			// モーションの確定
			g_player.MotionTypeBlend = motiontype;

			// モーションカウンタを初期化
			g_player.nCounterMotionBlend = 0;

			// 現在のキーを初期化
			g_player.nKeyBlend = 0;

			// ループするかどうか
			g_player.bLoopMotionBlend = g_player.aMotionInfo[g_player.MotionTypeBlend].bLoop;

			// キー数の確定
			g_player.nNumKeyBlend = g_player.aMotionInfo[g_player.MotionTypeBlend].nNumKey;

			// ブレンドフレームの設定
			g_player.nFrameBlend = nFrameBlend;

			// ブレンドするかどうか
			g_player.bBlendMotion = bBlend;

			// ブレンドカウンタの初期化
			g_player.nCounterBlend = 0;
		}
		else if (bBlend == false && g_player.MotionType != motiontype)
		{
			// モーションの確定
			g_player.MotionType = motiontype;

			// モーションカウンタを初期化
			g_player.nCounterMotion = 0;

			// 現在のキーを初期化
			g_player.nKey = 0;

			// ループするかどうか
			g_player.bLoopMotion = g_player.aMotionInfo[g_player.MotionType].bLoop;

			// キー数の確定
			g_player.nNumKey = g_player.aMotionInfo[g_player.MotionType].nNumKey;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// 角度の制限
// 引数		：rotX			- X軸の角度情報
//			：rotY			- Y軸の角度情報
//			：rotZ			- Z軸の角度情報
// 返り値	：D3DXVECTOR3	- 回転角を制限した後の角度情報を渡す
//--------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 AngleCorrection(float rotX, float rotY, float rotZ)
{
	// 現在の回転角を制限
	if (rotX > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		rotX -= D3DX_PI * 2.0f;
	}
	else if (rotX < -D3DX_PI)
	{// -3.14fより小さくなったとき値に3.14fにする
		rotX += D3DX_PI * 2.0f;
	}
	// 現在の回転角を制限
	if (rotY  > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		rotY -= D3DX_PI * 2.0f;
	}
	else if (rotY < -D3DX_PI)
	{// -3.14fより小さくなったとき値に3.14fにする
		rotY += D3DX_PI * 2.0f;
	}
	// 現在の回転角を制限
	if (rotZ > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		rotZ -= D3DX_PI * 2.0f;
	}
	else if (rotZ < -D3DX_PI)
	{// -3.14fより小さくなったとき値に3.14fにする
		rotZ += D3DX_PI * 2.0f;
	}

	return D3DXVECTOR3(rotX, rotY, rotZ);
}