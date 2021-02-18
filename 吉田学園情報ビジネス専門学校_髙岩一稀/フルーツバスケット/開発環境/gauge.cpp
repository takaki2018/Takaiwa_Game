//-----------------------------------------------------------------
//
// ゲージ (gauge.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "gauge.h"
#include "selectmode.h"
#include "player.h"
#include "score.h"
#include "result_score.h"
#include "rank_logo.h"
#include "sound.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN		(4)				// 種類

#define GAUGE_CENTER_X	(365.0f)		// ゲージの中心座標(X)
#define GAUGE_CENTER_Y	(690.0f)		// ゲージの中心座標(Y)

#define GAUGE_X			(100.0f)		// ゲージの幅

#define PlAYER_WIDTH	(180.0f)		// プレイヤーを配置する間隔

#define RANK_LOGO_WIDTH		(90.0f)			// ランクロゴの幅
#define RANK_LOGO_HEIGHT	(55.0f)			// ランクロゴの高さ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;			// ゲージの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureGauge[MAX_PATTERN] = {};	// ゲージのテクスチャのポインタ
GAUGE g_aGauge[MAX_PATTERN];							// ゲージ情報
int g_nDisplayGauge;									// 何個あるか
int g_nCntGauge;										// ゲージを伸ばす間隔のカウント									 
bool g_bSoundResult;									// リザルトのBGMがなったかどうか
int g_nCntGaugeTimer;									// リザルトのBGMを流すカウント
bool g_bSkipResult;										// リザルトを飛ばしたかどうか

//-----------------------------------------------------------------
// ゲージの初期化処理
//-----------------------------------------------------------------
HRESULT InitGauge(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();
	g_nDisplayGauge = GetPlayerCnt();
	g_bSoundResult = false;
	g_nCntGaugeTimer = 0;
	g_bSkipResult = false;

	// 変数の初期化
	for (int nCntBar = 0; nCntBar < MAX_PATTERN; nCntBar++)
	{
		g_aGauge[nCntBar].pos = D3DXVECTOR3(GAUGE_CENTER_X + PlAYER_WIDTH * nCntBar, GAUGE_CENTER_Y, 0.0f);
		g_aGauge[nCntBar].fMoveHeight = 0.0f;
		g_aGauge[nCntBar].nRank = RANK_1;
		g_aGauge[nCntBar].nCntScore = 0;
		g_aGauge[nCntBar].bReach = false;
	}
	// 順位の確定
	RankingGauge();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge_player_1.png", &g_pTextureGauge[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge_player_2.png", &g_pTextureGauge[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge_player_3.png", &g_pTextureGauge[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gauge_player_4.png", &g_pTextureGauge[3]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x - GAUGE_X / 2, g_aGauge[nCnt].pos.y , 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x - GAUGE_X / 2, g_aGauge[nCnt].pos.y - g_aGauge[nCnt].fMoveHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x + GAUGE_X / 2, g_aGauge[nCnt].pos.y , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x + GAUGE_X / 2, g_aGauge[nCnt].pos.y - g_aGauge[nCnt].fMoveHeight, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// ゲージの終了処理
//-----------------------------------------------------------------
void UninitGauge(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		if (g_pTextureGauge[nCnt] != NULL)
		{
			g_pTextureGauge[nCnt]->Release();
			g_pTextureGauge[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ゲージの更新処理
//-----------------------------------------------------------------
void UpdateGauge(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;
	PLAYMODE playmode = GetPlayMode();

	// 画面遷移
	if (GetKeyboardTrigger(KEYINFO_OK) == true || 
		GetJoypadTrigger(PLAYER_1, JPINFO_OKorJAMP) == true)
	{
		g_bSkipResult = true;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCntGaugeTimer != 0)
	{// ゲージが伸びるカウント
		g_nCntGaugeTimer--;

		if (g_bSoundResult == false)
		{// すべてのプレイヤーのゲージが伸びきったとき
			//サウンドの一時停止
			StopSound(SOUND_LABEL_SE_DORAM);

			// サウンド
			PlaySound(SOUND_LABEL_RESULT);
			g_bSoundResult = true;
		}
	}

	// カウントの加算
	g_nCntGauge++;

	for (int nCnt = 0; nCnt < g_nDisplayGauge; nCnt++)
	{
		// 変数宣言
		int nScore = GetScore(nCnt);			// 各プレイヤーのスコア

		if (nScore > g_aGauge[nCnt].nCntScore)
		{
			// 高さの更新
			g_aGauge[nCnt].fMoveHeight += 1.0f;

			// スコアの更新
			if (0 == g_nCntGauge % 5)
			{// スコアの加算
				g_aGauge[nCnt].nCntScore++;		
				AddResultScore(nCnt, 1);
			}
			if (g_bSkipResult == true)
			{// 変数宣言
				int nScoreRemaining;			// 必要な残りスコアを保存するための変数

				nScoreRemaining = nScore - g_aGauge[nCnt].nCntScore;			// 残りスコアの計算
				g_aGauge[nCnt].nCntScore = nScore;								// プレイヤーのスコアを保存
				g_aGauge[nCnt].fMoveHeight += 5.0f * (float)nScoreRemaining;	// 残りスコアの分だけゲージを伸ばす
				AddResultScore(nCnt, nScoreRemaining);							// 残りスコア分足す
			}
			if (g_aGauge[nCnt].fMoveHeight > 300.0f)
			{// ゲージの高さ上限
				g_aGauge[nCnt].fMoveHeight = 300.0f;
			}
		}
		else if (nScore == g_aGauge[nCnt].nCntScore && g_aGauge[nCnt].bReach == false)
		{// 自分のスコアに到達したら処理
			g_aGauge[nCnt].bReach = true;							// スコアに到達

			if (playmode == PLAYMODE_MULTI)
			{
				SetRankLogo(D3DXVECTOR3(g_aGauge[nCnt].pos.x, g_aGauge[nCnt].pos.y, g_aGauge[nCnt].pos.z),
					RANK_LOGO_WIDTH, RANK_LOGO_HEIGHT, (RANK)g_aGauge[nCnt].nRank);	// 何位か設置
			}

			switch (g_aGauge[nCnt].nRank)
			{// 一位
			case RANK_1:
				// サウンド
				PlaySound(SOUND_LABEL_SE_RANK_1);

				// 変数の初期化
				g_nCntGaugeTimer = 30;
				break;
			// 二位
			case RANK_2:
				// サウンド
				PlaySound(SOUND_LABEL_SE_RANK_2);
				break;
			// 三位
			case RANK_3:
				// サウンド
				PlaySound(SOUND_LABEL_SE_RANK_3);
				break;
			// 四位
			case RANK_4:
				// サウンド
				PlaySound(SOUND_LABEL_SE_RANK_4);
				break;
			}
		}

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x - GAUGE_X / 2, g_aGauge[nCnt].pos.y , 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x - GAUGE_X / 2, g_aGauge[nCnt].pos.y - g_aGauge[nCnt].fMoveHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x + GAUGE_X / 2, g_aGauge[nCnt].pos.y , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGauge[nCnt].pos.x + GAUGE_X / 2, g_aGauge[nCnt].pos.y - g_aGauge[nCnt].fMoveHeight , 0.0f);

		// 次の情報
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

//-----------------------------------------------------------------
// ゲージの描画処理
//-----------------------------------------------------------------
void DrawGauge(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_PATTERN; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureGauge[nCnt]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//-----------------------------------------------------------------
// ランキングの確定
//-----------------------------------------------------------------
void RankingGauge(void)
{
	// スコアの確定
	for (int nCnt = 0; nCnt < g_nDisplayGauge; nCnt++)
	{
		// 変数宣言
		int nScore = GetScore(nCnt);

		// スコアを保存
		g_aGauge[nCnt].nScore = nScore;
	}
	
	// 順位の確定
	for (int nCnt = 0; nCnt < g_nDisplayGauge; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < g_nDisplayGauge; nCnt2++)
		{
			if (nCnt != nCnt2)
			{
				if (g_aGauge[nCnt].nScore < g_aGauge[nCnt2].nScore)
				{// ランキングスコア更新したとき
					g_aGauge[nCnt].nRank++;
				}
			}
		}
	}
}


//-----------------------------------------------------------------
// ゲージの描画処理
//-----------------------------------------------------------------
float GetMoveGauge(int nCnt)
{
	return g_aGauge[nCnt].fMoveHeight;
}

//-----------------------------------------------------------------
// ゲージが上がりきったかどうか
//-----------------------------------------------------------------
bool GetbGauge(void)
{
	return g_bSoundResult;
}