//-----------------------------------------------------------------
//
// プレイヤー (polygon.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "player.h"
#include "selectmode.h"
#include "block.h"
#include "fruits.h"
#include "enemy.h"
#include "bear.h"
#include "point.h"
#include "score.h"
#include "gauge.h"
#include "input.h"
#include "gamepad.h"
#include "setparticle.h"
#include "sound.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define MAX_PLAYER			(4)															// プレイヤーの最大数
#define TEXTURE_X			(4)															// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y			(2)															// アニメーションパターンNo.の縦の最大数

#define PLAY_ANIM			((1.0f / TEXTURE_X) * g_aPlayer[nCntPlayer].nPatternAnim)	// テクスチャの動作
#define UV_U				(1.0f / TEXTURE_X + PLAY_ANIM)								// Uの値
#define UV_V				(1.0f / TEXTURE_Y)											// Vの値

#define PLAYER_VERTEX_X		(365.0f)													// プレイヤーの中心座標(X)
#define PLAYER_VERTEX_Y		(650.0f)													// プレイヤーの中心座標(Y)
#define PlAYER_WIDTH		(180.0f)													// プレイヤーを配置する間隔

#define MOVE_PLAYER			(1.5f)														// プレイヤーの移動量
#define PI_QUARTER			(D3DX_PI / 4)												// 円周率の1/4(斜め移動で使う)

#define JAMP_MOVE			(18.0f)														// ジャンプしたときの移動量
#define GRAVITY				(0.8f)														// 重力

#define WOOD_WIDTH			(80.0f)														// 木の幅

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		// プレイヤーの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayer[MAX_PLAYER] = {};	// プレイヤーのテクスチャのポインタ
PLAYER g_aPlayer[MAX_PLAYER];							// プレイヤーの情報
int g_nCntPlayer;										// プレイヤー数

//-----------------------------------------------------------------
// プレイヤーの初期化処理
//-----------------------------------------------------------------
HRESULT InitPlayer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// シングルモードの時処理
	case PLAYMODE_SINGLE:
		g_nCntPlayer = 1;
		break;
		// マルチモードの時処理
	case PLAYMODE_MULTI:
		g_nCntPlayer = GetnCntController();
		break;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_1P.png", &g_pTexturePlayer[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_2P.png", &g_pTexturePlayer[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_3P.png", &g_pTexturePlayer[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_4P.png", &g_pTexturePlayer[3]);

	// プレイヤー情報の初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = D3DXVECTOR3(PLAYER_VERTEX_X + PlAYER_WIDTH * nCntPlayer, PLAYER_VERTEX_Y, 0.0f);
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		g_aPlayer[nCntPlayer].move = D3DXVECTOR3(NULL, NULL, NULL);
		g_aPlayer[nCntPlayer].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aPlayer[nCntPlayer].fWidth = PLAYER_SIZE_WIDTH / 2.0f;
		g_aPlayer[nCntPlayer].fHeight = PLAYER_SIZE_HEIGHT / 2.0f;
		g_aPlayer[nCntPlayer].nCntAnim = 0;
		g_aPlayer[nCntPlayer].nPatternAnim = 0;
		g_aPlayer[nCntPlayer].fV = 0.0f;
		g_aPlayer[nCntPlayer].bMove = false;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].state = PLAYERSTATE_NORMAL;
		g_aPlayer[nCntPlayer].nCounterState = NULL;
		g_aPlayer[nCntPlayer].nCounterDisp = NULL;
		g_aPlayer[nCntPlayer].nCounterSize = NULL;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PLAYER, // 確保するバッファサイズ
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

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// 頂点座標の設定
		SetVertexPlayer(nCntPlayer);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// 更新
		pVtx += 4;
	}

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
	for (int nCntTexture = 0; nCntTexture < MAX_PLAYER; nCntTexture++)
	{
		if (g_pTexturePlayer[nCntTexture] != NULL)
		{
			g_pTexturePlayer[nCntTexture]->Release();
			g_pTexturePlayer[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// プレイヤーの更新処理
//-----------------------------------------------------------------
void UpdatePlayer(void)
{
	// 変数宣言
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// シングルモードのとき処理
	case PLAYMODE_SINGLE:
		SingleModePlayer();
		break;
	// マルチモードのとき処理
	case PLAYMODE_MULTI:
		MultiModePlayer();
		break;
	}
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

	for (int nCntPlayer = 0; nCntPlayer < g_nCntPlayer; nCntPlayer++)
	{// プレイヤーの描画
	 // テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer[nCntPlayer]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);
	}
}

//-----------------------------------------------------------------
// プレイヤーの頂点座標の設定
//-----------------------------------------------------------------
void SetVertexPlayer(int nIdx)
{
	// 変数宣言
	float ShaikingMove = GetShaking();

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer[nIdx].pos.x - g_aPlayer[nIdx].fWidth, g_aPlayer[nIdx].pos.y + g_aPlayer[nIdx].fHeight + ShaikingMove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer[nIdx].pos.x - g_aPlayer[nIdx].fWidth, g_aPlayer[nIdx].pos.y - g_aPlayer[nIdx].fHeight+ ShaikingMove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer[nIdx].pos.x + g_aPlayer[nIdx].fWidth, g_aPlayer[nIdx].pos.y + g_aPlayer[nIdx].fHeight+ ShaikingMove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer[nIdx].pos.x + g_aPlayer[nIdx].fWidth, g_aPlayer[nIdx].pos.y - g_aPlayer[nIdx].fHeight+ ShaikingMove, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = g_aPlayer[nIdx].col;
	pVtx[1].col = g_aPlayer[nIdx].col;
	pVtx[2].col = g_aPlayer[nIdx].col;
	pVtx[3].col = g_aPlayer[nIdx].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// シングルモードのプレイヤー処理
//-----------------------------------------------------------------
void SingleModePlayer(void)
{
	// 変数宣言
	MODE mode = GetMode();

	switch (mode)
	{// ゲーム画面のとき
	case MODE_GAME:
		// 共通の処理
		PlayerNormalProcessing(PLAYER_1);

		// プレイヤー状態で識別
		switch (g_aPlayer[PLAYER_1].state)
		{// プレイヤー状態が点滅のとき
		case PLAYERSTATE_FLASHING:
			PlayerStateFlashing(PLAYER_1);
			break;
		}
		break;

		// リザルト画面のとき
	case MODE_RESULT:
		ResultPlayer(PLAYER_1);
		break;
	}
}

//-----------------------------------------------------------------
// マルチモードのプレイヤー処理
//-----------------------------------------------------------------
void MultiModePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < g_nCntPlayer; nCntPlayer++)
	{
		// 変数宣言
		MODE mode = GetMode();

		switch (mode)
		{// ゲーム画面のとき
		case MODE_GAME:
			// 共通の処理
			PlayerNormalProcessing(nCntPlayer);

			if (g_aPlayer[nCntPlayer].fHeight != PLAYER_SIZE_HEIGHT / 2.0f)
			{// 高さが縮んでいるとき
				PlayerStateShrink(nCntPlayer);
			}
			// プレイヤー状態で識別
			switch (g_aPlayer[nCntPlayer].state)
			{// プレイヤー状態が点滅のとき
			case PLAYERSTATE_FLASHING:
				PlayerStateFlashing(nCntPlayer);
				break;
			}
			break;

			// リザルト画面のとき
		case MODE_RESULT:
			ResultPlayer(nCntPlayer);
			break;
		}

	}
}

//-----------------------------------------------------------------
// シングルモードのプレイヤー移動処理
//-----------------------------------------------------------------
void SingleMovePlayer(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 変数宣言
	bool *pStickPress = GetStickPress(PLAYER_1);
	bool *pCrossKeyPress = GetCrossKeyPress(PLAYER_1);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// プレイヤーの移動
	if (pStickPress[STICKPRESS_RIGHT] == true || 
		pCrossKeyPress[CROSSKEYPRESS_RIGHT] == true ||
		GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// 右移動
		g_aPlayer[PLAYER_1].move.x += MOVE_PLAYER;
		g_aPlayer[PLAYER_1].nCntAnim++;
		g_aPlayer[PLAYER_1].fV = 0.0f;
		g_aPlayer[PLAYER_1].bMove = true;

		// 砂埃
		SetMoveOrbit(D3DXVECTOR3(g_aPlayer[PLAYER_1].pos.x, g_aPlayer[PLAYER_1].pos.y + g_aPlayer[PLAYER_1].fHeight, 0.0f),
			g_aPlayer[PLAYER_1].move, SETPARTICLE_MOVE_PLAYER);
	}
	else if (pStickPress[STICKPRESS_LEFT] == true || 
		pCrossKeyPress[CROSSKEYPRESS_LEFT] == true ||
		GetKeyboardPress(KEYINFO_LEFT) == true)
	{// 左移動
		g_aPlayer[PLAYER_1].move.x -= MOVE_PLAYER;
		g_aPlayer[PLAYER_1].nCntAnim++;
		g_aPlayer[PLAYER_1].fV = 0.5f;
		g_aPlayer[PLAYER_1].bMove = true;

		// 砂埃
		SetMoveOrbit(D3DXVECTOR3(g_aPlayer[PLAYER_1].pos.x, g_aPlayer[PLAYER_1].pos.y + g_aPlayer[PLAYER_1].fHeight, 0.0f),
			g_aPlayer[PLAYER_1].move, SETPARTICLE_MOVE_PLAYER);
	}

	// 移動をやめたとき
	if (pStickPress[STICKPRESS_RIGHT] == false && pStickPress[STICKPRESS_LEFT] == false &&
		pCrossKeyPress[CROSSKEYPRESS_RIGHT] == false && pCrossKeyPress[CROSSKEYPRESS_LEFT] == false &&
		GetKeyboardPress(KEYINFO_RIGHT) == false && GetKeyboardPress(KEYINFO_LEFT) == false)
	{// 動いていない状態にする
		g_aPlayer[PLAYER_1].bMove = false;

		if (g_aPlayer[PLAYER_1].bJump == false)
		{// 何もしていないときアニメーションNO.0にする
			g_aPlayer[PLAYER_1].nPatternAnim = 0;
		}
	}
	// ジャンプしたときまたは空中にいるとき
	if (g_aPlayer[PLAYER_1].bJump == false)
	{
		if (GetJoypadTrigger(PLAYER_1, JPINFO_OKorJAMP) == true ||
			GetKeyboardTrigger(KEYINFO_JAMP) == true)
		{
			// サウンド
			PlaySound(SOUND_LABEL_SE_JUMP);

			// ジャンプ処理
			g_aPlayer[PLAYER_1].move.y -= JAMP_MOVE;
			g_aPlayer[PLAYER_1].nPatternAnim = 1;
			g_aPlayer[PLAYER_1].bJump = true;
		}
		if (g_aPlayer[PLAYER_1].move.y != 0.0f)
		{// ジャンプ中の処理
			g_aPlayer[PLAYER_1].bJump = true;
			g_aPlayer[PLAYER_1].nPatternAnim = 1;
		}
	}
	// 重力の設定
	g_aPlayer[PLAYER_1].move.y += GRAVITY;

	// 位置更新
	g_aPlayer[PLAYER_1].pos.x += g_aPlayer[PLAYER_1].move.x;
	g_aPlayer[PLAYER_1].pos.y += g_aPlayer[PLAYER_1].move.y;

	// ブロックとの当たり判定
	HitBlock(PLAYER_1);

	// 画面外に出ないようにする
	if (g_aPlayer[PLAYER_1].pos.y - g_aPlayer[PLAYER_1].fHeight >= 580.0f)
	{// 穴のある座標の高さにあるとき
		if (g_aPlayer[PLAYER_1].pos.x - g_aPlayer[PLAYER_1].fWidth * 0.7f <= 50.0f)
		{// 左側の穴にいるとき左側の木の上から出る
			g_aPlayer[PLAYER_1].pos.x = g_aPlayer[PLAYER_1].fWidth * 0.7f + 50.0f;
			g_aPlayer[PLAYER_1].pos.y = 270.0f;
		}
		else if (g_aPlayer[PLAYER_1].pos.x - g_aPlayer[PLAYER_1].fWidth * 0.7f >= SCREEN_WIDTH - 50.0f)
		{// 右側の穴にいるとき右側の木の上から出る
			g_aPlayer[PLAYER_1].pos.x = SCREEN_WIDTH - 50.0f - g_aPlayer[PLAYER_1].fWidth * 0.7f;
			g_aPlayer[PLAYER_1].pos.y = 270.0f;
		}
	}
	else if (g_aPlayer[PLAYER_1].pos.x - g_aPlayer[PLAYER_1].fWidth * 0.7f <= WOOD_WIDTH)
	{// 一番左に到達したとき処理
		g_aPlayer[PLAYER_1].pos.x = g_aPlayer[PLAYER_1].fWidth * 0.7f + WOOD_WIDTH;
	}
	else if (g_aPlayer[PLAYER_1].pos.x + g_aPlayer[PLAYER_1].fWidth * 0.7f >= SCREEN_WIDTH - WOOD_WIDTH)
	{// 一番右に到達したとき処理
		g_aPlayer[PLAYER_1].pos.x = SCREEN_WIDTH - WOOD_WIDTH - g_aPlayer[PLAYER_1].fWidth * 0.7f;
	}

	// 移動量の減衰(慣性)
	g_aPlayer[PLAYER_1].move.x += (0.0f - g_aPlayer[PLAYER_1].move.x) * 0.3f;

	// 頂点座標の更新
	SetVertexPlayer(PLAYER_1);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// マルチモードのプレイヤー移動処理
//-----------------------------------------------------------------
void MultiMovePlayer(int nCntPlayer)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 変数宣言
	bool *pStickPress = GetStickPress(nCntPlayer);
	bool *pCrossKeyPress = GetCrossKeyPress(nCntPlayer);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// プレイヤーの移動
	if (pStickPress[STICKPRESS_RIGHT] == true || pCrossKeyPress[CROSSKEYPRESS_RIGHT] == true)
	{// 右移動
		g_aPlayer[nCntPlayer].move.x += MOVE_PLAYER;
		g_aPlayer[nCntPlayer].nCntAnim++;
		g_aPlayer[nCntPlayer].fV = 0.0f;
		g_aPlayer[nCntPlayer].bMove = true;

		// 砂埃
		SetMoveOrbit(D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight, 0.0f),
			g_aPlayer[nCntPlayer].move, SETPARTICLE_MOVE_PLAYER);
	}
	else if (pStickPress[STICKPRESS_LEFT] == true || pCrossKeyPress[CROSSKEYPRESS_LEFT] == true)
	{// 左移動
		g_aPlayer[nCntPlayer].move.x -= MOVE_PLAYER;
		g_aPlayer[nCntPlayer].nCntAnim++;
		g_aPlayer[nCntPlayer].fV = 0.5f;
		g_aPlayer[nCntPlayer].bMove = true;

		// 砂埃
		SetMoveOrbit(D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight, 0.0f),
			g_aPlayer[nCntPlayer].move, SETPARTICLE_MOVE_PLAYER);
	}

	// 移動をやめたとき
	if (pStickPress[STICKPRESS_RIGHT] == false && pStickPress[STICKPRESS_LEFT] == false && 
		pCrossKeyPress[CROSSKEYPRESS_RIGHT] == false && pCrossKeyPress[CROSSKEYPRESS_LEFT] == false)
	{// 動いていない状態にする
		g_aPlayer[nCntPlayer].bMove = false;
		if (g_aPlayer[nCntPlayer].bJump == false)
		{// 何もしていないときアニメーションNO.0にする
			g_aPlayer[nCntPlayer].nPatternAnim = 0;
		}
	}
	// ジャンプしたときまたは空中にいるとき
	if (g_aPlayer[nCntPlayer].bJump == false)
	{
		if (GetJoypadTrigger(nCntPlayer,JPINFO_OKorJAMP) == true)
		{
			// サウンド
			PlaySound(SOUND_LABEL_SE_JUMP);

			g_aPlayer[nCntPlayer].move.y -= JAMP_MOVE;
			g_aPlayer[nCntPlayer].nPatternAnim = 1;
			g_aPlayer[nCntPlayer].bJump = true;
		}
		if (g_aPlayer[nCntPlayer].move.y != 0.0f)
		{
			g_aPlayer[nCntPlayer].bJump = true;
			g_aPlayer[nCntPlayer].nPatternAnim = 1;
		}
	}
	// 重力の設定
	g_aPlayer[nCntPlayer].move.y += GRAVITY;

	// 位置更新
	g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
	g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].move.y;

	// ブロックとの当たり判定
	HitBlock(nCntPlayer);

	// プレイヤーとの当たり判定
	CollisionPlayer(nCntPlayer);

	// 画面外に出ないようにする
	if (g_aPlayer[nCntPlayer].pos.y - g_aPlayer[nCntPlayer].fHeight >= 580.0f)
	{// 穴のある座標の高さにあるとき
		if (g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f <= 50.0f)
		{// 左側の穴にいるとき左側の木の上から出る
			g_aPlayer[nCntPlayer].pos.x = g_aPlayer[nCntPlayer].fWidth * 0.7f + 50.0f;
			g_aPlayer[nCntPlayer].pos.y = 270.0f;
		}
		else if (g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f >= SCREEN_WIDTH - 50.0f)
		{// 右側の穴にいるとき右側の木の上から出る
			g_aPlayer[nCntPlayer].pos.x = SCREEN_WIDTH - 50.0f - g_aPlayer[nCntPlayer].fWidth * 0.7f ;
			g_aPlayer[nCntPlayer].pos.y = 270.0f;
		}
	}
	else if (g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f <= WOOD_WIDTH)
	{// 一番左に到達したとき処理
		g_aPlayer[nCntPlayer].pos.x = g_aPlayer[nCntPlayer].fWidth * 0.7f + WOOD_WIDTH;
	}
	else if (g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth * 0.7f >= SCREEN_WIDTH - WOOD_WIDTH)
	{// 一番右に到達したとき処理
		g_aPlayer[nCntPlayer].pos.x = SCREEN_WIDTH - WOOD_WIDTH - g_aPlayer[nCntPlayer].fWidth * 0.7f;
	}

	// 移動量の減衰(慣性)
	g_aPlayer[nCntPlayer].move.x += (0.0f - g_aPlayer[nCntPlayer].move.x) * 0.3f;

	// 頂点座標の更新
	SetVertexPlayer(nCntPlayer);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// プレイヤーのアニメーション
//-----------------------------------------------------------------
void AnimationPlayer(int nCntPlayer)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCntPlayer * 4;

	if (0 == g_aPlayer[nCntPlayer].nCntAnim % 5 && g_aPlayer[nCntPlayer].bJump == false && g_aPlayer[nCntPlayer].bMove == true)
	{
		g_aPlayer[nCntPlayer].nPatternAnim = (g_aPlayer[nCntPlayer].nPatternAnim + 1) % TEXTURE_X;
	}
	// アニメーションの更新
	pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V + g_aPlayer[nCntPlayer].fV);
	pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f + g_aPlayer[nCntPlayer].fV);
	pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V + g_aPlayer[nCntPlayer].fV);
	pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f + g_aPlayer[nCntPlayer].fV);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// ブロックの当たり判定
//-----------------------------------------------------------------
void HitBlock(int nCntPlayer)
{
	// 変数宣言
	bool bLand;			// ブロックに乗っているか

	// ブロックとの当たり判定
	bLand = CollisionBlock(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move,
		g_aPlayer[nCntPlayer].fWidth * 0.7f, g_aPlayer[nCntPlayer].fHeight,COLLISIONINFO_PLAYER);

	// ブロックに乗っているときジャンプできるようにする
	if (bLand == true)
	{
		g_aPlayer[nCntPlayer].move.y = 0.0f;
		g_aPlayer[nCntPlayer].bJump = false;
	}
}

//-----------------------------------------------------------------
// プレイヤーの当たり判定
//-----------------------------------------------------------------
void HitPlayer(int nCntPlayer, float fAngle)
{
	// ノックバックの設定
	g_aPlayer[nCntPlayer].move.x += 10.0f * sinf(fAngle);
	g_aPlayer[nCntPlayer].move.y -= 15.0f;

	// プレイヤー状態を点滅にする
	g_aPlayer[nCntPlayer].state = PLAYERSTATE_FLASHING;

	// 状態カウンタの設定
	g_aPlayer[nCntPlayer].nCounterState = 150;

	// サウンド
	PlaySound(SOUND_LABEL_SE_DAMAGE);
}

//-----------------------------------------------------------------
// プレイヤー同士の当たり判定
//-----------------------------------------------------------------
void CollisionPlayer(int nCntPlayer)
{
	// プレイヤー同士の当たり判定
	for (int nCntCollisionPlayer = 0; nCntCollisionPlayer < g_nCntPlayer; nCntCollisionPlayer++)
	{
		if (nCntPlayer != nCntCollisionPlayer)
		{// 自分以外のプレイヤーのとき
			if ((g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f) < (g_aPlayer[nCntCollisionPlayer].pos.x + g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f) &&
				(g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth * 0.7f) > (g_aPlayer[nCntCollisionPlayer].pos.x - g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
			{// プレイヤーが他のプレイヤーの上にいるとき
				if (g_aPlayer[nCntPlayer].posOld.y + g_aPlayer[nCntPlayer].fHeight <= g_aPlayer[nCntCollisionPlayer].posOld.y - g_aPlayer[nCntCollisionPlayer].fHeight)
				{// プレイヤーが他のプレイヤーの上にいるとき
					if (g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight > g_aPlayer[nCntCollisionPlayer].pos.y - g_aPlayer[nCntCollisionPlayer].fHeight)
					{// めり込んだら処理
						g_aPlayer[nCntPlayer].pos.y = g_aPlayer[nCntCollisionPlayer].pos.y - g_aPlayer[nCntCollisionPlayer].fHeight - g_aPlayer[nCntPlayer].fHeight;
						g_aPlayer[nCntCollisionPlayer].move.y = 0.0f;
						g_aPlayer[nCntPlayer].move.y -= 10.0f;

						// サウンド
						PlaySound(SOUND_LABEL_SE_STOMP);

						if (g_aPlayer[nCntCollisionPlayer].fHeight == PLAYER_SIZE_HEIGHT / 2.0f)
						{// 踏まれたプレイヤーのサイズを小さくする
							g_aPlayer[nCntCollisionPlayer].fHeight /= 1.25f;
							g_aPlayer[nCntCollisionPlayer].nCounterSize = 300;
						}
					}
				}
			}
			if ((g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight) > (g_aPlayer[nCntCollisionPlayer].pos.y - g_aPlayer[nCntCollisionPlayer].fHeight) &&
				(g_aPlayer[nCntPlayer].pos.y - g_aPlayer[nCntPlayer].fHeight) < (g_aPlayer[nCntCollisionPlayer].pos.y + g_aPlayer[nCntCollisionPlayer].fHeight))
			{// プレイヤーが他のプレイヤーの横にいるとき
				if ((g_aPlayer[nCntPlayer].posOld.x + g_aPlayer[nCntPlayer].fWidth * 0.7f) <= (g_aPlayer[nCntCollisionPlayer].posOld.x - g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
				{// プレイヤーが他のプレイヤーの左側にいるとき
					if ((g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth * 0.7f) > (g_aPlayer[nCntCollisionPlayer].pos.x - g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
					{// めり込んだら他のプレイヤーの左の座標にする
						g_aPlayer[nCntCollisionPlayer].pos.x += (g_aPlayer[nCntPlayer].move.x / 2.0f) - (g_aPlayer[nCntCollisionPlayer].move.x / 2.0f);
						g_aPlayer[nCntPlayer].pos.x = g_aPlayer[nCntCollisionPlayer].pos.x - (g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f) - (g_aPlayer[nCntPlayer].fWidth * 0.7f);
					}
				}
				else if ((g_aPlayer[nCntPlayer].posOld.x - g_aPlayer[nCntPlayer].fWidth * 0.7f) >= (g_aPlayer[nCntCollisionPlayer].posOld.x + g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
				{// プレイヤーが他のプレイヤーの右側にいるとき
					if ((g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth * 0.7f) < (g_aPlayer[nCntCollisionPlayer].pos.x + g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f))
					{// めり込んだら他のプレイヤーの右の座標にする
						g_aPlayer[nCntCollisionPlayer].pos.x += (g_aPlayer[nCntPlayer].move.x / 2.0f) - (g_aPlayer[nCntCollisionPlayer].move.x / 2.0f);
						g_aPlayer[nCntPlayer].pos.x = g_aPlayer[nCntCollisionPlayer].pos.x + (g_aPlayer[nCntCollisionPlayer].fWidth * 0.7f) + (g_aPlayer[nCntPlayer].fWidth * 0.7f);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------
// プレイヤー状態が通常の時
//-----------------------------------------------------------------
void PlayerNormalProcessing(int nCntPlayer)
{
	// 変数宣言
	MODE mode = GetMode();					// ゲームモードの取得
	PLAYMODE playmode = GetPlayMode();		// プレイモードの取得

	// アニメーションの更新
	AnimationPlayer(nCntPlayer);

	// 現在の位置情報を保存
	g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

	if (mode == MODE_GAME)
	{// 移動処理
		switch (playmode)
		{// シングルモードのとき処理
		case PLAYMODE_SINGLE:
			SingleMovePlayer();
			break;
		// マルチモードのとき処理
		case PLAYMODE_MULTI:
			MultiMovePlayer(nCntPlayer);
			break;
		}		
	}
}

//-----------------------------------------------------------------
// プレイヤー状態が点滅の時
//-----------------------------------------------------------------
void PlayerStateFlashing(int nCntPlayer)
{
	// 状態カウンタの減少
	g_aPlayer[nCntPlayer].nCounterState--;

	// 表示カウンタの加算
	g_aPlayer[nCntPlayer].nCounterDisp++;

	if (g_aPlayer[nCntPlayer].nCounterState <= 0)
	{// 状態カウンタが0以下のとき状態を通常に変更
		g_aPlayer[nCntPlayer].state = PLAYERSTATE_NORMAL;

		// 表示カウントの初期化
		g_aPlayer[nCntPlayer].nCounterDisp = 0;
	}
	else
	{// 状態カウンタが0より大きいとき
		if (5 == g_aPlayer[nCntPlayer].nCounterDisp % 10)
		{// 表示カウンタが5のときα値を1.0fに設定
			g_aPlayer[nCntPlayer].col.a = 1.0f;
		}
		else if (0 == g_aPlayer[nCntPlayer].nCounterDisp % 10)
		{// 表示カウンタが10のときα値を0.2fに設定
			g_aPlayer[nCntPlayer].col.a = 0.2f;
		}
	}
}

//-----------------------------------------------------------------
// プレイヤー状態が縮んでいるとき
//-----------------------------------------------------------------
void PlayerStateShrink(int nCntPlayer)
{
	// 状態カウンタの減少
	g_aPlayer[nCntPlayer].nCounterSize--;

	if (g_aPlayer[nCntPlayer].nCounterSize <= 0)
	{// 表示カウントの初期化
		g_aPlayer[nCntPlayer].nCounterSize = 0;

		// 少し浮かせる
		g_aPlayer[nCntPlayer].move.y -= 5.0f;

		// 高さを初期化する
		g_aPlayer[nCntPlayer].fHeight = PLAYER_SIZE_HEIGHT / 2.0f;
	}
}

//-----------------------------------------------------------------
// リザルト画面のプレイヤー
//-----------------------------------------------------------------
void ResultPlayer(int nCntPlayer)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 変数宣言
	float fmove = GetMoveGauge(nCntPlayer);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nCntPlayer * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth, g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight - fmove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x - g_aPlayer[nCntPlayer].fWidth, g_aPlayer[nCntPlayer].pos.y - g_aPlayer[nCntPlayer].fHeight - fmove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth, g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].fHeight - fmove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].fWidth, g_aPlayer[nCntPlayer].pos.y - g_aPlayer[nCntPlayer].fHeight - fmove, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-----------------------------------------------------------------
// プレイヤー情報の取得
//-----------------------------------------------------------------
PLAYER *GetPlayer(void)
{
	return &g_aPlayer[0];
}

//-----------------------------------------------------------------
// プレイヤー数の獲得
//-----------------------------------------------------------------
int GetPlayerCnt(void)
{
	return g_nCntPlayer;
}