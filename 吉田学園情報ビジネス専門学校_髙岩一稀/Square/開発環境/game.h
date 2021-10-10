//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;				// プレイヤークラス
class CUI;					// UIクラス
class CSpawnEnemy;			// 敵発生クラス
class CGameClear;			// ゲームクリアクラス
class CModel;				// モデルクラス

//-----------------------------------------------------------------------------
// ゲーム画面クラス(派生クラス)
//-----------------------------------------------------------------------------
class CGame : public CScene
{
public:
	CGame();
	~CGame()
		;
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *GetPlayer(void);
	static CUI *GetUI(void);
	static CModel *GetModel(void) { return m_pModel; }

private:
	void TileFrameEffectCreate(void);			// タイルフレームエフェクトの生成
	static CPlayer *m_pPlayer;					// プレイヤー情報のポインタ
	static CUI *m_pUI;							// UI情報のポインタ
	static CSpawnEnemy  *m_pSpawnEnemy;			// 敵発生情報のポインタ
	static CGameClear *m_pGameClear;			// ゲームクリア情報のポインタ
	static CModel *m_pModel;					// モデル情報のポインタ
	static int m_nCntTileFrameEffect;			// タイルフレームエフェクトを出すカウンター
};
#endif