//-----------------------------------------------------------------
//
// 弾 (bullet.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bullet.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "setparticle.h"
#include "explosion.h"
#include "input.h"
#include "gamepad.h"
#include "Xinput_gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_BULLET	(256)	// 用意する弾データのサイズ
#define BULLET_SIZE	(5.0f)	// 弾のサイズ
#define BULLET_MOVE	(10.0f)	// 弾の移動量

//-----------------------------------------------------------------
// 構造体の定義
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 pos;			// 壁の中心座標
	D3DXVECTOR3 rot;			// 壁の中心座標
	D3DXVECTOR3 move;			// 移動量
	float fWidth;				// 幅
	float fHeight;				// 高さ
	int nIdx;					// 影の番号
	bool bUse;					// 使用しているかどうか
}Bullet;

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;			// 壁の頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;				// 壁のテクスチャのポインタ
Bullet g_aBullet[MAX_BULLET];							// ビルボードの情報

//-----------------------------------------------------------------
// 弾の初期化処理
//-----------------------------------------------------------------
HRESULT InitBullet(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = GetDevice();

	for (int nCntBillboard = 0; nCntBillboard < MAX_BULLET; nCntBillboard++)
	{
		g_aBullet[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBillboard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBillboard].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBillboard].fWidth = BULLET_SIZE;
		g_aBullet[nCntBillboard].fHeight = BULLET_SIZE;
		g_aBullet[nCntBillboard].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_BULLET,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// 頂点座標の設定
		SetVertexBullet(nCntBullet);

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
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// 弾の終了処理
//-----------------------------------------------------------------
void UninitBullet(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	// テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
}

//-----------------------------------------------------------------
// 弾の更新処理
//-----------------------------------------------------------------
void UpdateBullet(void)
{
	// 構造体のポインタ変数
	Bullet *pBullet = &g_aBullet[0];
	Player *pPlayer = GetPlayer();
	XinputGamepad *pXinput = GetXinputGamepad();

	/*if (GetKeyboardTrigger(KEYINFO_SHOOT) == true || pXinput->bPressA == true)
	{
		// 弾の発射
		SetBullet(D3DXVECTOR3(pPlayer->pos.x , pPlayer->pos.y + 28.0f, pPlayer->pos.z),
			D3DXVECTOR3(sinf(pPlayer->rot.y) * -BULLET_MOVE,0.0f, cosf(pPlayer->rot.y) * -BULLET_MOVE),
			BULLET_SIZE,
			BULLET_SIZE);
	}*/

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{// 位置更新
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.z += pBullet->move.z;

			// 弾の軌道
			//SetOrbitBullet(pBullet->pos);

			// 影の移動
			SetPositionShadow(pBullet->nIdx, D3DXVECTOR3(pBullet->pos.x, 0.0f, pBullet->pos.z));

			// 画面外チェック
			if (pBullet->pos.z <= -600.0f || pBullet->pos.z >= 600.0f ||
				pBullet->pos.x <= -600.0f || pBullet->pos.x >= 600.0f)
			{// 弾の状態をfalseにする
				pBullet->bUse = false;

				// 爆発アニメーションの配置
				SetExplosion(pBullet->pos);

				// 影をなくす
				DeleteShadow(pBullet->nIdx);
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//-----------------------------------------------------------------
// 弾の描画処理
//-----------------------------------------------------------------
void DrawBullet(void)
{
	// 変数宣言
	Bullet *pBullet = &g_aBullet[0];
	LPDIRECT3DDEVICE9 pDevice;					// デバイスへのポインタ
	D3DXMATRIX mtxView;							// ビューマトリックス取得用
	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス

	// 変数の受け渡し
	pDevice = GetDevice();

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 120);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pBullet->mtxWorld);

			// ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 0
			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&pBillboard->mtxWorld, NULL, &mtxView);
			pBillboard->mtxWorld._41 = 0.0f;
			pBillboard->mtxWorld._42 = 0.0f;
			pBillboard->mtxWorld._43 = 0.0f;
#else
			pBullet->mtxWorld._11 = mtxView._11;
			pBullet->mtxWorld._12 = mtxView._21;
			pBullet->mtxWorld._13 = mtxView._31;
			pBullet->mtxWorld._21 = mtxView._12;
			pBullet->mtxWorld._22 = mtxView._22;
			pBullet->mtxWorld._23 = mtxView._32;
			pBullet->mtxWorld._31 = mtxView._13;
			pBullet->mtxWorld._32 = mtxView._23;
			pBullet->mtxWorld._33 = mtxView._33;
#endif

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pBullet->pos.x, pBullet->pos.y, pBullet->pos.z);
			D3DXMatrixMultiply(&pBullet->mtxWorld, &pBullet->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pBullet->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
	// アルファテストを元の設定に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-----------------------------------------------------------------
// 弾の配置
//-----------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight)
{
	// 構造体のポインタ変数
	Bullet *pBullet = &g_aBullet[0];

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			// 位置の設定
			pBullet->pos = pos;

			// 移動量の設定
			pBullet->move = move;

			// 幅の設定
			pBullet->fWidth = fWidth;

			// 高さの設定
			pBullet->fHeight = fHeight;

			// 使用状態をtrueにする
			pBullet->bUse = true;

			// 影を付ける
			pBullet->nIdx = SetShadow(D3DXVECTOR3(pos.x,0.0f,pos.z),BULLET_SIZE, BULLET_SIZE);

			break;
		}
	}
}

//-----------------------------------------------------------------
// 弾の頂点座標	
//-----------------------------------------------------------------
void SetVertexBullet(int nIdx)
{
	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// 何番目か
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - g_aBullet[nIdx].fWidth, g_aBullet[nIdx].pos.y + g_aBullet[nIdx].fHeight, g_aBullet[nIdx].pos.z);
	pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + g_aBullet[nIdx].fWidth, g_aBullet[nIdx].pos.y + g_aBullet[nIdx].fHeight, g_aBullet[nIdx].pos.z);
	pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - g_aBullet[nIdx].fWidth, g_aBullet[nIdx].pos.y - g_aBullet[nIdx].fHeight, g_aBullet[nIdx].pos.z);
	pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + g_aBullet[nIdx].fWidth, g_aBullet[nIdx].pos.y - g_aBullet[nIdx].fHeight, g_aBullet[nIdx].pos.z);

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}