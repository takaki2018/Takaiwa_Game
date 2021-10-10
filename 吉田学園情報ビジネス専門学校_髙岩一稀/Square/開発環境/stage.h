//==============================================================================================================
//
// �X�e�[�W�w�b�_�[ (stage.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TILE		(512)	// �^�C���̍ő吔
#define MAX_WDITH		(40)	// �^�C���̍ő啝
#define MAX_HEIGHT		(30)	// �^�C���̍ő卂��
#define MAX_EDGE		(64)	// �^�C���̒[�ő吔

//--------------------------------------------------------------------------------------------------------------
// �X�e�[�W�N���X(��{�N���X)
//--------------------------------------------------------------------------------------------------------------
class CStage
{
public:
	// �X�e�[�W�̎��
	typedef enum
	{
		STAGETYPE_BOX = 0,
		STAGETYPE_2,
		STAGETYPE_MAX,
	}STAGETYPE;

	// �^�C���̒[�[�ԍ��ۑ��p
	typedef struct
	{
		int nTileEdgeTop[MAX_EDGE];
		int nTileEdgeBottom[MAX_EDGE];
		int nTileEdgeRight[MAX_EDGE];
		int nTileEdgeLeft[MAX_EDGE];
	}TileEdge;

	// �^�C���̒[�����ۑ��p
	typedef struct
	{
		int nNumTileEdgeTop;
		int nNumTileEdgeBottom;
		int nNumTileEdgeRight;
		int nNumTileEdgeLeft;
	}NumTileEdge;

	CStage();
	~CStage();

	static bool LoadStageData(STAGETYPE stageType);	// �X�e�[�W����ǂݍ���
	static int GetNumPlayerSpawn(void) { return m_nNumPlayerSpawn; }
	static bool GetRevival(int nNum) { return m_bRevival[nNum]; }
	static void SetRevival(bool bRevival, int nNum) { m_bRevival[nNum] = bRevival; }
	static int GetNumTile(void) { return m_nNumTile; }
	static TileEdge GetTileEdge(void) { return m_tileEdge; }
	static NumTileEdge GetNumTileEdge(void) { return m_numTileEdge; }

private:
	static bool SetStageDetale(void);								// �X�e�[�W�̏ڍׂ�ݒ肷��
	static void SetEdgeInfo(int nCntX, int nCntY,int nCnttile);		// �[���̕ۑ�
	static char m_aStageData[STAGETYPE_MAX][128];					// �X�e�[�W�f�[�^���������e�L�X�g���i�[�p
	static char m_aStageInfo[128];									// �X�e�[�W�z�u���̃p�X�ۑ��p
	static int m_aMapData[MAX_WDITH][MAX_HEIGHT];					// �}�b�v�f�[�^�ۑ��p
	static int m_nCntX;												// �����̃u���b�N��
	static int m_nCntY;												// �c���̃u���b�N��
	static D3DXVECTOR3 m_pos;										// �^�C���̏����ʒu
	static int m_nNumPlayerSpawn;									// �v���C���[�̃X�|�[���ʒu
	static bool m_bRevival[MAX_TILE];								// ���������ǂ���
	static int m_nNumTile;											// �^�C���̑���
	static TileEdge m_tileEdge;										// �^�C���̒[�ԍ��̕ۑ�
	static NumTileEdge m_numTileEdge;								// �^�C���̒[�����̕ۑ��p
};	

#endif