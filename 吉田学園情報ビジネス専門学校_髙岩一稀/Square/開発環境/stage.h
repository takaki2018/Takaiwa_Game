//==============================================================================================================
//
// ステージヘッダー (stage.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TILE		(512)	// タイルの最大数
#define MAX_WDITH		(40)	// タイルの最大幅
#define MAX_HEIGHT		(30)	// タイルの最大高さ
#define MAX_EDGE		(64)	// タイルの端最大数

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

	// タイルの端端番号保存用
	typedef struct
	{
		int nTileEdgeTop[MAX_EDGE];
		int nTileEdgeBottom[MAX_EDGE];
		int nTileEdgeRight[MAX_EDGE];
		int nTileEdgeLeft[MAX_EDGE];
	}TileEdge;

	// タイルの端総数保存用
	typedef struct
	{
		int nNumTileEdgeTop;
		int nNumTileEdgeBottom;
		int nNumTileEdgeRight;
		int nNumTileEdgeLeft;
	}NumTileEdge;

	CStage();
	~CStage();

	static bool LoadStageData(STAGETYPE stageType);	// ステージ情報を読み込む
	static int GetNumPlayerSpawn(void) { return m_nNumPlayerSpawn; }
	static bool GetRevival(int nNum) { return m_bRevival[nNum]; }
	static void SetRevival(bool bRevival, int nNum) { m_bRevival[nNum] = bRevival; }
	static int GetNumTile(void) { return m_nNumTile; }
	static TileEdge GetTileEdge(void) { return m_tileEdge; }
	static NumTileEdge GetNumTileEdge(void) { return m_numTileEdge; }

private:
	static bool SetStageDetale(void);								// ステージの詳細を設定する
	static void SetEdgeInfo(int nCntX, int nCntY,int nCnttile);		// 端情報の保存
	static char m_aStageData[STAGETYPE_MAX][128];					// ステージデータが入ったテキスト名格納用
	static char m_aStageInfo[128];									// ステージ配置情報のパス保存用
	static int m_aMapData[MAX_WDITH][MAX_HEIGHT];					// マップデータ保存用
	static int m_nCntX;												// 横幅のブロック数
	static int m_nCntY;												// 縦幅のブロック数
	static D3DXVECTOR3 m_pos;										// タイルの初期位置
	static int m_nNumPlayerSpawn;									// プレイヤーのスポーン位置
	static bool m_bRevival[MAX_TILE];								// 復活中かどうか
	static int m_nNumTile;											// タイルの総数
	static TileEdge m_tileEdge;										// タイルの端番号の保存
	static NumTileEdge m_numTileEdge;								// タイルの端総数の保存用
};	

#endif