//==============================================================================================================
//
// ポーズヘッダー (pause.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "scene.h"

//--------------------------------------------------------------------------------------------------------------
// 前方宣言
//--------------------------------------------------------------------------------------------------------------
class CScene2D;			// ポリゴン2Dクラス

//--------------------------------------------------------------------------------------------------------------
// ポーズ画面クラス(派生クラス)
//--------------------------------------------------------------------------------------------------------------
class CPause : public CScene
{
public:
	// ポーズで使用するUIの種類　
	typedef enum
	{
		PAUSEUI_NONE = 0,	// ポーズ背景
		PAUSEUI_MENUBG,		// メニュー背景
		PAUSEUI_CONTINUE,	// コンティニュー
		PAUSEUI_RETRY,		// リトライ
		PAUSEUI_QUIT,		// 終了
		PAUSEUI_MAX,
	}PAUSEUI;

	CPause(int nPriolity);
	~CPause();
	static HRESULT Load(void);
	static void Unload(void);
	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);				
	void Update(void);				
	void Draw(void);				

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[PAUSEUI_MAX];		// ポーズ画面のテクスチャのポインタ
	CScene2D *m_apScene2D[PAUSEUI_MAX];						// 2Dポリゴン情報
	int m_nPauseSelect;										// ポーズメニューから遷移する場所を保存
};

#endif