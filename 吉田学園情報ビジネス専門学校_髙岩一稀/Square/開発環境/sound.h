//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//-----------------------------------------------------------------------------
// サウンドクラス(基本クラス)
//-----------------------------------------------------------------------------
class CSound
{
public:
	CSound();
	~CSound();
	// サウンドファイル
	typedef enum
	{
		SOUND_LABEL_TITLE = 0,			// タイトルBGM
		SOUND_LABEL_GAME,				// ゲームBGM
		SOUND_LABEL_RESULT,				// リザルトBGM
		SOUND_LABEL_SE_SHOOT,			// 弾発射
		SOUND_LABEL_SE_HIT_PLAYER,		// プレイヤーの爆発
		SOUND_LABEL_SE_MOVE_CURSOR,		// カーソルの移動
		SOUND_LABEL_SE_COUNTDOWN,		// カウントダウン
		SOUND_LABEL_SE_GAME_START,		// ゲーム開始
		SOUND_LABEL_SE_HITENEMY,		// 敵撃破
		SOUND_LABEL_SE_PAUSE_IN,		// ポーズイン
		SOUND_LABEL_SE_PAUSE_OUT,		// ポーズアウト
		SOUND_LABEL_SE_GAME_TRANSITION,	// ゲーム遷移
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	static HRESULT Play(SOUND_LABEL label);
	static void Stop(SOUND_LABEL label);
	static void Stop(void);

private:
	// パラメータ構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// オーディオデータサイズ

	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};

#endif
