//-------------------------------------------------------------------
//
// �����L���O�w�b�_�[ (ranking.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitRanking(void);			// �����L���O�̏���������
void UninitRanking(void);			// �����L���O�̏I������
void UpdateRanking(void);			// �����L���O�̍X�V����
void DrawRanking(void);				// �����L���O�̕`�揈��
void SetVertexRanking(int nCnt);	// �����L���O�̒��_���W�̐ݒ�
void RankingBg(int nCnt);			// �����j���O�̔w�i
void RankingLogo(int nCnt);			// �����j���O�̃��S
void RankingRank(int nCnt);			// �����j���O�̏���
void RankingScore(int nCnt);		// �����j���O�̃X�R�A

#endif
