//=============================================================================
//
// フェードヘッダー (fade.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "manager.h"

//*****************************************************************************
// フェードクラス(基本クラス)
//*****************************************************************************
class CFade
{
public:
	// フェードコマンド
	typedef enum
	{
		FADE_NONE = 0,	// 何もしていない
		FADE_IN,		// フェードイン
		FADE_OUT,		// フェードアウト
		FADE_MAX,
	}FADE;

	HRESULT Init(CManager::MODE modeNext);		
	void Uninit(void);					
	void Update(void);					
	void Draw(void);					
	static void SetFade(FADE fade, CManager::MODE modeNext);
	static FADE GetFade(void);						

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffFade;	// 頂点バッファのポインタ
	static FADE m_fade;						// フェードの状態
	static CManager::MODE m_modeNext;		// 次のモード
	D3DXCOLOR m_colorFade;					// フェード色
};

#endif
