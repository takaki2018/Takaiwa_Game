//=============================================================================
//
// ロゴ処理 [bg.h]
// Author : 
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "scene.h"

//-----------------------------------------------------------------------------
// ロゴクラス(基本クラス)
//-----------------------------------------------------------------------------
class CLogo
{
public:
	// テクスチャの種類
	typedef enum
	{
		TEXTURETYPE_TITLELOGO = 0,	// タイトルロゴ
		TEXTURETYPE_PLAY,			// プレイロゴ
		TEXTURETYPE_TUTORIAL,		// チュートリアルロゴ
		TEXTURETYPE_RANKING,		// ランキングロゴ
		TEXTURETYPE_OPTION,			// オプションロゴ
		TEXTURETYPE_SCORE,			// スコアロゴ
		TEXTURETYPE_HIGHSCORE,		// ハイスコアロゴ
		TEXTURETYPE_PRESSENTER,		// PressEnter
		TEXTURETYPE_GAMESTART,		// GameStartロゴ
		TEXTURETYPE_GAMECLEAR,		// GameClearロゴ
		TEXTURETYPE_GAMEOVER,		// GameOverロゴ
		TEXTURETYPE_RESULTLOGO,		// リザルトロゴ
		TEXTURETYPE_ARCADEMODE,		// ArcadeModeロゴ
		TEXTURETYPE_YOURSCORE,		// YourScoreロゴ
		TEXTURETYPE_NEWRECORD,		// NewRecordロゴ
		TEXTURETYPE_MAXCOMBO,		// 最大コンボロゴ
		TEXTURETYPE_KILLENEMY,		// 敵撃破数ロゴ
		TEXTURETYPE_MAX,
	}TEXTURETYPE_LOGO;

	CLogo();
	~CLogo();
	static HRESULT Load(void);
	static void Unload(void);
	static CLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTextureNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTextureNumber);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void StartBlinking(int nBlimkingInterval);
	void SetPosition(D3DXVECTOR3 pos);
	void SetCol(D3DXCOLOR col);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR2 GetSize(void) { return m_size; }

private:
	void Blinking(void);										// 点滅処理
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURETYPE_MAX];		// テクスチャ情報のポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// 頂点バッファのポインタ
	D3DXVECTOR3 m_pos;											// 位置情報
	D3DXVECTOR2 m_size;											// サイズ情報
	int m_nTextureNumber;										// テクスチャ識別番号
	bool m_bBlinking;											// 点滅するかどうか
	int m_nBlimkingInterval;									// 点滅間隔
	int m_nBlinkingCounter;										// 点滅カウント
};
#endif