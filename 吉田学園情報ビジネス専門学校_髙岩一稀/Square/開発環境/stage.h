//==============================================================================================================
//
// �X�e�[�W�w�b�_�[ (stage.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

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

	CStage();
	~CStage();

	static bool LoadStageData(STAGETYPE stageType);	// �X�e�[�W����ǂݍ���
	static int GetNumPlayerSpawn(void) { return m_nNumPlayerSpawn; }
	static bool GetRevival(int nNum) { return m_bRevival[nNum]; }
	static void SetRevival(bool bRevival, int nNum) { m_bRevival[nNum] = bRevival; }
	static int GetNumTile(void) { return m_nNumTile; }
	static int *GetTileEdgeSide(void) { return &m_nNumTileEdgeSide[0]; }
	static int GetCntTileEdgeSide(void) { return m_nCnttileEdgeSide; }
	static int *GetTileEdgeVertical(void) { return &m_nNumTileEdgeVertical[0]; }
	static int GetCntTileEdgeVertical(void) { return m_nCnttileEdgeVertical; }

private:
	static bool SetStageDetale(void);				// �X�e�[�W�̏ڍׂ�ݒ肷��
	static char m_aStageData[STAGETYPE_MAX][128];	// �X�e�[�W�f�[�^���������e�L�X�g���i�[�p
	static char m_aStageInfo[128];					// �X�e�[�W�z�u���̃p�X�ۑ��p
	static int m_nCntX;								// �����̃u���b�N��
	static int m_nCntY;								// �c���̃u���b�N��
	static D3DXVECTOR3 m_pos;						// �^�C���̏����ʒu
	static int m_nNumPlayerSpawn;					// �v���C���[�̃X�|�[���ʒu
	static bool m_bRevival[512];					// ���������ǂ���
	static int m_nNumTile;							// �^�C���̑���
	static int m_nNumTileEdgeSide[64];				// �^�C���̒[��ۑ�
	static int m_nCnttileEdgeSide;					// �^�C���̒[����
	static int m_nNumTileEdgeVertical[64];			// �^�C���̒[��ۑ�
	static int m_nCnttileEdgeVertical;				// �^�C���̒[����
};	

#endif