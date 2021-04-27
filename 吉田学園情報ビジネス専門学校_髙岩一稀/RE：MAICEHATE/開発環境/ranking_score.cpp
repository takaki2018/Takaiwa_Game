//==============================================================================================================
//
// �����L���O�X�R�A (ranking_score.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "ranking_score.h"
#include "score.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"
#include "2Deffect.h"
#include "setparticle.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_TEXTURE				(1)											// �e�N�X�`���̍ő吔
#define TEXTURE_X				(10)										// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y				(1)											// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔
#define MAX_RANKING_SCORE		(5)											// ���ʂ̐�
#define MAX_SCORE_DIGIT			(8)											// ����
#define MAX_NUMBER				(MAX_SCORE_DIGIT * MAX_RANKING_SCORE)		// �w�i�̎��

#define RANKING_SCORE_X			(600.0f)									// �����L���O�X�R�A�̒��_���W(X)
#define RANKING_SCORE_Y			(220.0f)									// �����L���O�X�R�A�̒��_���W(Y)
#define RANKING_SCORE_WIDTH		(60.0f)										// �����L���O�X�R�A�̕�
#define RANKING_SCORE_HEIGHT	(90.0f)										// �����L���O�X�R�A�̍���
#define RANKING_SCORE_SPACE		(60.0f)										// �����L���O�X�R�A��z�u����Ԋu

#define FLASH_SCORE_INTERVAL	(8)										// �_�ł̃C���^�[�o��

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingScore = NULL;						// �����L���O�X�R�A�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRankingScore[MAX_TEXTURE] = {};				// �����L���O�X�R�A�̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posRankingScore;												// �����L���O�X�R�A�̒��_���W
D3DXCOLOR g_ScoreCol[MAX_RANKING_SCORE];										// �X�R�A�̐F
int g_aScore[MAX_RANKING_SCORE] = { 5000,4000,3000,2000,1000 };				// �����L���O�X�R�A�̏����X�R�A
int g_nCntScore;															// �\������X�R�A�̐�
int g_nSubScore;															// �X�R�A�̕ۑ���
bool g_bFlash;																// �_�ł����邩
int g_nFlashScore;															// �_�ł�����X�R�A
int g_nFlashScoreTimer;														// �_�Ń^�C�}�[
int g_SelectMode2;															// �ǂ̃��[�h��I��������

//--------------------------------------------------------------------------------------------------------------
// �����L���O�X�R�A�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitRankingScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posRankingScore = D3DXVECTOR3(RANKING_SCORE_X, RANKING_SCORE_Y, 0.0f);
	g_nCntScore = 0;
	g_nSubScore = 0;
	g_bFlash = false;
	g_nFlashScore = -1;
	g_nFlashScoreTimer = MAX_RANKING_SCORE;

	// �X�R�A�̐F�̏�����
	for (int nCnt = 0; nCnt < MAX_RANKING_SCORE; nCnt++)
	{
		g_ScoreCol[nCnt] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	}

	// �����L���O�̓���ւ�
	Ranking();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureRankingScore[0]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_NUMBER, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingScore,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nCntRankingRank = 0; nCntRankingRank < MAX_RANKING_SCORE; nCntRankingRank++)
	{
		for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + 100.0f * nCntRankingRank, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + 100.0f * nCntRankingRank, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + 100.0f * nCntRankingRank, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nCntDigit, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + 100.0f * nCntRankingRank, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;


			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_ScoreCol[g_nCntScore];
			pVtx[1].col = g_ScoreCol[g_nCntScore];
			pVtx[2].col = g_ScoreCol[g_nCntScore];
			pVtx[3].col = g_ScoreCol[g_nCntScore];

			// �X�R�A���i�[
			int nNumber = (g_aScore[nCntRankingRank] / (int)powf(10.0f, (float)(MAX_SCORE - (nCntDigit + 1))));

			// �e�N�X�`���̍X�V
			pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[1].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 1.0f / TEXTURE_Y);
			pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 0.0f);

			// ���̏��Ɉڂ�
			pVtx += 4;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingScore->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �����L���O�X�R�A�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitRankingScore(void)
{
	// �T�E���h�̈ꎞ��~
	StopSound();

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRankingScore != NULL)
	{
		g_pVtxBuffRankingScore->Release();
		g_pVtxBuffRankingScore = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntRanking = 0; nCntRanking < MAX_TEXTURE; nCntRanking++)
	{
		if (g_pTextureRankingScore[nCntRanking] != NULL)
		{
			g_pTextureRankingScore[nCntRanking]->Release();
			g_pTextureRankingScore[nCntRanking] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �����L���O�X�R�A�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateRankingScore(void)
{
	// ��ʑJ��
	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
	// �_��
	if (g_nFlashScore != -1)
	{
		if (g_nFlashScoreTimer == FLASH_SCORE_INTERVAL / 2)
		{// �F�̊m��
			g_ScoreCol[g_nFlashScore] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		}
		else if (g_nFlashScoreTimer == FLASH_SCORE_INTERVAL)
		{// �F�̊m��
			g_ScoreCol[g_nFlashScore] = D3DXCOLOR(0.8f, 0.1f, 0.1f, 1.0f);
			// �^�C�}�[�̏�����
			g_nFlashScoreTimer = 0;
		}
		g_nFlashScoreTimer++;

		// �\���̂̃|�C���^�ϐ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

		// ������
		for (int nCntRankingRank = 0; nCntRankingRank < MAX_RANKING_SCORE; nCntRankingRank++)
		{
			for (int nCntDigit = 0; nCntDigit < MAX_SCORE_DIGIT; nCntDigit++)
			{
				// ���_�J���[�̐ݒ�
				pVtx[0].col = g_ScoreCol[nCntRankingRank];
				pVtx[1].col = g_ScoreCol[nCntRankingRank];
				pVtx[2].col = g_ScoreCol[nCntRankingRank];
				pVtx[3].col = g_ScoreCol[nCntRankingRank];

				// ���̏��Ɉڂ�
				pVtx += 4;
			}
		}

		SetRankingEffect(D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * (MAX_SCORE_DIGIT - 1),
			g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + 100.0f * g_nFlashScore, 0.0f),
			true);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �����L���O�X�R�A�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawRankingScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankingScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_NUMBER; nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRankingScore[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �����L���O�̓���ւ�
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void Ranking(void)
{
	// �ϐ��錾
	int nScore = GetScore();
	int nSubScore = 0;

	if (g_aScore[4] <= nScore)
	{//	5�ʂ̃X�R�A���傫���Ƃ�
		g_aScore[4] = nScore;
	}

	// �����L���O�̍X�V
	for (int nCnt = 0; nCnt < MAX_RANKING_SCORE - 1; nCnt++)
	{
		for (int nCnt2 = nCnt + 1; nCnt2 < MAX_RANKING_SCORE; nCnt2++)
		{
			if (g_aScore[nCnt] <= g_aScore[nCnt2])
			{// �����L���O�X�R�A�X�V�����Ƃ�
				nSubScore = g_aScore[nCnt];
				g_aScore[nCnt] = g_aScore[nCnt2];
				g_aScore[nCnt2] = nSubScore;
			}
		}
	}
	// �ǂ��ɓ�������
	for (int nCnt = 0; nCnt < MAX_RANKING_SCORE; nCnt++)
	{
		if (g_aScore[nCnt] == nScore)
		{
			g_bFlash = true;
			g_nFlashScore = nCnt;
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �_�ł���X�R�A�����Ԗڂ��擾
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fint	- �_�ł��鏇�ʂ�Ԃ�
//--------------------------------------------------------------------------------------------------------------
int GetFlashNumber(void)
{
	return g_nFlashScore;
}