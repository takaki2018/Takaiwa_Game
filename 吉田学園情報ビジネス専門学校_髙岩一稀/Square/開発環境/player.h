//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene2D.h"

//*****************************************************************************
// プレイヤークラス(派生クラス)
//*****************************************************************************
class CPlayer : public CScene2D
{
public:
	// プレイヤーの状態
	typedef enum
	{
		PLAYERSTATE_NORMAL = 0,		// 通常
		PLAYERSTATE_INVINCIBLE,		// 無敵
		PLAYERSTATE_HIT,			// 弾や敵が当たった
		PLAYERSTATE_MAX,
	}PlayerState;

	CPlayer(int nPriority);
	~CPlayer();

	static HRESULT Load(const char *TextureName);
	static void Unload(void);
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	PlayerState GetPlayerState(void) { return m_playerState; }
	void HitPlayer(void);											// プレイヤーが当たったとき処理

private:
	void Move(void);												// 移動処理
	void Shot(void);												// 弾発射処理
	D3DXVECTOR3 CollitionTile(D3DXVECTOR3 pos,D3DXVECTOR2 size);	// タイルとのあたり判定
	bool CollitionEnemy(void);										// 敵との当たり判定
	bool CollitionBullet(void);										// 弾との当たり判定
	void CommonProcess(void);										// 共通処理
	void StateNormal(void);											// 通常状態のとき
	void StateHit(void);											// オブジェクトと当たった状態のとき
	void StateInvincible(void);										// 無敵状態のとき

	static LPDIRECT3DTEXTURE9 m_pTexture;							// テクスチャ情報のポインタ
	D3DXVECTOR3 m_posOld;											// 前回の位置
	D3DXVECTOR3 m_move;												// 移動量
	PlayerState m_playerState;										// プレイヤーの状態
	int m_nCntState;												// 状態カウンタ
	int m_nCntDisp;													// 表示カウンタ
};
#endif