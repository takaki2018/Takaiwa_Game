//==============================================================================================================
//
// セレクトモード (selectmode.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "press_enter.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define PRESSENTER_X					(SCREEN_WIDTH / 2)	// プレスエンターのX座標
#define PREESENTER_Y					(500.0f)			// プレスエンターのY座標
#define PRESSENTER_WIDTH				(400.0f)			// プレスエンターの幅
#define PREESENTER_HIEGHT				(40.0f)				// プレスエンターの高さ

#define PREESENTER_FLASH_INTERBAL		(50)				// プレスエンターの点滅インターバル
#define PREESENTER_CLEAR_NUMBER			(25)				// プレスエンターの透明
#define PREESENTER_LESS_CLEAR_NUMBER	(0)					// プレスエンターの不透明

#define PREESENTER_FLASH_INTERBAL_HI	(10)				// プレスエンターの点滅インターバル
#define PREESENTER_CLEAR_NUMBER_HI		(5)					// プレスエンターの透明

#define TITLE_TRANSITION_COUNT			(50)				// タイトルから遷移するカウント

//--------------------------------------------------------------------------------------------------------------
// 構造体の定義
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXCOLOR col;				// 色
	float fWidth;				// 幅
	float fHieght;				// 高さ
	int nCntFlash;				// 点滅用カウンタ
	bool bPressingEnter;		// おしたかどうか
	bool bTutorialTransition;	// チュートリアル画面に遷移
}PressEnter;

//--------------------------------------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPressEnter = NULL;	// プレスエンターの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePressEnter = NULL;			// プレスエンターのテクスチャのポインタ
PressEnter g_PressEnter;								// プレスエンター情報

//--------------------------------------------------------------------------------------------------------------
// セレクトモードの初期化処理
// 引数		：void		- 何もなし
// 返り値	：HRESULT	- 頂点バッファを生成できたかどうか返す
//--------------------------------------------------------------------------------------------------------------
HRESULT InitPressEnter(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// セレクトモード情報の初期化
	g_PressEnter.pos = D3DXVECTOR3(PRESSENTER_X, PREESENTER_Y,0.0f);
	g_PressEnter.col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	g_PressEnter.fWidth = PRESSENTER_WIDTH / 2.0f;
	g_PressEnter.fHieght = PREESENTER_HIEGHT / 2.0f;
	g_PressEnter.nCntFlash = 0;
	g_PressEnter.bPressingEnter = false;
	g_PressEnter.bTutorialTransition = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &g_pTexturePressEnter);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPressEnter,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetVertexPressEnter();

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPressEnter->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// プレスエンターの終了処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UninitPressEnter(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffPressEnter != NULL)
	{
		g_pVtxBuffPressEnter->Release();
		g_pVtxBuffPressEnter = NULL;
	}

	// テクスチャの開放
	if (g_pTexturePressEnter != NULL)
	{
		g_pTexturePressEnter->Release();
		g_pTexturePressEnter = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// プレスエンターの更新処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void UpdatePressEnter(void)
{
	// 点滅させるためにカウントを進める
	g_PressEnter.nCntFlash++;

	switch (g_PressEnter.bPressingEnter)
	{
	case true:
		// エンターキーを押した後の処理
		AfterPreesingEnter();
		break;

	case false:
		// エンターキーを押す前の処理
		BeforePressingEnter();
		break;
	}
	// 頂点座標の更新
	SetVertexPressEnter();
}

//--------------------------------------------------------------------------------------------------------------
// プレスエンターの描画処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void DrawPressEnter(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPressEnter, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, g_pTexturePressEnter);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//--------------------------------------------------------------------------------------------------------------
// エンターキーを押す前の処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void BeforePressingEnter(void)
{
	if (PREESENTER_CLEAR_NUMBER == g_PressEnter.nCntFlash % PREESENTER_FLASH_INTERBAL)
	{// 透明
		g_PressEnter.col.a = 0.0f;
	}
	else if (PREESENTER_LESS_CLEAR_NUMBER == g_PressEnter.nCntFlash % PREESENTER_FLASH_INTERBAL)
	{// 不透明
		g_PressEnter.col.a = 1.0f;
	}

	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true)
	{// 押したのでtrueにする
		g_PressEnter.bPressingEnter = true;

		// サウンド
		PlaySound(SOUND_LABEL_SE_OK);
	}
}

//--------------------------------------------------------------------------------------------------------------
// エンターキーを押した後の処理
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void AfterPreesingEnter(void)
{
	if (PREESENTER_CLEAR_NUMBER_HI == g_PressEnter.nCntFlash % PREESENTER_FLASH_INTERBAL_HI)
	{// 透明
		g_PressEnter.col.a = 0.0f;
	}
	else if (PREESENTER_LESS_CLEAR_NUMBER == g_PressEnter.nCntFlash % PREESENTER_FLASH_INTERBAL_HI)
	{// 不透明
		g_PressEnter.col.a = 1.0f;
	}
	// カウントがある程度進んだら遷移
	if (g_PressEnter.nCntFlash >= TITLE_TRANSITION_COUNT)
	{// 次に進んだとき処理
	 // ゲームに遷移する許可を与える
		g_PressEnter.bTutorialTransition = true;
	}
}

//--------------------------------------------------------------------------------------------------------------
// タイトル画面に貼るプレスエンターの頂点座標の設定
// 引数		：void	- 何もなし
// 返り値	：void	- 何も返さない
//--------------------------------------------------------------------------------------------------------------
void SetVertexPressEnter(void)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_PressEnter.pos.x - g_PressEnter.fWidth, g_PressEnter.pos.y + g_PressEnter.fHieght, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_PressEnter.pos.x - g_PressEnter.fWidth, g_PressEnter.pos.y - g_PressEnter.fHieght, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_PressEnter.pos.x + g_PressEnter.fWidth, g_PressEnter.pos.y + g_PressEnter.fHieght, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_PressEnter.pos.x + g_PressEnter.fWidth, g_PressEnter.pos.y - g_PressEnter.fHieght, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = g_PressEnter.col;
	pVtx[1].col = g_PressEnter.col;
	pVtx[2].col = g_PressEnter.col;
	pVtx[3].col = g_PressEnter.col;

	// 頂点バッファをアンロックする
	g_pVtxBuffPressEnter->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// チュートリアル画面に遷移するかどうか渡す
// 引数		：void	- 何もなし
// 返り値	：bool	- チュートリアル画面に遷移するかどうか渡す
//--------------------------------------------------------------------------------------------------------------
bool bGetTutorialTransition(void)
{
	return g_PressEnter.bTutorialTransition;
}