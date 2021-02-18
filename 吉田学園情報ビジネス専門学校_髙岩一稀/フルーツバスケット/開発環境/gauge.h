//-------------------------------------------------------------------
//
// �Q�[�W�w�b�_�[ (gauge.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	float fMoveHeight;		// �����̈ړ���
	int nScore;				// �X�R�A��ۑ�
	int nRank;				// ����
	int nCntScore;			// �X�R�A�̃J�E���g
	int nCntTex;			// ���ʂ̃e�N�X�`�����͂邩
	bool bReach;			// ���B�������ǂ���
}GAUGE;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitGauge(void);			// �Q�[�W�̏���������
void UninitGauge(void);				// �Q�[�W�̏I������
void UpdateGauge(void);				// �Q�[�W�̍X�V����
void DrawGauge(void);				// �Q�[�W�̕`�揈��
void RankingGauge(void);			// �����L���O�̊m��
float GetMoveGauge(int nCnt);	    // �Q�[�W�̈ړ��ʂ̎擾
bool GetbGauge(void);				// �Q�[�W���オ�肫�������ǂ���

#endif
