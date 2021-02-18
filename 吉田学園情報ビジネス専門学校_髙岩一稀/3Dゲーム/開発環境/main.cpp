//-----------------------------------------------------------------
//
// ウィンドウの表示 (main.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "ranking.h"
#include "loadscreen.h"
#include "effect.h"
#include "camera.h"
#include "light.h"
#include "Xinput_gamepad.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
#include "shadow.h"
#include "cross_product.h"

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT g_pFont = NULL;				// FPSフォントのポインタ
int g_nCountFPS;						// FPSカウンタ
MODE g_mode = MODE_GAME;

//-----------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ウィンドウプロシージャ
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);						 // 初期化処理
void Uninit(void);																 // 終了処理
void Update(void);																 // 更新処理
void Draw(void);																 // 描画処理
void DrawFPS(void);																 // FPSの描画処理
void DrawInfo(void);															 // 情報の描画処理

//-----------------------------------------------------------------
// メイン関数
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		("%s",CLASS_NAME),             // ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),
	};
	HWND hWnd; // ウィンドウハンドル(識別子)
	MSG msg;   // メッセージを格納する

			   // ウィンドウクラスを登録
	RegisterClassEx(&wcex);

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindowEx(
		0,
		("%s", CLASS_NAME), // ウィンドウクラスの名前
		("%s", WINDOW_NAME), // ウィンドウの名前
		WS_OVERLAPPEDWINDOW, // ウィンドウのスタイル
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right,
		rect.bottom,
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	DWORD dwExecLastTime;	// 処理実行最終時刻
	DWORD dwFPSLastTime;	// FPS計測最終時刻
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwFrameCount;		// フレームカウント

	dwExecLastTime = timeGetTime();
	dwFPSLastTime = timeGetTime();
	dwCurrentTime = timeGetTime();	// 現在時刻の設定
	dwFrameCount = 0;

	// メッセージループ
	while (1)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) // メッセージを取得しなっかた場合0を返す
		{ // windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// メッセージの翻訳を送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{ // DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			// フレームカウントが1000/60秒の時処理
			if((dwCurrentTime - dwExecLastTime) > (1000 / 60))
			{
				// 代入
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				// カウント
				dwFrameCount++;
			}
		}
	}

	// 終了処理
	Uninit();

	// ウィンドウクラスの登録削除
	UnregisterClass(("%s", CLASS_NAME), wcex.hInstance);

	return (int)msg.wParam;
}

//-----------------------------------------------------------------
// ウィンドウプロシージャ
//-----------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: // キーを押したとき表示
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(NULL, "終了しますか?", "確認", MB_YESNO | MB_TOPMOST);
			if (nID == IDYES)
			{
				nID = MessageBox(NULL, "終了します。", "", MB_OK | MB_TOPMOST);
				if (nID == IDOK)
				{
					DestroyWindow(hWnd); // メッセージを返す
				}
			}
			break;
		}
		break;

	case WM_CLOSE: // ウィンドウを閉じようとしたとき表示
		nID = MessageBox(NULL, "終了しますか?", "確認", MB_YESNO | MB_TOPMOST);
		if (nID == IDYES)
		{
			nID = MessageBox(NULL, "終了します。", "", MB_OK | MB_TOPMOST);
			if (nID == IDOK)
			{
				DestroyWindow(hWnd); // メッセージを返す
			}
		}
		break;

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//-----------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドの合成
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ソースの合成方法の設定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// デスティネーションの合成方法の説明

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// 縮小時に補間
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 拡大時に補間
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U値の繰り返し
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V値の繰り返し

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // α値の合成
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
					
	//FPSの初期化処理
	D3DXCreateFont(g_pD3DDevice,18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,		
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// キーボードの初期化処理
	InitKeyboard(hInstance, hWnd);

	// ゲームパッドの初期化処理
	InitGamepad(hInstance, hWnd);

	// サウンドの初期化処理
	InitSound(hWnd);

	// フェードの初期化
	InitFade(g_mode);

	// 画面モードの初期化処理
	SetMode(g_mode);

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	return S_OK;
}

//-----------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------
void Uninit(void)
{
	// ゲームパッドの終了処理
	UninitGamepad();

	// キーボードの終了処理
	UninitKeyboard();

	// フェードの終了処理
	UninitFade();

	// 画面モードの終了処理
	switch (g_mode)
	{// タイトル画面の終了処理
	case MODE_TITLE:
		UninitTitle();
		break;
		// ロード画面の終了処理
	case MODE_LOADSCREEN:
		UninitLoadScreen();
		break;
		// ゲームの終了処理
	case MODE_GAME:
		UninitGame();
		break;
		// リザルト画面の終了処理
	case MODE_RESULT:
		UninitResult();
		break;
		// ランキングの終了処理
	case MODE_RANKING:
		UninitRanking();
		break;
	}

	// サウンドの終了処理
	UninitSound();

	// ライトの終了処理
	UninitLight();

	// カメラの終了処理
	UninitCamera();

	// フォントオブジェクトの開放
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3Dデバイスの開放
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの開放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//-----------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------
void Update(void)
{
	// キーボードの更新処理
	UpdateKeyboard();

	// ゲームパッドの更新処理
	UpdateGamepad();

	// ライトの更新処理
	UpdateLight();

	// カメラの更新処理
	UpdateCamera();

	// 画面モードの更新処理
	switch (g_mode)
	{// タイトル画面の更新処理
	case MODE_TITLE:
		UpdateTitle();
		break;
		// ロード画面の更新処理
	case MODE_LOADSCREEN:
		UpdateLoadScreen();
		break;
		// ゲームの更新処理
	case MODE_GAME:
		UpdateGame();
		break;
		// リザルト画面の更新処理
	case MODE_RESULT:
		UpdateResult();
		break;
		// ランキングの更新処理
	case MODE_RANKING:
		UpdateRanking();
		break;
	}
	// フェードの更新処理
	UpdateFade();
}

//-----------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------
void Draw(void)
{
	// バックバッファとZバッファのクリア
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// 描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// 変数宣言
		D3DVIEWPORT9 viewportDef;

		// 現在のビューポートを保存
		g_pD3DDevice->GetViewport(&viewportDef);

		for (int nCntCameraType = 0; nCntCameraType < CAMERATYPE_MAX; nCntCameraType++)
		{
			// カメラの配置
			SetCamera(nCntCameraType);

			// 画面モードの修了処理
			switch (g_mode)
			{// タイトル画面の描画処理
			case MODE_TITLE:
				DrawTitle();
				break;
				// ロード画面の描画処理
			case MODE_LOADSCREEN:
				DrawLoadScreen();
				break;
				// ゲームの描画処理
			case MODE_GAME:
				DrawGame();
				break;
				// リザルト画面の描画処理
			case MODE_RESULT:
				DrawResult();
				break;
				// ランキングの描画処理
			case MODE_RANKING:
				DrawRanking();
				break;
			}
			// フェードの描画処理
			DrawFade();

#ifdef _DEBUG
			// FPSの描画処理
			DrawFPS();

			// 情報の描画
			DrawInfo();
#endif
		}
		// ビューポートを元に戻す
		g_pD3DDevice->SetViewport(&viewportDef);

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//-----------------------------------------------------------------
// デバイスの取得
//-----------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------------------------------------------------------
// FPSの描画
//-----------------------------------------------------------------
void DrawFPS(void)
{
	// 変数宣言
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	// テキスト
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect,
		DT_RIGHT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//-----------------------------------------------------------------
// 情報の描画処理
//-----------------------------------------------------------------
void DrawInfo(void)
{
	// 変数宣言
	RECT rect = { 5,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1024];

	// それぞれの情報の取得
	Player *pPlayer = GetPlayer();			// モデルの位置情報
	Camera camera = GetCamera();		// カメラ情報
	Shadow *pShadow = GetShadow();
	CrossProduct *pCrossProduct = GetCrossProduct();

	// テキスト
	int nNum = wsprintf(&aStr[0], "+--------------------------+\n");
	nNum += wsprintf(&aStr[nNum], "+      様々な位置情報      +\n");
	nNum += wsprintf(&aStr[nNum], "+--------------------------+\n");

	// モデルの位置情報
	nNum += sprintf(&aStr[nNum], "モーションカウンタ     : (%d / %d)\n", pPlayer->nCounterMotion, pPlayer->aMotionInfo[pPlayer->MotionType].aKeyInfo[pPlayer->nKey].nFrame );
	nNum += sprintf(&aStr[nNum], "キーNo.	             : (%d)\n", pPlayer->aMotionInfo[pPlayer->MotionType].nNumKey);
	nNum += sprintf(&aStr[nNum], "プレイヤーの位置       : (%4.2f,%4.2f,%4.2f)\n", pPlayer->pos.x,pPlayer->pos.y,pPlayer->pos.z);
	nNum += sprintf(&aStr[nNum], "プレイヤーの向き       : (%4.2f)\n",pPlayer->rot.y);
	nNum += sprintf(&aStr[nNum], "プレイヤーの目的の向き : (%4.2f)\n", pPlayer->rotDest.y);
	nNum += sprintf(&aStr[nNum], "親の位置               : (%4.2f,%4.2f,%4.2f)\n", pPlayer->aModel[0].pos.x, pPlayer->aModel[0].pos.y, pPlayer->aModel[0].pos.z);
	nNum += sprintf(&aStr[nNum], "親の向き               : (%4.2f,%4.2f,%4.2f)\n", pPlayer->aModel[0].rot.x, pPlayer->aModel[0].rot.y, pPlayer->aModel[0].rot.z);

	// テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect,
		DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//-----------------------------------------------------------------
// モードの設定
//-----------------------------------------------------------------
void SetMode(MODE mode)
{
	// 画面モードの修了処理
	switch (g_mode)
	{// タイトル画面の終了処理
	case MODE_TITLE:
		UninitTitle();
		break;
		// ロード画面の終了処理
	case MODE_LOADSCREEN:
		UninitLoadScreen();
		break;
		// ゲームの終了処理
	case MODE_GAME:
		UninitGame();
		break;
		// リザルト画面の終了処理
	case MODE_RESULT:
		UninitResult();
		break;
		// ランキングの終了処理
	case MODE_RANKING:
		UninitRanking();
		break;
	}
	// モードの更新
	g_mode = mode;

	// 画面モードの初期化処理
	switch (mode)
	{// タイトル画面の初期化処理
	case MODE_TITLE:
		InitTitle();
		break;
		// ロード画面の初期化処理
	case MODE_LOADSCREEN:
		InitLoadScreen();
		break;
		// ゲームの初期化処理
	case MODE_GAME:
		InitGame();
		break;
		// リザルト画面の初期化処理
	case MODE_RESULT:
		InitResult();
		break;
		// ランキングの初期化処理
	case MODE_RANKING:
		InitRanking();
		break;
	}
}

//-----------------------------------------------------------------
// 画面モードの取得
//-----------------------------------------------------------------
MODE GetMode(void)
{
	return g_mode;
}