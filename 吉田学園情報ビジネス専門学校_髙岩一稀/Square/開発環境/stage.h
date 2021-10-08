//==============================================================================================================
//
// ステージヘッダー (stage.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//--------------------------------------------------------------------------------------------------------------
// ステージクラス(基本クラス)
//--------------------------------------------------------------------------------------------------------------
class CStage
{
public:
	// ステージの種類
	typedef enum
	{
		STAGETYPE_BOX = 0,
		STAGETYPE_2,
		STAGETYPE_MAX,
	}STAGETYPE;

	CStage();
	~CStage();

	static bool LoadStageData(STAGETYPE stageType);	// ステージ情報を読み込む
	static int GetNumPlayerSpawn(void) { return m_nNumPlayerSpawn; }
	static bool GetRevival(int nNum) { return m_bRevival[nNum]; }
	static void SetRevival(bool bRevival, int nNum) { m_bRevival[nNum] = bRevival; }
	static int GetNumTile(void) { return m_nNumTile; }
	static int *GetTileEdgeSide(void) { return &m_nNumTileEdgeSide[0]; }
	static int GetCntTileEdgeSide(void) { return m_nCnttileEdgeSide; }
	static int *GetTileEdgeVertical(void) { return &m_nNumTileEdgeVertical[0]; }
	static int GetCntTileEdgeVertical(void) { return m_nCnttileEdgeVertical; }

private:
	static bool SetStageDetale(void);				// ステージの詳細を設定する
	static char m_aStageData[STAGETYPE_MAX][128];	// ステージデータが入ったテキスト名格納用
	static char m_aStageInfo[128];					// ステージ配置情報のパス保存用
	static int m_nCntX;								// 横幅のブロック数
	static int m_nCntY;								// 縦幅のブロック数
	static D3DXVECTOR3 m_pos;						// タイルの初期位置
	static int m_nNumPlayerSpawn;					// プレイヤーのスポーン位置
	static bool m_bRevival[512];					// 復活中かどうか
	static int m_nNumTile;							// タイルの総数
	static int m_nNumTileEdgeSide[64];				// タイルの端を保存
	static int m_nCnttileEdgeSide;					// タイルの端総数
	static int m_nNumTileEdgeVertical[64];			// タイルの端を保存
	static int m_nCnttileEdgeVertical;				// タイルの端総数
};	

#endif