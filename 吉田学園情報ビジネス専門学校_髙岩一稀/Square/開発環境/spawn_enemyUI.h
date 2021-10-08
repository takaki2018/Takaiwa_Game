//=============================================================================
//
// 敵の発生UI処理 [spawn_enemyUI.h]
// Author : 
//
//=============================================================================
#ifndef _SPAWNENEMYUI_H_
#define _SPAWNENEMYUI_H_

#include "scene2D.h"

//-----------------------------------------------------------------------------
// 敵発生クラス(派生クラス)
//------------------ ----------------------------------------------------------
class CSpawnEnemyUI : public CScene2D
{
public:
	CSpawnEnemyUI(int nPriority);
	~CSpawnEnemyUI();
	static HRESULT Load(void);
	static void Unload(void);
	static CSpawnEnemyUI *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI,int nCntSpawnInterval,int nEnemyType);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fMoveAI, int nCntSpawnInterval, int nEnemyType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Blinking(void);							// 点滅処理
	void CreateEnemy(void);							// 敵の生成
	static LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャ情報のポインタ
	int m_nCntSpawnInterval;						// 敵が発生するまでの時間
	int m_nCounter;									// カウンター
	float m_fMoveAlpha;								// α値移動の基準値
	int m_nCntFlash;								// 点滅回数
	int m_nEnemyType;								// 敵の種類識別用
	float m_fMoveAI;								// 基本移動量
};
#endif