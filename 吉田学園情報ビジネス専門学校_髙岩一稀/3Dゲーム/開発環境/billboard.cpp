//-----------------------------------------------------------------
//
// 壁 (wall.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "billboard.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_BILLBOARD	(1)		// 用意するビルボードデータのサイズ
#define WALL_SIZE		(50.0f)		// ビルボードのサイズ

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 pos;			// 壁の中心座標
	float fWidth;
	float fHeight;
	int nIdx;					// 影の番号
	bool bUse;					// 使用しているかどうか
}Billboard;

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;				// 壁の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;					// 壁のテクスチャのポインタ
Billboard g_aBillboard[MAX_BILLBOARD];							// ビルボードの情報

//-----------------------------------------------------------------
// ビルボードの初期化処理
//-----------------------------------------------------------------
HRESULT InitBillboard(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].fWidth = 100.0f;
		g_aBillboard[nCntBillboard].fHeight = 100.0f ;
		g_aBillboard[nCntBillboard].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field002.jpg", &g_pTextureBillboard);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_BILLBOARD,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].pos.x - g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].pos.y + g_aBillboard[nCntBillboard].fHeight, g_aBillboard[nCntBillboard].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].pos.x + g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].pos.y + g_aBillboard[nCntBillboard].fHeight, g_aBillboard[nCntBillboard].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].pos.x - g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].pos.x + g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		// 何番目か
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();

	// ビルボードの設置
	SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 0.0f), WALL_SIZE, WALL_SIZE);

	return S_OK;
}

//-----------------------------------------------------------------
// ビルボードの終了処理
//-----------------------------------------------------------------
void UninitBillboard(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}

	// テクスチャの開放
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
}

//-----------------------------------------------------------------
// ビルボードの更新処理
//-----------------------------------------------------------------
void UpdateBillboard(void)
{
	if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{
		g_aBillboard[0].pos.x -= 5.0f;
	}
}

//-----------------------------------------------------------------
// ビルボードの描画処理
//-----------------------------------------------------------------
void DrawBillboard(void)
{
	// 変数宣言
	Billboard *pBillboard = &g_aBillboard[0];
	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ
	D3DXMATRIX mtxView;							// ビューマトリックス取得用
	D3DXMATRIX mtxScaling,mtxTrans;						// 計算用マトリックス

	// 変数の受け渡し
	pDevice = GetDevice();

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファを適用
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pBillboard->mtxWorld);

#if 0
			// 向きを反映
			D3DXMatrixScaling(&mtxScaling, pBillboard->fWidth, pBillboard->fHeight, 0.0f);
			D3DXMatrixMultiply(&pBillboard->mtxWorld, &pBillboard->mtxWorld, &mtxScaling);
#else
			// 向きを反映
			//pBillboard->mtxWorld._22 = pBillboard->fHeight;
#endif

			// ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 0
			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&pBillboard->mtxWorld, NULL,&mtxView);
			pBillboard->mtxWorld._41 = 0.0f;
			pBillboard->mtxWorld._42 = 0.0f;
			pBillboard->mtxWorld._43 = 0.0f;
#else
			pBillboard->mtxWorld._11 = mtxView._11;
			pBillboard->mtxWorld._12 = mtxView._21;
			pBillboard->mtxWorld._13 = mtxView._31;
			//pBillboard->mtxWorld._21 = mtxView._12;
			//pBillboard->mtxWorld._22 = mtxView._22;
			//pBillboard->mtxWorld._23 = mtxView._32;
			pBillboard->mtxWorld._31 = mtxView._13;
			pBillboard->mtxWorld._32 = mtxView._23;
			pBillboard->mtxWorld._33 = mtxView._33;
#endif
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);
			D3DXMatrixMultiply(&pBillboard->mtxWorld, &pBillboard->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			
			// ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureBillboard);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);
		}
	}
	// 標準に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-----------------------------------------------------------------
// ビルボードの配置
//-----------------------------------------------------------------
void SetBillboard(D3DXVECTOR3 pos,float fWidth,float fDepth)
{
	// 構造体のポインタ変数
	Billboard *pBillboard = &g_aBillboard[0];

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bUse == false)
		{
			// 位置の設定
			pBillboard->pos = pos;

			pBillboard->fWidth = fWidth;

			pBillboard->fHeight = fDepth;

			// 使用状態をtrueにする
			pBillboard->bUse = true;

			break;
		}
	}
}