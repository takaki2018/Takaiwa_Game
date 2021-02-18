//-----------------------------------------------------------------
//
// ライト (light.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "light.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_LIGHT		(3)				// ライトの最大数

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
D3DLIGHT9 g_light[MAX_LIGHT];			// ライト情報

//-----------------------------------------------------------------
// ライトの初期化処理
//-----------------------------------------------------------------
void InitLight(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
	D3DXVECTOR3 vecDir;		// 設定用方向ベクトル

	// 変数の受け渡し
	pDevice = GetDevice();

	// ライトをクリアする
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの方向を設定
		switch (nCntLight)
		{
		case 0:
			// ライトの拡散光の設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
			D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
			g_light[nCntLight].Direction = vecDir;
			break;

		case 1:
			// ライトの拡散光の設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			vecDir = D3DXVECTOR3(-0.2f, 0.8f, -0.4f);
			D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
			g_light[nCntLight].Direction = vecDir;
			break;

		case 2:
			// ライトの拡散光の設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			vecDir = D3DXVECTOR3(0.9f, -0.1f, 0.4f);
			D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
			g_light[nCntLight].Direction = vecDir;
			break;
		}
		// ライトを設定
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//-----------------------------------------------------------------
// ライトの終了処理
//-----------------------------------------------------------------
void UninitLight(void)
{

}

//-----------------------------------------------------------------
// ライトの更新処理
//-----------------------------------------------------------------
void UpdateLight(void)
{

}