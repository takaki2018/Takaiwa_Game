//-----------------------------------------------------------------
//
// プレイヤー (polygon.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "player.h"
#include "bg.h"
#include "input.h"
#include "gamepad.h"
#include "bullet.h"
#include "enemy.h"
#include "setparticle.h"
#include "object.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define PLAYER_VERTEX_X			(SCREEN_WIDTH / 2)			// プレイヤーの中心座標(X)
#define PLAYER_VERTEX_Y			(SCREEN_HEIGHT / 2)			// プレイヤーの中心座標(Y)

#define PLAYER_SIZE_WIDTH		(15)						// プレイヤーのサイズ(幅)
#define PLAYER_SIZE_HEIGHT		(15)						// プレイヤーのサイズ(高さ)

#define MOVE_PLAYER				(0.5f)						// プレイヤーの移動量
#define PI_QUARTER				(D3DX_PI / 4)				// 円周率の1/4(斜め移動で使う)

#define HIT_DATA				(15)						// 当たり判定
#define RENGE					(250)						// 円の半径

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// プレイヤーの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			// プレイヤーのテクスチャのポインタ
PLAYER g_Player;									// プレイヤーの情報
D3DXVECTOR3 g_rotPlayer;							// プレイヤーの回転角	
float g_fLengthPlayer;								// プレイヤーの対角線の長さ
float g_fAnglePlayer;								// プレイヤーの対角線の角度
D3DXCOLOR g_Playercol;								// プレイヤーの色
D3DXCOLOR g_PlyaerOrbit;							// プレイヤーの軌跡の色

//-----------------------------------------------------------------
// プレイヤーの初期化処理
//-----------------------------------------------------------------
HRESULT InitPlayer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_Playercol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_PlyaerOrbit = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player100.png", &g_pTexturePlayer);

	// プレイヤー情報の初期化
	g_Player.pos = D3DXVECTOR3(PLAYER_VERTEX_X, PLAYER_VERTEX_Y, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(NULL, NULL, NULL);
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.nCounterState = NULL;
	g_Player.nCounterDisp = NULL;
	g_Player.bDisp = false;

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 対角線の長さ
	g_fLengthPlayer = sqrtf(PLAYER_SIZE_WIDTH * PLAYER_SIZE_WIDTH + PLAYER_SIZE_HEIGHT * PLAYER_SIZE_HEIGHT);

	// 対角線の角度
	g_fAnglePlayer = atan2f(PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE_WIDTH, g_Player.pos.y + PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE_WIDTH, g_Player.pos.y - PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE_WIDTH, g_Player.pos.y + PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE_WIDTH, g_Player.pos.y - PLAYER_SIZE_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = g_Playercol;
	pVtx[1].col = g_Playercol;
	pVtx[2].col = g_Playercol;
	pVtx[3].col = g_Playercol;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// プレエヤーの終了処理
//-----------------------------------------------------------------
void UninitPlayer(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	// テクスチャの開放
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//-----------------------------------------------------------------
// プレイヤーの更新処理
//-----------------------------------------------------------------
void UpdatePlayer(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// プレイヤーの状態
	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
		// 通常の時
		PlayerStateNormal();
		break;

	case PLAYERSTATE_HIT:
		// 敵に当たったまたは攻撃を受けたとき
		PlayerStateHit();
		break;

	case PLAYERSTATE_FLASHING:
		// 点滅のとき
		PlayerStateFlashing();
		break;

	default:
		break;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// プレイヤーの色更新
	pVtx[0].col = g_Playercol;
	pVtx[1].col = g_Playercol;
	pVtx[2].col = g_Playercol;
	pVtx[3].col = g_Playercol;

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// プレイヤーの描画処理
//-----------------------------------------------------------------
void DrawPlayer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	// プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------
// プレイヤーの移動処理
//-----------------------------------------------------------------
void MovePlayer(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	DIJOYSTATE2 Controller = GetControllerPless();

	// 変数宣言
	D3DXVECTOR3 posObject = GetposObject();
	int nCntBulletcol = GetBulletcol();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// プレイヤーの位置を更新
	if (GetKeyboardPress(KEYINFO_UP) == true )
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// 右斜め上移動
			g_Player.move.x += MOVE_PLAYER * sinf(PI_QUARTER);
			g_Player.move.y -= MOVE_PLAYER * cosf(PI_QUARTER);
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// 左斜め上移動
			g_Player.move.x -= MOVE_PLAYER * sinf(PI_QUARTER);
			g_Player.move.y -= MOVE_PLAYER * cosf(PI_QUARTER);
		}
		else
		{// 上移動
			g_Player.move.y -=  MOVE_PLAYER;
		}
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true)
	{
		if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// 右斜め下移動
			g_Player.move.x += MOVE_PLAYER * sinf(PI_QUARTER);
			g_Player.move.y += MOVE_PLAYER * cosf(PI_QUARTER);
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// 左斜め下移動
			g_Player.move.x -= MOVE_PLAYER * sinf(PI_QUARTER);
			g_Player.move.y += MOVE_PLAYER * cosf(PI_QUARTER);
		}
		else 
		{// 下移動
			g_Player.move.y += MOVE_PLAYER;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// 右移動
		g_Player.move.x += MOVE_PLAYER;
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{// 左移動
		g_Player.move.x -= MOVE_PLAYER;
	}
	// JOYPADのスティック入力
	if (Controller.lY != 0 || Controller.lX != 0)
	{// スティックの入力角度
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY * -1);

		g_Player.move.x += MOVE_PLAYER * sinf(D3DX_PI - fAngle);
		g_Player.move.y += MOVE_PLAYER * cosf(D3DX_PI - fAngle);
	}
	// 回転
	if (GetKeyboardPress(KEYINFO_LEFT_TURN) == true || GetJoypadPress(JPINFO_LEFT_TURN) == true)
	{// 左回転
		g_Player.rot.z += 0.07f;
		if (g_Player.rot.z > D3DX_PI)
		{
			g_Player.rot.z -= D3DX_PI * 2.0f;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT_TURN) == true || GetJoypadPress(JPINFO_RIGHT_TURN) == true)
	{// 右回転
		g_Player.rot.z -= 0.07f;
		if (g_Player.rot.z > -D3DX_PI)
		{
			g_Player.rot.z += D3DX_PI * 2.0f;
		}
	}
	// 位置更新
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;

	// 自機と中心点の対角線の長さ
	float fLength = sqrtf((g_Player.pos.x - posObject.x) * (g_Player.pos.x - posObject.x) +
		(g_Player.pos.y - posObject.y) * (g_Player.pos.y - posObject.y));

	if (fLength > RENGE)
	{// 現在の角度
		float fAngle = atan2f(g_Player.pos.x - posObject.x, g_Player.pos.y - posObject.y);
		
		// 座標の設定
		g_Player.pos.x = RENGE * sinf(fAngle) + posObject.x;
		g_Player.pos.y = RENGE * cosf(fAngle) + posObject.y;
	}

	// 画面外に出ないようにする
	if (g_Player.pos.y - PLAYER_SIZE_HEIGHT < 0)
	{// 一番上に到達したとき処理
		g_Player.pos.y = PLAYER_SIZE_HEIGHT;
	}
	if (g_Player.pos.y + PLAYER_SIZE_HEIGHT > SCREEN_HEIGHT)
	{// 一番下に到達したとき処理
		g_Player.pos.y = SCREEN_HEIGHT - PLAYER_SIZE_HEIGHT;
	}
	if (g_Player.pos.x - PLAYER_SIZE_WIDTH <= 0)
	{// 一番左に到達したとき処理
		g_Player.pos.x = PLAYER_SIZE_WIDTH;
	}
	if (g_Player.pos.x + PLAYER_SIZE_WIDTH >= SCREEN_WIDTH)
	{// 一番右に到達したとき処理
		g_Player.pos.x = SCREEN_WIDTH - PLAYER_SIZE_WIDTH;
	}

	// 移動量の減衰(慣性)
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.1f;
	g_Player.move.y += (0.0f - g_Player.move.y) * 0.1f;

	// 頂点座標の更新
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI * 0.00f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI * 0.00f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (g_fAnglePlayer + D3DX_PI * 1.00f)) * g_fLengthPlayer;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (g_fAnglePlayer + D3DX_PI * 1.00f)) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (g_fAnglePlayer + D3DX_PI * 0.00f)) * g_fLengthPlayer;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (g_fAnglePlayer + D3DX_PI * 0.00f)) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI * 1.00f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI * 1.00f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// 軌道色の設定
	if (nCntBulletcol == BULLETCOLOR_WHITE)
	{// 白に設定
		g_PlyaerOrbit = D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.9f);
	}
	else if (nCntBulletcol == BULLETCOLOR_RED)
	{// 赤に設定
		g_PlyaerOrbit = D3DXCOLOR(0.9f, 0.3f, 0.1f, 0.9f);
	}
	else if (nCntBulletcol == BULLETCOLOR_BLUE)
	{// 青に設定
		g_PlyaerOrbit = D3DXCOLOR(0.1f, 0.3f, 0.9f, 0.9f);
	}
	// プレイヤーの軌道
	SetMoveOrbit(g_Player.pos,
		D3DXVECTOR3(sinf(g_Player.rot.z) * (float)(rand() % 10 + 5), cosf(g_Player.rot.z) * (float)(rand() % 10 + 5), 0.0f),
		g_PlyaerOrbit,
		SETPARTICLE_MOVE_PLAYER);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// プレイヤーの当たり判定
//-----------------------------------------------------------------
bool HitPlayer(int nIdx)
{
	// 構造体のポインタ変数
	ENEMY *pEnemy;
	BULLET *pBullet;

	// 変数の受け渡し
	pEnemy = GetEnemy();
	pBullet = GetBullet();

	// 何番目か
	pEnemy += nIdx;
	pBullet += nIdx;

	if (pEnemy->bUse == true || pBullet->bUse == true)
	{// 敵または敵の弾が使われているとき
		// α値を0.0fに設定
		g_Playercol.a = 0.0f;

		// プレイヤーの状態を当たったときに変更
		g_Player.state = PLAYERSTATE_HIT;

		// 状態カウンタを60に設定
		g_Player.nCounterState = 60;

		// 爆発パーティクル
		SetBom(g_Player.pos, SETPARTICLE_HIT_PLAYER);
		
		// 効果音(爆発)
		PlaySound(SOUND_LABEL_SE_PLAYER_BRAEK);

		return true;
	}
	else
	{
		return false;
	}
}

//-----------------------------------------------------------------
// プレイヤー状態が通常の時
//-----------------------------------------------------------------
void PlayerStateNormal(void)
{
	// 構造体のポインタ変数
	ENEMY *pEnemy;

	// 変数の受け渡し
	pEnemy = GetEnemy();

	// 表示カウンタの初期化
	g_Player.nCounterDisp = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// 自機と敵の当たり判定
			if (pEnemy->pos.y - ENEMY_Y <= g_Player.pos.y + HIT_DATA &&
				pEnemy->pos.y + ENEMY_Y >= g_Player.pos.y - HIT_DATA &&
				pEnemy->pos.x - ENEMY_X + pEnemy->move.x <= g_Player.pos.x + HIT_DATA &&
				pEnemy->pos.x + ENEMY_X + pEnemy->move.x >= g_Player.pos.x - HIT_DATA)
			{// 敵に当たったら処理
				HitPlayer(nCntEnemy);
			}
		}
	}
	// 移動処理
	MovePlayer();
}

//-----------------------------------------------------------------
// プレイヤー状態が攻撃を受けたとき
//-----------------------------------------------------------------
void PlayerStateHit(void)
{
	// 状態カウンタの減少
	g_Player.nCounterState--;

	if (g_Player.nCounterState <= 0)
	{// 状態カウンタが0以下になったとき
		g_Player.nCounterState = 100;

		// プレイヤー状態を点滅に変更
		g_Player.state = PLAYERSTATE_FLASHING;
	}
}

//-----------------------------------------------------------------
// プレイヤー状態が点滅の時
//-----------------------------------------------------------------
void PlayerStateFlashing(void)
{
	// 状態カウンタの減少
	g_Player.nCounterState--;

	// 表示カウンタの加算
	g_Player.nCounterDisp++;

	if (g_Player.nCounterState <= 0)
	{// 状態カウンタが0以下のとき状態を通常に変更
		g_Player.state = PLAYERSTATE_NORMAL;
	}
	else
	{// 状態カウンタが0より大きいとき
		if (g_Player.nCounterDisp == 5)
		{// 表示カウンタが5のときα値を1.0fに設定
			g_Playercol.a = 1.0f;
		}
		else if (g_Player.nCounterDisp == 10)
		{// 表示カウンタが10のときα値を0.2fに設定
			g_Playercol.a = 0.2f;

			// 表示カウントの初期化
			g_Player.nCounterDisp = 0;
		}
	}

	// 移動処理
	MovePlayer();
}

//-----------------------------------------------------------------
// プレイヤー情報の取得
//-----------------------------------------------------------------
PLAYER *GetPlayer(void)
{
	return &g_Player;
}